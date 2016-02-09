#include "PlayerActor.h"
#include "GameScreen.h"

PlayerActor::PlayerActor(Vector2 position, GameManager& manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
	const std::string&& startSprite, SpriteSheet::XAxisDirection startXDir, SpriteSheet::YAxisDirection startYDir)
	: Actor(position, manager, spd, sprites, std::move(startSprite), startXDir, startYDir), _prevDirection(startXDir), _atGoal(false), _jumpVelocity(0), _maxJumpVel(400),
	_digDir{' ' , ' '}, _jumped(false), _isDigging(false), _jumpDuration(0.75), _jumpTimeElapsed(0)
{
}

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
	if (!_jumped)
	{
		_jumpVelocity += -9.8 * 64 * elapsedSecs * -1.0f;
	}
	if (_jumpVelocity <= _maxJumpVel)
	{
		StopJumping();
	}

	// Check whether we're finished digging and update sprites accordingly
	if (!_sprites[_currentSpriteSheet]->IsAnimating() && _isDigging)
	{
		_currentSpriteSheet = "idle";
		SetActorYDirection(SpriteSheet::YAxisDirection::UP);
		_sprites[_currentSpriteSheet]->Start();

		_isDigging = false;
	}

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

	UpdateCollisions(elapsedSecs);
}

void PlayerActor::UpdateCollisions(double elapsedSecs)
{
	std::shared_ptr<Level> level = _gameScreen->GetLevel();

	Edge colEdge, rowEdge;
	int colPenetration, rowPenetration;
	std::vector<std::shared_ptr<Tile>> rowIntersection, colIntersection;

	DetectTileCollisions(rowEdge, colEdge, rowPenetration, colPenetration, rowIntersection, colIntersection, level);

	if (rowEdge != Edge::NONE)
	{
		float correctedYPos = _position.GetY();
		if (rowEdge == Edge::BOTTOM) correctedYPos -= rowPenetration;
		else if (rowEdge == Edge::TOP) correctedYPos += rowPenetration;
		DefaultTileCollisionHandler(rowIntersection, rowEdge, correctedYPos);
	}

	if (colEdge != Edge::NONE)
	{
		float correctedXPos = _position.GetX();
		if (colEdge == Edge::RIGHT) correctedXPos -= colPenetration;
		else if (colEdge == Edge::LEFT) correctedXPos += colPenetration;
		DefaultTileCollisionHandler(colIntersection, colEdge, correctedXPos);
	}

	if (_atGoal)
		std::cout << "You Win." << std::endl;
}

void PlayerActor::StopJumping()
{
	_jumped = false;
	//_speed.SetY(-_maxJumpVel);
	_jumpTimeElapsed = 0;
}

void PlayerActor::DefaultTileCollisionHandler(std::vector<std::shared_ptr<Tile>>& tiles, Edge edge, float correctedPos)
{
	bool canDig = (_spriteYDir == SpriteSheet::YAxisDirection::UP && edge == Edge::TOP) ||
		(_spriteYDir == SpriteSheet::YAxisDirection::DOWN && edge == Edge::BOTTOM) ||
		(_spriteYDir == SpriteSheet::XAxisDirection::RIGHT && edge == Edge::RIGHT) ||
		(_spriteYDir == SpriteSheet::XAxisDirection::LEFT && edge == Edge::LEFT);
	
	canDig = canDig && _isDigging;

	// Did you think I was done hacking? You were wrong
	// Still confusing af with naming here... If we're processing columns of tiles, i.e. things that would be collided with by travelling along the x axis, then we're looking at the x direction
	bool isXDirection = edge == Edge::RIGHT || edge == Edge::LEFT;

	// Assume we weren't on the ground until proven otherwise, but only do this when we're looking at tiles above/below us
	if (!isXDirection) _wasOnGround = false;

	if (edge != Edge::NONE)
	{
		for (auto& tile : tiles)
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
				else
				{
					if(isXDirection) _position.SetX(correctedPos);
					else
					{
						_position.SetY(correctedPos);
						if (edge == Edge::BOTTOM) _wasOnGround = true;
						if (_jumped) StopJumping();
					}
				}
				break;
			case Tile::goal:
				_atGoal = true;
				break;
			case Tile::spike:
				_health = 0;
				break;
			default:
				if (isXDirection) _position.SetX(correctedPos);
				else
				{
					_position.SetY(correctedPos); 
					if (edge == Edge::BOTTOM) _wasOnGround = true;
					if (_jumped) StopJumping();
				}
				break;
			}
		}
	}
}

void PlayerActor::UpdateInput()
{
	if (_isDigging) return;

	bool triedDigging = _mgr->inputManager->ActionOccurred("DIG", Input::Held);

	if (_mgr->inputManager->ActionOccurred("LEFT", Input::Held))
	{
		SetActorXDirection(SpriteSheet::XAxisDirection::LEFT);
		SetSpeed(Vector2(Math::Clamp(_speed.GetX() - 50.0f, -300.0f, -50.0f), _speed.GetY()));

		if (triedDigging)
		{
			_isDigging = true;
			_sprites[_currentSpriteSheet]->Stop();
			_currentSpriteSheet = "sideDig";
			_sprites[_currentSpriteSheet]->Start();
		}
		else if (_currentSpriteSheet != "walk")
		{
			_sprites[_currentSpriteSheet]->Stop();
			_currentSpriteSheet = "walk";
			_sprites[_currentSpriteSheet]->Start();
		}
	}
	else if (_mgr->inputManager->ActionOccurred("RIGHT", Input::Held))
	{
		SetActorXDirection(SpriteSheet::XAxisDirection::RIGHT);
		SetSpeed(Vector2(Math::Clamp(_speed.GetX() + 50.0f, 50.0f, 300.0f), _speed.GetY()));

		if (triedDigging)
		{
			_isDigging = true;
			_sprites[_currentSpriteSheet]->Stop();
			_currentSpriteSheet = "sideDig";
			_sprites[_currentSpriteSheet]->Start();
		}
		else if (_currentSpriteSheet != "walk")
		{
			_sprites[_currentSpriteSheet]->Stop();
			_currentSpriteSheet = "walk";
			_sprites[_currentSpriteSheet]->Start();
		}
	}
	else
	{
		// If we're not trying to move in a given direction, stop all motion on the x axis and use the idle animation
		SetSpeed(Vector2(0.0f, _speed.GetY()));
		if (_currentSpriteSheet != "idle")
		{
			_sprites[_currentSpriteSheet]->Stop();
			_currentSpriteSheet = "idle";
			_sprites[_currentSpriteSheet]->Start();
		}
	}

	if (_mgr->inputManager->ActionOccurred("UP", Input::Held))
	{
		if (triedDigging)
		{
			_isDigging = true;
			SetActorYDirection(SpriteSheet::YAxisDirection::UP);
			_sprites[_currentSpriteSheet]->Stop();
			_currentSpriteSheet = "verticalDig";
			_sprites[_currentSpriteSheet]->Start();
		}
	}
	else if (_mgr->inputManager->ActionOccurred("DOWN", Input::Held))
	{
		if (triedDigging)
		{
			_isDigging = true;
			SetActorYDirection(SpriteSheet::YAxisDirection::DOWN);
			_sprites[_currentSpriteSheet]->Stop();
			_currentSpriteSheet = "verticalDig";
			_sprites[_currentSpriteSheet]->Start();
		}
	}
	else if (_mgr->inputManager->ActionOccurred("JUMP", Input::Pressed) && _wasOnGround)
	{
		// jump 8 tiles tall of 1 metre each, at 64 pixels per metre, multiplied by -1 because positive moves down in our world
		_jumped = true;
		SetJumpVelocity(8.0f * 1.0f * 64.0f * -1.0f);
		SetMaximumJumpVelocity(8.0f * 1.0f * 64.0f * -1.0f);
	}
	else
	{
		// reset jump velocity
		//SetJumpVelocity(0.0f);
		//_speed.SetY(0);
	}

	//COMMENT OUT THIS LINE TO DISABLE JUMP
	SetSpeed(Vector2(_speed.GetX(), GetJumpVelocity()));
}

void PlayerActor::UpdatePosition(double elapsedSecs)
{
	const std::shared_ptr<Level> level = _gameScreen->GetLevel();
	_position.SetX(Math::Clamp(_position.GetX() + _speed.GetX() * (float)elapsedSecs, 0, level->GetLevelSize().x * level->GetTileWidth() - _sprites[_currentSpriteSheet]->GetFrameWidth()));
	_position.SetY(Math::Clamp(_position.GetY() + _speed.GetY() * (float)elapsedSecs, 0, level->GetLevelSize().y * level->GetTileHeight() - _sprites[_currentSpriteSheet]->GetFrameHeight()));
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