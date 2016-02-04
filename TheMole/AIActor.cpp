#include "AIActor.h"

AIActor::~AIActor()
{
}

void AIActor::Update(std::shared_ptr<Level> & level)
{
	Actor::Update(level);

	// While the AI is alive, do stuff.
	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	if (_health <= 0)
	{
		//std::cout << "Dead\n";
	}
	if (_health > 0)
	{
		//_aabb.UpdatePosition(*this);
		UpdatePosition();
		ScanNeighbouringTiles(level);
	}
}

void AIActor::UpdatePosition()
{
	Actor::UpdatePosition();

	_position.SetX(_position.GetX() + _speed.GetX());
	_position.SetY(_position.GetY() + _speed.GetY());
	std::cout << GetPosition().GetX() << ", " << GetPosition().GetY() << "AI1\n";
}

bool AIActor::CollisionCheck(Actor &otherAI)
{
	return _aabb.CheckCollision(otherAI.GetAABB());
}

void AIActor::ScanNeighbouringTiles(std::shared_ptr<Level> & level)
{
	int tileWidth  = level->GetTileWidth();
	int tileHeight = level->GetTileHeight();

	if ((int)_position.GetX() % tileWidth == 0 || (int)_position.GetY() % tileWidth == 0)
	{
		int xInd = _position.GetX() / tileWidth;
		int yInd = _position.GetY() / tileHeight;
		
		if (xInd + 1 < level->GetLevelSize().GetX() && _speed.GetX() > 0.0f)
		{
			if (level->GetTileFromLevel(xInd + 1, yInd)->GetID() != Tile::blank)
			{
				_speed.SetX(_speed.GetX() * -1.0f);
			}
		}
		else if (xInd >= 0 && _speed.GetX() < 0.0f)
		{
			if (level->GetTileFromLevel(xInd, yInd)->GetID() != Tile::blank)
			{
				_speed.SetX(_speed.GetX() * -1.0f);
			}
		}
		if (yInd + 1 < level->GetLevelSize().GetY())
		{
			if (level->GetTileFromLevel(xInd, yInd + 1)->GetID() == Tile::blank)
			{
				_speed.SetY(1.0f);
			}
			else if(level->GetTileFromLevel(xInd, yInd + 1)->GetID() == Tile::spike)
			{
				_health = 0;
			}
			else
			{
				_speed.SetY(0.0f);
			}
		}
	}
}

void AIActor::Draw(std::shared_ptr<Level> & level)
{
	Actor::Draw(level);
}
