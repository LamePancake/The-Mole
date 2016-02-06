#include "Actor.h"

Actor::Actor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath, int framesPerSecond)
	:_position(position), _mgr(&manager), _speed(spd)
{
	_health = 100;
	
	_sprite = std::make_shared<SpriteSheet>(texturePath, framesPerSecond, 1.0, SpriteSheet::XAxisDirection::RIGHT);
	_spriteShadow = std::make_shared<SpriteSheet>(texturePath, framesPerSecond, 1.0, SpriteSheet::XAxisDirection::RIGHT);

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

void Actor::GetTileCollisionInfo(Edge & rowEdge, Edge & colEdge, int & rowPenetration, int & colPenetration, 
	std::vector<std::shared_ptr<Tile>>& rowIntersect, std::vector<std::shared_ptr<Tile>>& colIntersect, std::shared_ptr<Level>& level)
{
	int tileWidth = level->GetTileWidth();
	int tileHeight = level->GetTileHeight();

	// Calculate the actor's bounds
	// Note that this should use the AABB, but it's reporting incorrect positions currently
	double rightBound = ceil(_position.GetX() + _sprite->GetFrameWidth());
	double leftBound = floor(_position.GetX());
	double topBound = floor(_position.GetY());
	double bottomBound = ceil(_position.GetY() + _sprite->GetFrameHeight());

	// Determine which tiles we intersect
	// If the bottom or right is exactly flush, we subtract one so that we don't test against tiles in the next row/column
	int topRow = (int)floor(topBound / tileHeight);
	int bottomRow = (int)floor(bottomBound / tileHeight) - ((int)bottomBound % tileHeight == 0 ? 1 : 0);
	int leftCol = (int)floor(leftBound / tileWidth);
	int rightCol = (int)floor(rightBound / tileWidth) - ((int)rightBound % tileWidth == 0 ? 1 : 0);

	double xVel = _speed.GetX();
	if (xVel)
	{
		colEdge = xVel > 0 ? Edge::RIGHT : Edge::LEFT;

		// If we're moving right, we need to test x + width; if we're moving left, we need to test x
		int testSideX = xVel > 0 ? (int)ceil(rightBound) : (int)floor(leftBound);

		// Determine by how much we're intersecting (left edge must be adjusted since we want the distance from the tile's right side)
		colPenetration = testSideX % tileWidth;
		if (colEdge == Edge::LEFT) colPenetration = level->GetTileWidth() - colPenetration;

		int col = testSideX / tileWidth;
		level->GetTileRange(topRow, bottomRow + 1, col, col + 1, colIntersect);
	}
	else
	{
		colEdge = Edge::NONE;
	}

	double yVel = _speed.GetY();
	if (yVel)
	{
		rowEdge = yVel > 0 ? Edge::BOTTOM : Edge::TOP;

		// If we're moving right, we need to test x + width; if we're moving left, we need to test x
		int testSideY = yVel > 0 ? (int)ceil(bottomBound) : (int)floor(topBound);

		// Determine by how much we're intersecting (top edge must be adjusted since we want the distance from the tile's bottom side)
		rowPenetration = testSideY % tileHeight;
		if (rowEdge == Edge::TOP) rowPenetration = level->GetTileHeight() - rowPenetration;

		int row = testSideY / tileHeight;
		level->GetTileRange(row, row + 1, leftCol, rightCol + 1, rowIntersect);
	}
	else
	{
		rowEdge = Edge::NONE;
	}

	if (rowEdge != Edge::NONE && colEdge != Edge::NONE)
	{
		int pruneRow = rowIntersect[0]->GetIndices().y;
		int pruneCol = colIntersect[0]->GetIndices().x;

		// The tile common to both ranges
		std::shared_ptr<Tile> cornerTile = level->GetTileFromLevel(pruneCol, pruneRow);

		if (cornerTile->GetID() != Tile::blank)
		{
			// Try to find a tile that's not blank in one of the ranges so that we can prune the common tile from the other one
			const auto nonCornerSolidTileFinder = [cornerTile](std::shared_ptr<Tile>& tile) {return tile != cornerTile && tile->GetID() != Tile::blank; };
			if (std::find_if(rowIntersect.begin(), rowIntersect.end(), nonCornerSolidTileFinder) != rowIntersect.end())
			{
				colIntersect.erase(remove(colIntersect.begin(), colIntersect.end() - 1, cornerTile));
			}
			else if (std::find_if(colIntersect.begin(), colIntersect.end(), nonCornerSolidTileFinder) != colIntersect.end())
			{
				rowIntersect.erase(remove(rowIntersect.begin(), rowIntersect.end() - 1, cornerTile));
			}
			else
			{
				// We'll prefer the side tiles because it will probably look more realistic thanks to gravity... maybe (we can change this later for visual effects if necessary)
				if (colPenetration <= rowPenetration)
				{
					rowIntersect.erase(remove(rowIntersect.begin(), rowIntersect.end() - 1, cornerTile));
				}
				else
				{
					colIntersect.erase(remove(colIntersect.begin(), colIntersect.end() - 1, cornerTile));
				}
			}
		}
	}
}

