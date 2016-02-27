#include "AIActor.h"
#include "GameScreen.h"

AIActor::~AIActor()
{
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
				_curKinematic.velocity.SetX(reverseX);
				_spriteXDir = reverseDir;
				break;
			}
		}
	}
}

void AIActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
}

void AIActor::Draw(Camera& camera)
{
	if (!_isVisible) return;

	const SDL2pp::Rect& viewport = camera.GetViewport();
	int offsetX = 4;
	int offsetY = 0;

	SDL2pp::Renderer& rend = _mgr->GetRenderer();

	std::shared_ptr<SpriteSheet> spriteSheet = _sprites[_currentSpriteSheet];
	SDL_Texture* rawTexture = spriteSheet->GetTexture().Get();
	SDL2pp::Point tempPoint = SDL2pp::Point((int)_curKinematic.position.GetX(), (int)_curKinematic.position.GetY());

	// Draw shadow first, so we need to adjust drawing parameters
	SDL_SetTextureColorMod(rawTexture, 95, 127, 95);
	SDL_SetTextureAlphaMod(rawTexture, 127);
	spriteSheet->Draw(tempPoint + SDL2pp::Point(offsetX - viewport.x, offsetY - viewport.y), _spriteXDir, _spriteYDir);

	SDL_SetTextureColorMod(rawTexture, 190, 255, 190);
	SDL_SetTextureAlphaMod(rawTexture, 255);
	spriteSheet->Draw(tempPoint + SDL2pp::Point(-viewport.x, -viewport.y), _spriteXDir, _spriteYDir);
}
