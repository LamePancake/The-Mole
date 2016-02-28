#include "AIActor.h"
#include "GameScreen.h"
#include <cstring>

const static Uint8 NORMAL_SHADOW[3] = { 127, 127, 127 };
const static Uint8 NORMAL_COLOUR[3] = { 255, 255, 255 };

const static Uint8 MIND_CTRL_SHADOW[3] = { 95, 127, 95 };
const static Uint8 MIND_CTRL_COLOUR[3] = { 190, 255, 190 };

const static double MIND_CONTROL_TIME = 15.0;
const static double PULSE_START_TIME = 0.75;
const static double PULSE_END_TIME = 0.05;
const static double PULSE_MULTIPLIER = (((PULSE_START_TIME - (PULSE_START_TIME * PULSE_END_TIME)) / MIND_CONTROL_TIME) - 1) * -1; // Don't ask

AIActor::~AIActor()
{
}

void AIActor::SetSelectedForControl(bool selected)
{
	_isSelected = selected;
}

void AIActor::SetIsMindControlCandidate(bool isCandidate)
{
	_isCandidate = isCandidate;

	// We're finishing up mind control selection and we're under control, so determine velocity
	if(!_isCandidate && _underControl)
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

void AIActor::Update(double elapsedSecs)
{
	Actor::Update(elapsedSecs);

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

	// While the AI is alive, do stuff.
	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	if (_health <= 0)
	{
		//std::cout << "Dead\n";
	}
	else
	{
		_aabb.UpdatePosition(*this);
		UpdatePosition(elapsedSecs);
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

	if (_collisionInfo.rowEdge != Edge::NONE)
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
				_health = 0;
			default:
				_curKinematic.position.SetY(correctedYPos);
				break;
			}
		}
	}
	
	if (_collisionInfo.colEdge != Edge::NONE)
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
				_health = 0;
			default:
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
	SDL2pp::Point curPos = SDL2pp::Point(_curKinematic.position.GetX(), _curKinematic.position.GetY());

	// Values to modify
	SDL2pp::Rect destRect(curPos.x - viewport.x, curPos.y - viewport.y, _sprites[_currentSpriteSheet]->GetFrameWidth(), _sprites[_currentSpriteSheet]->GetFrameHeight());
	SDL2pp::Rect shadowRect(destRect);
	Uint8 shadowColour[3];
	Uint8 colour[3];
	SpriteSheet::XAxisDirection xDir = _spriteXDir;

	shadowRect.x -= offsetX;
	shadowRect.y -= offsetY;

	if (_isCandidate)
	{
		std::memcpy(shadowColour, MIND_CTRL_SHADOW, sizeof(Uint8) * 3);
		std::memcpy(colour, MIND_CTRL_COLOUR, sizeof(Uint8) * 3);

		if (_isSelected)
		{
			destRect.x -= (destRect.w * 0.3);
			destRect.y -= (destRect.h * 0.3);
			destRect.w *= 1.3;
			destRect.h *= 1.3;

			shadowRect.x -= (shadowRect.w * 0.3);
			shadowRect.y -= (shadowRect.h * 0.3);
			shadowRect.w *= 1.3;
			shadowRect.h *= 1.3;
		}
		xDir = _underControl ? _controlDir : _spriteXDir;
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
	SDL_SetTextureColorMod(rawTexture, shadowColour[0], shadowColour[1], shadowColour[2]);
	SDL_SetTextureAlphaMod(rawTexture, 127);
	spriteSheet->Draw(shadowRect, xDir, _spriteYDir);

	SDL_SetTextureColorMod(rawTexture, colour[0], colour[1], colour[2]);
	SDL_SetTextureAlphaMod(rawTexture, 255);
	spriteSheet->Draw(destRect, xDir, _spriteYDir);

	if (_underControl)
	{
		SDL2pp::Rect controlRect;
		controlRect.y = destRect.y - (_ctrlIndicator->GetHeight() - destRect.h);
		controlRect.x = _curKinematic.position.GetX() - viewport.x;
		controlRect.w = _ctrlIndicator->GetWidth();
		controlRect.h = _ctrlIndicator->GetHeight();
		_mgr->GetRenderer().Copy(*_ctrlIndicator, SDL2pp::NullOpt, controlRect);
	}
}
