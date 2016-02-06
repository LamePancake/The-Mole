#include "PlayerActor.h"
#include "GameScreen.h"

PlayerActor::~PlayerActor()
{
}

void PlayerActor::Draw(Camera& camera)
{
	Actor::Draw(camera);
}

void PlayerActor::Update(double elapsedSecs)
{
	Actor::Update(elapsedSecs);
	UpdateInput();
	UpdatePosition(elapsedSecs);
	_aabb.UpdatePosition(*this);

	const std::shared_ptr<GameScreen> screen = std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen());
	for (int i = 0; i < screen->GetLevel()->GetEnemySize(); ++i)
	{
		if (CollisionCheck(*screen->GetLevel()->GetEnemy(i)))
		{
			_health = 0;
			std::cout << "You lose." << std::endl;
		}
	}

	UpdateCollisions();
}

void PlayerActor::UpdateCollisions()
{
	std::shared_ptr<Level> level = _gameScreen->GetLevel();

	Edge colEdge, rowEdge;
	int colPenetration, rowPenetration;
	std::vector<std::shared_ptr<Tile>> rowIntersection, colIntersection;

	GetTileCollisionInfo(rowEdge, colEdge, rowPenetration, colPenetration, rowIntersection, colIntersection, level);


	if (rowEdge != Edge::NONE)
	{
		bool canDig = (_digDir[1] == 'U' && rowEdge == Edge::TOP) || (_digDir[1] == 'D' && rowEdge == Edge::BOTTOM);
		float correctedYPos = _position.GetY();
		if (rowEdge == Edge::BOTTOM) correctedYPos -= rowPenetration;
		else if (rowEdge == Edge::TOP) correctedYPos += rowPenetration;

		for (auto& tile : rowIntersection)
		{
			switch (tile->GetID())
			{
			case Tile::blank:
				break;
			case Tile::dirt:
				if (canDig)
				{
					tile->SetID(Tile::blank);
				}
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
		bool canDig = (_digDir[0] == 'R' && colEdge == Edge::RIGHT) ||	(_digDir[0] == 'L' && colEdge == Edge::LEFT);
		float correctedXPos = _position.GetX();
		if (colEdge == Edge::RIGHT) correctedXPos -= colPenetration;
		else if (colEdge == Edge::LEFT) correctedXPos += colPenetration;

		for (auto& tile : colIntersection)
		{
			switch (tile->GetID())
			{
			case Tile::blank:
				break;
			case Tile::dirt:
				if (canDig)
				{
					tile->SetID(Tile::blank);
				}
				break;
			case Tile::goal:
				_atGoal = true;
			default:
				_position.SetX(correctedXPos);
				break;
			}
		}
	}

	if (_atGoal)
		std::cout << "You Win." << std::endl;
}

void PlayerActor::UpdateInput()
{
	_digDir[0] = ' ';
	_digDir[1] = ' ';
	if (_mgr->inputManager->ActionOccurred("LEFT", Input::Held))
	{
		_digDir[0] = 'L';
		SetSpeed(Vector2(Math::Clamp(_speed.GetX() - 400.0f, -400.0f, 0.0f), _speed.GetY()));
		SetActorDirection(SpriteSheet::XAxisDirection::LEFT);
	}
	else if (_mgr->inputManager->ActionOccurred("RIGHT", Input::Held))
	{
		_digDir[0] = 'R';
		
		SetSpeed(Vector2(Math::Clamp(_speed.GetX() + 400.0f, 400.0f, 400.0f), _speed.GetY()));
		SetActorDirection(SpriteSheet::XAxisDirection::RIGHT);
	
	}
	else
	{
		SetSpeed(Vector2(0.0f, _speed.GetY()));
	}

	if (_mgr->inputManager->ActionOccurred("UP", Input::Held))
	{
		_digDir[1] = 'U';
		SetSpeed(Vector2(_speed.GetX(), Math::Clamp(_speed.GetY() - 400.0f, 0.0f, -400.0f)));
	}
	else if (_mgr->inputManager->ActionOccurred("DOWN", Input::Held))
	{
		_digDir[1] = 'D';
		SetSpeed(Vector2(_speed.GetX(), Math::Clamp(_speed.GetY() + 400.0f, 0.0f, 400.0f)));
	}
	else if (_mgr->inputManager->ActionOccurred("JUMP", Input::Pressed))
	{
		SetJumpVelocity(800.0f);
		SetMaximumJumpVelocity(800.0f);
	}
	else
	{
		_speed.SetY(0);
	}

}

void PlayerActor::UpdatePosition(double elapsedSecs)
{
	const std::shared_ptr<GameScreen> gameScreen = std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen());
	const std::shared_ptr<Level> level = gameScreen->GetLevel();
	_position.SetX(Math::Clamp(_position.GetX() + _speed.GetX() * (float)elapsedSecs, 0, level->GetLevelSize().x * level->GetTileWidth()));
	_position.SetY(Math::Clamp(_position.GetY() + _speed.GetY() * (float)elapsedSecs, 0, level->GetLevelSize().y * level->GetTileHeight()));
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