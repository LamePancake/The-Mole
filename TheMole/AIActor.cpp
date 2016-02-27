#include "AIActor.h"
#include "GameScreen.h"
#include <cstring>

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
		std::memcpy(shadowColour, _mindCtrlShadow, sizeof(Uint8) * 3);
		std::memcpy(colour, _mindCtrlColour, sizeof(Uint8) * 3);

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
		std::memcpy(shadowColour, _mindCtrlShadow, sizeof(Uint8) * 3);
		std::memcpy(colour, _mindCtrlColour, sizeof(Uint8) * 3);
	}
	else
	{
		std::memcpy(shadowColour, _normalShadow, sizeof(Uint8) * 3);
		std::memcpy(colour, _normalColour, sizeof(Uint8) * 3);
	}

	// Draw shadow first, so we need to adjust drawing parameters
	SDL_SetTextureColorMod(rawTexture, shadowColour[0], shadowColour[1], shadowColour[2]);
	SDL_SetTextureAlphaMod(rawTexture, 127);
	spriteSheet->Draw(shadowRect, xDir, _spriteYDir);

	SDL_SetTextureColorMod(rawTexture, colour[0], colour[1], colour[2]);
	SDL_SetTextureAlphaMod(rawTexture, 255);
	spriteSheet->Draw(destRect, xDir, _spriteYDir);
}
