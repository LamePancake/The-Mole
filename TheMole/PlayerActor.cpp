#include "PlayerActor.h"
#include "GameScreen.h"

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
	_aabb.UpdatePosition(*this);
	UpdatePosition(elapsedSecs);
	
	for (int i = 0; i < level->GetEnemySize(); ++i)
	{
		if (CollisionCheck(*level->GetEnemy(i)))
		{
			_health = 0;
		}
	}

	Edge colEdge, rowEdge;
	int colPenetration, rowPenetration;
	std::vector<std::shared_ptr<Tile>> rowIntersection, colIntersection;

	GetTileCollisionInfo(rowEdge, colEdge, rowPenetration, colPenetration, rowIntersection, colIntersection, level);

	if (rowEdge != Edge::NONE)
	{
		float correctedYPos = _position.GetY();
		if (rowEdge == Edge::BOTTOM) correctedYPos -= rowPenetration;
		else if (rowEdge == Edge::TOP) correctedYPos += rowPenetration;

		for (auto& tile : rowIntersection)
		{
			switch (tile->GetID())
			{
			case Tile::blank:
				break;
			case Tile::goal:
				_atGoal = true;
			default:
				_position.SetY(correctedYPos);
				break;
			}
		}
	}

	if (colEdge != Edge::NONE)
	{
		float correctedXPos = _position.GetX();
		if (colEdge == Edge::RIGHT) correctedXPos -= colPenetration;
		else if (colEdge == Edge::LEFT) correctedXPos += colPenetration;

		for (auto& tile : colIntersection)
		{
			switch (tile->GetID())
			{
			case Tile::blank:
				break;
			case Tile::goal:
				_atGoal = true;
			default:
				_position.SetX(correctedXPos);
				break;
			}
		}
	}
}

void PlayerActor::UpdatePosition(double elapsedSecs)
{
	const std::shared_ptr<GameScreen> gameScreen = std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen());
	const std::shared_ptr<Level> level = gameScreen->GetLevel();
	_position.SetX(Math::Clamp(_position.GetX() + _speed.GetX() * (float)elapsedSecs, 0, level->GetLevelSize().x * level->GetTileWidth()));
	_position.SetY(Math::Clamp(_position.GetY() + _speed.GetY() * (float)elapsedSecs, 0, level->GetLevelSize().y * level->GetTileHeight()));
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

bool PlayerActor::CollisionCheck(Actor & otherAI)
{
	return _aabb.CheckCollision(otherAI.GetAABB());
}

bool PlayerActor::IsDead()
{
	return _health == 0 ? true : false;
}

bool PlayerActor::AtGoal()
{
	return _atGoal;
}

void PlayerActor::SetJumpVelocity(float initVel)
{
	_jumpVelocity = initVel;
}

float PlayerActor::GetJumpVelocity()
{
	return _jumpVelocity;
}

void PlayerActor::SetMaximumJumpVelocity(float initVel)
{
	_maxJumpVel = initVel;
}

float PlayerActor::GetMaximumJumpVelocity()
{
	return _maxJumpVel;
}