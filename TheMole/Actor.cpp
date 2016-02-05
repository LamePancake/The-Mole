#include "Actor.h"

Actor::Actor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath, std::string shadowTexturePath)
	:_position(position), _mgr(&manager), _speed(spd)
{
	_health = 100;
	
	_sprite = std::make_shared<SpriteSheet>(texturePath, 8, 1.0, SpriteSheet::XAxisDirection::RIGHT);
	_spriteShadow = std::make_shared<SpriteSheet>(shadowTexturePath, 8, 1.0, SpriteSheet::XAxisDirection::RIGHT);

	_aabb = AABB(_sprite->GetFrameWidth(), _sprite->GetFrameHeight(), *this);

	SDL_SetTextureColorMod(_spriteShadow->GetTexture().Get(), 127, 127, 127);
	SDL_SetTextureAlphaMod(_spriteShadow->GetTexture().Get() , 127);

	_actorDir = SpriteSheet::XAxisDirection::RIGHT;
}

Actor::~Actor()
{
}

AABB Actor::GetAABB()
{
	return _aabb;
}

std::shared_ptr<SpriteSheet> Actor::GetTexture()
{
	return _sprite;
}

std::shared_ptr<SpriteSheet> Actor::GetTextureShadow()
{
	return _spriteShadow;
}

Vector2 Actor::GetPosition()
{
	return _position;
}

Vector2 Actor::GetSpeed()
{
	return _speed;
}

size_t Actor::GetHealth()
{
	return _health;
}

void Actor::SetSpeed(Vector2 spd)
{
	_speed = spd;
}

void Actor::SetHealth(size_t health)
{
	_health = health;
}

void Actor::SetPosition(Vector2 pos)
{
	_position = pos;
}

SpriteSheet::XAxisDirection Actor::GetActorDirection()
{
	return _actorDir;
}

void Actor::SetActorDirection(SpriteSheet::XAxisDirection dir)
{
	_actorDir = dir;
}

void Actor::Update(double elapsedSecs, std::shared_ptr<Level>& level)
{
	_sprite->Update(elapsedSecs);
	_spriteShadow->Update(elapsedSecs);
}

void Actor::UpdatePosition(double elapsedSecs)
{
}


void Actor::Draw(Camera& camera)
{
	const SDL2pp::Rect& viewport = camera.GetViewport();
	int offsetX = 4;
	int offsetY = 0;
			
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	SDL2pp::Point tempPoint;

	tempPoint = { (int)_position.GetX(), (int)_position.GetY() };

	_spriteShadow->Draw(tempPoint + SDL2pp::Point(offsetX - viewport.x, offsetY - viewport.y), _actorDir);
	_sprite->Draw(tempPoint + SDL2pp::Point(-viewport.x, -viewport.y), _actorDir);
}

void Actor::GetTileCollisionInfo(Edge & xEdge, Edge & yEdge, int & xPenetration, int & yPenetration, 
	std::vector<std::shared_ptr<Tile>>& xIntersect, std::vector<std::shared_ptr<Tile>>& yIntersect, std::shared_ptr<Level>& level)
{
	int tileWidth = level->GetTileWidth();
	int tileHeight = level->GetTileHeight();

	// Calculate the actor's bounds
	// Note that this should use the AABB, but it's reporting incorrect positions currently
	double rightBound = _position.GetX() + _sprite->GetFrameWidth();
	double leftBound = _position.GetX();
	double topBound = _position.GetY();
	double bottomBound = _position.GetY() + _sprite->GetFrameHeight();

	// Determine which tiles we intersect
	// If the bottom or right is exactly flush, we subtract one so that we don't test against tiles in the next row/column
	int topRow = (int)floor(topBound / tileHeight);
	int bottomRow = (int)floor(bottomBound / tileHeight) - ((int)bottomBound % tileHeight == 0 ? 1 : 0);
	int leftCol = (int)floor(leftBound / tileWidth);
	int rightCol = (int)floor(rightBound / tileWidth) - ((int)rightBound % tileWidth == 0 ? 1 : 0);

	double xVel = _speed.GetX();
	if (xVel)
	{
		xEdge = xVel > 0 ? Edge::RIGHT : Edge::LEFT;

		// If we're moving right, we need to test x + width; if we're moving left, we need to test x
		int testSideX = xVel > 0 ? (int)ceil(rightBound) : (int)floor(leftBound);

		// If we aren't flush with a tile column border, check whether we're intersecting any tiles of interest
		xPenetration = testSideX % tileWidth;
		int col = testSideX / tileWidth;
		level->GetTileRange(topRow, bottomRow + 1, col, col + 1, xIntersect);		
	}
	else
	{
		xEdge = Edge::NONE;
	}

	double yVel = _speed.GetY();
	if (yVel)
	{
		yEdge = yVel > 0 ? Edge::BOTTOM : Edge::TOP;

		// If we're moving right, we need to test x + width; if we're moving left, we need to test x
		int testSideY = yVel > 0 ? (int)ceil(bottomBound) : (int)floor(topBound);

		yPenetration = testSideY % tileHeight;
		int row = testSideY / tileHeight;
		level->GetTileRange(row, row, leftCol, rightCol + 1, yIntersect);
	}
	else
	{
		yEdge = Edge::NONE;
	}
}

