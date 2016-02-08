#include "PlayerActor.h"
#include "GameScreen.h"

PlayerActor::PlayerActor(Vector2 position, GameManager& manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
	const std::string&& startSprite, SpriteSheet::XAxisDirection startXDir, SpriteSheet::YAxisDirection startYDir)
	: Actor(position, manager, spd, sprites, std::move(startSprite), startXDir, startYDir), _prevDirection(startXDir), _atGoal(false), _jumpVelocity(0), _maxJumpVel(400),
	_digDir{' ' , ' '}, _jumped(false), _isDigging(false)
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

	// Check whether we're finished digging and update sprites accordingly
	if (!_sprites[_currentSpriteSheet]->IsAnimating() && _isDigging)
	{
		_currentSpriteSheet = "idle";
		SetActorXDirection(_prevDirection);
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

void PlayerActor::DefaultTileCollisionHandler(std::vector<std::shared_ptr<Tile>>& tiles, Edge edge, float correctedPos)
{
	bool canDig = (_digDir[1] == 'U' && edge == Edge::TOP) ||
		(_digDir[1] == 'D' && edge == Edge::BOTTOM) ||
		(_digDir[0] == 'R' && edge == Edge::RIGHT) ||
		(_digDir[0] == 'L' && edge == Edge::LEFT);

	// Did you think I was done hacking? You were wrong
	// Still confusing af with naming here... If we're processing columns of tiles, i.e. things that would be collided with by travelling along the x axis, then we're looking at the x direction
	bool isXDirection = edge == Edge::RIGHT || edge == Edge::LEFT;

	// Set what *could* be our new direction in a given axis (keeping in mind that if the a given _digDir is ' ', we're probably not processing that part anyway... I hope)
	SpriteSheet::XAxisDirection digXDirection = _digDir[0] == 'R' ? SpriteSheet::XAxisDirection::RIGHT : SpriteSheet::XAxisDirection::LEFT;
	SpriteSheet::YAxisDirection digYDirection = _digDir[1] == 'U' ? SpriteSheet::YAxisDirection::UP : SpriteSheet::YAxisDirection::DOWN;
	std::string digSheet = isXDirection ? "sideDig" : "verticalDig";

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
					_isDigging = true;

					// If we haven't started the dig animation, do that now
					if (_currentSpriteSheet != digSheet)
					{
						SetSpeed(Vector2(0.0f, 0.0f));
						_sprites[_currentSpriteSheet]->Stop();
						_currentSpriteSheet = digSheet;
						_sprites[_currentSpriteSheet]->Start();

						if (isXDirection) SetActorXDirection(digXDirection);
						else SetActorYDirection(digYDirection);
					}
				}
				else
				{
					if(isXDirection) _position.SetX(correctedPos);
					else _position.SetY(correctedPos);
					_jumpVelocity = 0.0f;
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
				else _position.SetY(correctedPos);
				_jumpVelocity = 0.0f;
				break;
			}
		}
	}
}

void PlayerActor::UpdateInput()
{
	if (_isDigging) return;

	_digDir[0] = ' ';
	_digDir[1] = ' ';

	if (_mgr->inputManager->ActionOccurred("LEFT", Input::Held))
	{
		_digDir[0] = 'L';
		SetSpeed(Vector2(Math::Clamp(_speed.GetX() - 400.0f, -400.0f, 0.0f), _speed.GetY()));
		SetActorXDirection(SpriteSheet::XAxisDirection::LEFT);

		if (_currentSpriteSheet != "walk")
		{
			_sprites[_currentSpriteSheet]->Stop();
			_currentSpriteSheet = "walk";
			_sprites[_currentSpriteSheet]->Start();
		}
	}
	else if (_mgr->inputManager->ActionOccurred("RIGHT", Input::Held))
	{
		_digDir[0] = 'R';
		
		SetSpeed(Vector2(Math::Clamp(_speed.GetX() + 400.0f, 400.0f, 400.0f), _speed.GetY()));
		SetActorXDirection(SpriteSheet::XAxisDirection::RIGHT);

		if (_currentSpriteSheet != "walk")
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
		// jump 12 tiles tall of 1 metre each, at 64 pixels per metre, multiplied by -1 because positive moves down in our world
		SetJumpVelocity(12.0f * 1.0f * 64.0f * -1.0f);
		SetMaximumJumpVelocity(12.0f * 1.0f * 64.0f * -1.0f);
	}
	else
	{
		// reset jump velocity
		//SetJumpVelocity(0.0f);
		//_speed.SetY(0);
	}

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