#include "PlayerActor.h"

PlayerActor::~PlayerActor()
{
}

void PlayerActor::Draw(Camera& camera)
{
	Actor::Draw(camera);
}

void PlayerActor::Update(double elapsedSecs, std::shared_ptr<Level>& level)
{
	Actor::Update(elapsedSecs, level);
	UpdatePosition(elapsedSecs);
}

void PlayerActor::UpdatePosition(double elapsedSecs)
{
	Actor::UpdatePosition(elapsedSecs);
	_position.SetX(_position.GetX() + _speed.GetX() * (float)elapsedSecs);
	_position.SetY(_position.GetY() + _speed.GetY() * (float)elapsedSecs);
}

bool PlayerActor::Dig(char dir, std::shared_ptr<Level>& level)
{
	int levelSizeX = level->GetLevelSize().x - 1;
	int levelSizeY = level->GetLevelSize().y - 1;

	int xInd = Math::Clamp((int)(GetPosition().GetX() / level->GetTileWidth()), 0, levelSizeX);
	int yInd = Math::Clamp((int)(GetPosition().GetY() / level->GetTileHeight()), 0, levelSizeY);

	switch (toupper(dir))
	{
		case 'L':
		{
			std::shared_ptr<Tile> tile = level->GetTileFromLevel(Math::Clamp(xInd - 1, 0, levelSizeX), yInd);
			if (tile->GetID() == Tile::dirt)
			{
				tile->SetID(Tile::blank);
				return true;
			}
			return false;
		}
		case 'R':
		{
			std::shared_ptr<Tile> tile = level->GetTileFromLevel(Math::Clamp(xInd + 1, 0, levelSizeX), yInd);
			if (tile->GetID() == Tile::dirt)
			{
				tile->SetID(Tile::blank);
				return true;
			}
			return false;
		}
		case 'U':
		{
			std::shared_ptr<Tile> tile = level->GetTileFromLevel(xInd, Math::Clamp(yInd - 1, 0, levelSizeY));
			if (tile->GetID() == Tile::dirt)
			{
				tile->SetID(Tile::blank);
				return true;
			}
			return false;
		}
		case 'D':
		{
			std::shared_ptr<Tile> tile = level->GetTileFromLevel(xInd, Math::Clamp(yInd + 1, 0, levelSizeY));
			if (tile->GetID() == Tile::dirt)
			{
				tile->SetID(Tile::blank);
				return false;
			}
			return false;
		}
		default:
		{
			return false;
		}
	}
}
