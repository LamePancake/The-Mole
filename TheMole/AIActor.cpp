#include "AIActor.h"
#include "BombAIActor.h"
#include "GameScreen.h"
#include "DoorActor.h"
#include <cmath>
#include <cstring>

const static Uint8 NORMAL_SHADOW[3] = { 127, 127, 127 };
const static Uint8 NORMAL_COLOUR[3] = { 255, 255, 255 };

const static Uint8 MIND_CTRL_SHADOW[3] = { 95, 127, 95 };
const static Uint8 MIND_CTRL_COLOUR[3] = { 190, 255, 190 };

const static double MIND_CONTROL_TIME = 15.0;
const static double PULSE_START_TIME = 0.75;
const static double PULSE_END_TIME = 0.05;
const static double PULSE_MULTIPLIER = ((PULSE_END_TIME - PULSE_START_TIME) / MIND_CONTROL_TIME) + 1; // Don't ask

using std::shared_ptr;

AIActor::AIActor(std::string & serialised)
    : Actor(serialised), _underControl{ false }, _controlTimeLeft{ 0.0f }, _isSelected{ false }, _isCandidate{ false }
{
    std::istringstream lineStream(serialised);
    string line;
    getline(lineStream, line);
    line.erase(std::remove(line.end() - 1, line.end(), '\r'), line.end());

    // Get spawn point
    std::istringstream spawnPosStream(line);
    double pos;
    spawnPosStream >> pos;
    _spawn.SetX(pos);
    spawnPosStream >> pos;
    _spawn.SetY(pos);

    // Get mind control indicator texture
    getline(lineStream, line);
    line.erase(std::remove(line.end() - 1, line.end(), '\r'), line.end());
    _ctrlIndicator = shared_ptr<SDL2pp::Texture>(new SDL2pp::Texture(_mgr->GetRenderer(), line));

    serialised.erase(0, lineStream.tellg());
}

AIActor::~AIActor()
{
}

void AIActor::SetSelectedForControl(bool selected)
{
	_isSelected = selected;
}

void AIActor::SetIsMindControlCandidate(bool isCandidate)
{
    // Don't allow mind control while exploding
    if (GetType() == Type::bombenemy)
    {
        BombAIActor* bomb = (BombAIActor*)this;
        if (bomb->IsBlowingUp()) return;
    }

	_isCandidate = isCandidate;

	// We're finishing up mind control selection and we're under control, so determine velocity
	if(!_isCandidate)
	{
		_sprites[_currentSpriteSheet]->SetScale(1.0);
		if (_underControl)
		{
			// Change the x direction and speed appropriately
			_spriteXDir = _controlDir;
			int multiplier = _spriteXDir == SpriteSheet::XAxisDirection::LEFT ? -1 : 1;
			float xSpeed = std::fabsf(_curKinematic.velocity.GetX());
			_curKinematic.velocity.SetX(xSpeed * multiplier);
			_controlTimeLeft = MIND_CONTROL_TIME;
			_currentPulseTime = 0;
			_pulseTimeTotal = PULSE_START_TIME;
		}
	}
}

void AIActor::StopMindControl()
{
	_underControl = false;
}

void AIActor::SetMindControlDirection(SpriteSheet::XAxisDirection direction)
{
	_controlDir = direction;
	_underControl = true;
}

bool AIActor::IsUnderMindControl() const
{
	return _underControl;
}

SpriteSheet::XAxisDirection AIActor::GetMindControlDirection() const
{
	return _controlDir;
}

Vector2 AIActor::GetSpawnPoint() const
{
    return _spawn;
}

void AIActor::Update(double elapsedSecs)
{
	Actor::Update(elapsedSecs);
    if (_isDestroyed || !_isActive) return;

	if (_underControl)
	{
		_controlTimeLeft -= elapsedSecs;
		if (_controlTimeLeft <= 0)
		{
			StopMindControl();
		}
		else
		{
			_currentPulseTime += elapsedSecs;
			if (_currentPulseTime > _pulseTimeTotal)
			{
				_pulseTimeTotal *= PULSE_MULTIPLIER;
				_currentPulseTime = 0;
			}
		}
	}

	if (_health <= 0)
	{
		SetVisibility(false);
        SetActive(false);
	}
	else
	{
		UpdatePosition(elapsedSecs);
        _aabb.UpdatePosition(*this);

		for (auto actor : _gameScreen->GetLevel()->GetActors())
		{
            if (actor.get() == (Actor*)this || !actor->IsActive()) continue;
			if (_aabb.CheckCollision(actor->GetAABB()))
			{
				Type type = actor->GetType();
				switch (type)
				{
				case Type::door:
                {
                    shared_ptr<DoorActor> door = dynamic_pointer_cast<DoorActor>(actor);
                    if (!door->IsOpen())
                    {
                        Edge edge = door->GetEdge();
                        bool affectsY = edge == Edge::BOTTOM || edge == Edge::TOP;
                        Vector2 overlap = _aabb.GetOverlap(actor->GetAABB(), true);
                        // Push our hero out of the door
                        if (affectsY)
                        {
                            int extraOff = door->GetPosition().GetY() > _curKinematic.position.GetY() ? -1 : 1;
                            _curKinematic.position.SetY(_curKinematic.position.GetY() + overlap.GetY() + extraOff); // + extraOff since there might be some rounding problems
                            _prevKinematic.position.SetY(_curKinematic.position.GetY());
                        }
                        else
                        {
                            int extraOff = door->GetPosition().GetX() > _curKinematic.position.GetX() ? -1 : 1;
                            _curKinematic.position.SetX(_curKinematic.position.GetX() + overlap.GetX() + extraOff);
                            _prevKinematic.position.SetX(_curKinematic.position.GetX());
                            if (!_underControl)
                            {
                                float reverseX = _curKinematic.velocity.GetX() * -1;
                                SpriteSheet::XAxisDirection reverseDir = _spriteXDir == SpriteSheet::XAxisDirection::LEFT ? SpriteSheet::XAxisDirection::RIGHT : SpriteSheet::XAxisDirection::LEFT;
                                _curKinematic.velocity.SetX(reverseX);
                                _spriteXDir = reverseDir;
                            }
                        }
                    }
                }
					break;
                case Type::bombenemy:
                {
                    shared_ptr<BombAIActor> bomber = dynamic_pointer_cast<BombAIActor>(actor);
                    if (!bomber->IsBlowingUp() && bomber->IsUnderMindControl())
                    {
                        bomber->BlowUp();
                    }
                    
                    if (bomber->IsBlowingUp())
                    {
                        // Yet another hack
                        // If we're actually a bomb AI, then we need to explode instead of simply dying
                        if (GetType() == Type::bombenemy && !((BombAIActor*)this)->IsBlowingUp())
                        {
                            ((BombAIActor*)this)->BlowUp();
                        }
                        else
                        {
                            SetVisibility(false);
                            SetActive(false);
                        }
                    }
                }
                    break;
				}
			}
		}
		ScanNeighbouringTiles(_gameScreen->GetLevel());
	}
    _prevKinematic = _curKinematic;
}

void AIActor::UpdatePosition(double elapsedSecs)
{
	// Copy everything over so that we can use this in collision detection stuff later
	_prevKinematic = _curKinematic;

	Actor::UpdatePosition(elapsedSecs);

	_curKinematic.position.SetX(_curKinematic.position.GetX() + _curKinematic.velocity.GetX() * elapsedSecs);
	_curKinematic.position.SetY(_curKinematic.position.GetY() + _curKinematic.velocity.GetY() * elapsedSecs);
}

bool AIActor::CollisionCheck(Actor &otherActor)
{
	return _aabb.CheckCollision(otherActor.GetAABB());
}

void AIActor::ScanNeighbouringTiles(std::shared_ptr<Level>& level)
{
    _collisionInfo.colIntersect.clear();
    _collisionInfo.rowIntersect.clear();
	DetectTileCollisions(_collisionInfo, level);

	if (_collisionInfo.shouldCorrectY)
	{
		float correctedYPos = _curKinematic.position.GetY();
		if (_collisionInfo.rowEdge == Edge::BOTTOM) correctedYPos -= _collisionInfo.rowPenetration;
		else if (_collisionInfo.rowEdge == Edge::TOP) correctedYPos += _collisionInfo.rowPenetration;

		for (auto& tile : _collisionInfo.rowIntersect)
		{
			switch (tile->GetID())
			{
			case Tile::blank:
				break;
			case Tile::spike:
				SetHealth(0);
			default:
				_curKinematic.position.SetY(correctedYPos);
				break;
			}
		}
	}
	
	if (_collisionInfo.shouldCorrectX)
	{
		float correctedXPos = _curKinematic.position.GetX();
		if (_collisionInfo.colEdge == Edge::RIGHT) correctedXPos -= _collisionInfo.colPenetration;
		else if (_collisionInfo.colEdge == Edge::LEFT) correctedXPos += _collisionInfo.colPenetration;

		float reverseX = _curKinematic.velocity.GetX() * -1;
		SpriteSheet::XAxisDirection reverseDir = _spriteXDir == SpriteSheet::XAxisDirection::LEFT ? SpriteSheet::XAxisDirection::RIGHT : SpriteSheet::XAxisDirection::LEFT;
		for (auto& tile : _collisionInfo.colIntersect)
		{
			switch (tile->GetID())
			{
			case Tile::blank:
				break;
			case Tile::spike:
				SetHealth(0);
			default:
				float beforeX = _curKinematic.position.GetX();
                if (_collisionInfo.colPenetration == 0) continue;
				_curKinematic.position.SetX(correctedXPos);

				// If we're being mind-controlled, we want to continue walking this way
				if (!_underControl) 
				{
					_curKinematic.velocity.SetX(reverseX);
					_spriteXDir = reverseDir;
				}
				break;
			}
		}
	}
}

void AIActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
	SetSpeed(Vector2(100.0f, 341.3f));
	_sprites[_currentSpriteSheet]->Stop();
	_currentSpriteSheet = "walk";
	_sprites[_currentSpriteSheet]->Start();
	_sprites[_currentSpriteSheet]->SetScale(1.0);

	_underControl = false;

	_curKinematic = KinematicState(pos, GetSpeed(), 0.f, 0.f);
	_prevKinematic = _curKinematic;

	_collisionInfo.colEdge = Edge::NONE;
	_collisionInfo.colIntersect.clear();
	_collisionInfo.colPenetration = 0;
	_collisionInfo.rowEdge = Edge::NONE;
	_collisionInfo.rowIntersect.clear();
	_collisionInfo.rowPenetration = 0;

    SetVisibility(true);
    SetActive(true);
}

AIActor * AIActor::Clone()
{
    return new AIActor(*this);
}

void AIActor::CancelMindControl()
{
    _underControl = false;
}

void AIActor::GetPulseColour(const Uint8* startColour, const Uint8* endColour, Uint8* result)
{
	double percent = _currentPulseTime / _pulseTimeTotal;
	result[0] = startColour[0] + ((endColour[0] - startColour[0]) * percent);
	result[1] = startColour[1] + ((endColour[1] - startColour[1]) * percent);
	result[2] = startColour[2] + ((endColour[2] - startColour[2]) * percent);
}

void AIActor::Draw(Camera& camera)
{
	if (!_isVisible) return;

	const SDL2pp::Rect& viewport = camera.GetViewport();
	SDL2pp::Renderer& rend = _mgr->GetRenderer();

	// Offsets for drawing the shadow
	int offsetX = 4;
	int offsetY = 0;

	std::shared_ptr<SpriteSheet> spriteSheet = _sprites[_currentSpriteSheet];
	SDL_Texture* rawTexture = spriteSheet->GetTexture().Get();

	SDL2pp::Point curPos = SDL2pp::Point((int)_curKinematic.position.GetX(), (int)_curKinematic.position.GetY());

	// Values to modify
	SDL2pp::Point dest(curPos.x - viewport.x, curPos.y - viewport.y);
	SDL2pp::Point shadow(dest);
	Uint8 shadowColour[3];
	Uint8 colour[3];
	SpriteSheet::XAxisDirection xDir = _spriteXDir;

	shadow.x -= offsetX;
	shadow.y -= offsetY;

	if (_isCandidate)
	{
		std::memcpy(shadowColour, MIND_CTRL_SHADOW, sizeof(Uint8) * 3);
		std::memcpy(colour, MIND_CTRL_COLOUR, sizeof(Uint8) * 3);
		xDir = _underControl ? _controlDir : _spriteXDir;

		spriteSheet->SetScale(_isSelected ? 1.3 : 1.0);
	}
	else if (_underControl)
	{
		// TODO: Get the pulse colour; for now just copy the colours as-is
		GetPulseColour(MIND_CTRL_SHADOW, NORMAL_SHADOW, shadowColour);
		GetPulseColour(MIND_CTRL_COLOUR, NORMAL_COLOUR, colour);
	}
	else
	{
		std::memcpy(shadowColour, NORMAL_SHADOW, sizeof(Uint8) * 3);
		std::memcpy(colour, NORMAL_COLOUR, sizeof(Uint8) * 3);
	}

	// Draw shadow first, so we need to adjust drawing parameters
	spriteSheet->SetColourMod({shadowColour[0], shadowColour[1], shadowColour[2], 127});
	spriteSheet->SetXAxisDirection(xDir);
	spriteSheet->Draw(shadow);

	spriteSheet->SetColourMod({ colour[0], colour[1], colour[2], 255 });
	spriteSheet->SetXAxisDirection(xDir);
	spriteSheet->Draw(dest);

	// Draw the indicator
	if (_underControl)
	{
		SDL2pp::Rect controlRect;
		controlRect.y = dest.y - (_ctrlIndicator->GetHeight() - spriteSheet->GetFrameHeight());
		controlRect.x = _curKinematic.position.GetX() - viewport.x;
		controlRect.w = _ctrlIndicator->GetWidth();
		controlRect.h = _ctrlIndicator->GetHeight();
		_mgr->GetRenderer().Copy(*_ctrlIndicator, SDL2pp::NullOpt, controlRect);
	}
}
