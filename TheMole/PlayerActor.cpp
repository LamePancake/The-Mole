#include "PlayerActor.h"
#include "GameScreen.h"

using std::vector;
using std::shared_ptr;

PlayerActor::PlayerActor(Vector2 position, GameManager& manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
	const std::string&& startSprite, SpriteSheet::XAxisDirection startXDir, SpriteSheet::YAxisDirection startYDir)
	: Actor(position, manager, spd, sprites, std::move(startSprite), startXDir, startYDir), _prevDirection(startXDir), _atGoal(false), _jumpVelocity(0), _maxJumpVel(400),
	_digDir{ Edge::NONE }, _jumped(false), _jumpDuration(0.75), _jumpTimeElapsed(0), _godMode(false), _stoppedTime(false), _selected(0)
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
	UpdateInput();
	if (_stoppedTime) return;

	Actor::Update(elapsedSecs);
	_jumpVelocity = Math::Clamp(_jumpVelocity, -_maxJumpVel, _maxJumpVel);
	if (!_jumped)
	{
		_jumpVelocity += -9.8 * 64 * elapsedSecs * -1.0f;//-9.8 * 64 * elapsedSecs * -1.0f;
	}
	if (_jumpVelocity <= _maxJumpVel)
	{
		StopJumping();
	}


	// Check whether we're finished digging and update sprites accordingly
	if (!_sprites[_currentSpriteSheet]->IsAnimating() && _digDir != Edge::NONE)
	{
		_sprites[_currentSpriteSheet]->Stop();
		_currentSpriteSheet = "idle";
		SetActorYDirection(SpriteSheet::YAxisDirection::UP);
		_sprites[_currentSpriteSheet]->Start();

        _digDir = Edge::NONE;
	}

	UpdatePosition(elapsedSecs);
	_aabb.UpdatePosition(*this);

	const std::shared_ptr<GameScreen> screen = std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen());
	for (int i = 0; i < screen->GetLevel()->GetEnemySize(); ++i)
	{
		if (CollisionCheck(*screen->GetLevel()->GetEnemy(i)))
		{
			_health -= 100;
			std::cout << "You got hit!" << std::endl;
		}
		if (_health == 0)
		{
			std::cout << "You lose." << std::endl;
		}
	}
    DetectTileCollisions(_collisionInfo, _gameScreen->GetLevel());
    Dig();
	UpdateCollisions(elapsedSecs);
    _prevKinematic = _curKinematic;
    _collisionInfo.colIntersect.clear();
    _collisionInfo.rowIntersect.clear();
}

void PlayerActor::UpdateCollisions(double elapsedSecs)
{
	std::shared_ptr<Level> level = _gameScreen->GetLevel();

	if (_collisionInfo.rowEdge != Edge::NONE)
	{
		float correctedYPos = _curKinematic.position.GetY();
		if (_collisionInfo.rowEdge == Edge::BOTTOM) correctedYPos -= _collisionInfo.rowPenetration;
		else if (_collisionInfo.rowEdge == Edge::TOP) correctedYPos += _collisionInfo.rowPenetration;
		DefaultTileCollisionHandler(_collisionInfo.rowIntersect, _collisionInfo.rowEdge, correctedYPos);
	}

	if (_collisionInfo.colEdge != Edge::NONE)
	{
		float correctedXPos = _curKinematic.position.GetX();
		if (_collisionInfo.colEdge == Edge::RIGHT) correctedXPos -= _collisionInfo.colPenetration;
		else if (_collisionInfo.colEdge == Edge::LEFT) correctedXPos += _collisionInfo.colPenetration;
		DefaultTileCollisionHandler(_collisionInfo.colIntersect, _collisionInfo.colEdge, correctedXPos);
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
	// Did you think I was done hacking? You were wrong
	// Still confusing af with naming here... If we're processing columns of tiles, i.e. things that would be collided with by travelling along the x axis, then we're looking at the x direction
	bool isXDirection = edge == Edge::RIGHT || edge == Edge::LEFT;

	// Assume we weren't on the ground until proven otherwise, but only do this when we're looking at tiles above/below us
	if (!isXDirection) _wasOnGround = false;

    // If the tile we're intersecting with or a very close one directly below it is solid, then we were on the ground
    if (edge == Edge::BOTTOM)
    {
        if (correctedPos == _curKinematic.position.GetY())
        {
            SDL2pp::Point indices = tiles[0]->GetIndices();
            if (indices.y < _gameScreen->GetLevel()->GetLevelSize().y - 1)
            {
                std::shared_ptr<Tile> below = _gameScreen->GetLevel()->GetTileFromLevel(indices.x, indices.y + 1);
                float bottomBound = _curKinematic.position.GetY() + _sprites[_currentSpriteSheet]->GetFrameHeight();
                _wasOnGround = below->GetID() != Tile::blank && (below->GetWorldPosition().GetY() - bottomBound < 3);
            }
        }
        else
        {
            _wasOnGround = true;
        }
    }

	if (edge != Edge::NONE)
	{
		for (auto& tile : tiles)
		{
			switch (tile->GetID())
			{
			case Tile::blank:
				break;
			case Tile::goal:
				_atGoal = true;
				break;
			case Tile::spike:
				_health = 0;
				break;
			default:
				if (isXDirection) _curKinematic.position.SetX(correctedPos);
				else
				{
					_curKinematic.position.SetY(correctedPos); 
					if (_jumped) StopJumping();
				}
				break;
			}
		}
	}
}

void PlayerActor::UpdateInput()
{
	if (_digDir != Edge::NONE) return;

	_stoppedTime = false;
	if (_mgr->inputManager->ActionOccurred("MIND", Input::Held))
	{
		_stoppedTime = true;
        UpdateMindControlSelection(false);
		return;
	}
	else if (_mgr->inputManager->ActionOccurred("MIND", Input::Released))
	{
        UpdateMindControlSelection(true);
	}

	bool triedDigging = _mgr->inputManager->ActionOccurred("DIG", Input::Pressed);

	if (_mgr->inputManager->ActionOccurred("LEFT", Input::Held))
	{
		SetActorXDirection(SpriteSheet::XAxisDirection::LEFT);
		SetSpeed(Vector2(Math::Clamp(_curKinematic.velocity.GetX() - 50.0f, -300.0f, -50.0f), _curKinematic.velocity.GetY()));

		if (triedDigging)
		{
			_digDir = Edge::LEFT;
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
		SetSpeed(Vector2(Math::Clamp(_curKinematic.velocity.GetX() + 50.0f, 50.0f, 300.0f), _curKinematic.velocity.GetY()));

		if (triedDigging)
		{
			_digDir = Edge::RIGHT;
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
		SetSpeed(Vector2(0.0f, _curKinematic.velocity.GetY()));
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
			_digDir = Edge::TOP;
			SetActorYDirection(SpriteSheet::YAxisDirection::UP);
			_sprites[_currentSpriteSheet]->Stop();
			_currentSpriteSheet = "verticalDig";
			_sprites[_currentSpriteSheet]->Start();
		}
		if (_godMode)
		{
			SetSpeed(Vector2(_curKinematic.velocity.GetX(), Math::Clamp(_curKinematic.velocity.GetY() - 50.0f, -50.0f, -300.0f)));
		}
	}
	else if (_mgr->inputManager->ActionOccurred("DOWN", Input::Held))
	{
		if (triedDigging)
		{
			_digDir = Edge::BOTTOM;
			SetActorYDirection(SpriteSheet::YAxisDirection::DOWN);
			_sprites[_currentSpriteSheet]->Stop();
			_currentSpriteSheet = "verticalDig";
			_sprites[_currentSpriteSheet]->Start();
		}
		if (_godMode)
		{
			SetSpeed(Vector2(_curKinematic.velocity.GetX(), Math::Clamp(_curKinematic.velocity.GetY() + 50.0f, 50.0f, 300.0f)));
		}
	}
	else if (_mgr->inputManager->ActionOccurred("JUMP", Input::Pressed))
	{
        if(_wasOnGround)
        {
		    // jump 8 tiles tall of 1 metre each, at 64 pixels per metre, multiplied by -1 because positive moves down in our world
	    	_jumped = true;
		    SetJumpVelocity(7.5f * 1.0f * 64.0f * -1.0f);
		    //SetMaximumJumpVelocity(3.0f * 1.0f * 64.0f * -1.0f);
		}
	}
	else if(_mgr->inputManager->ActionOccurred("GODMODE", Input::Pressed))
	{
		_godMode = !_godMode;
		_health = 40000000;
	}

	if (!_godMode)
	{
		//COMMENT OUT THIS LINE TO DISABLE JUMP
		SetSpeed(Vector2(_curKinematic.velocity.GetX(), GetJumpVelocity()));
	}
}

void PlayerActor::UpdateMindControlSelection(bool released)
{
	vector<shared_ptr<AIActor>> enemies = _gameScreen->GetLevel()->GetEnemies();
	vector<shared_ptr<AIActor>> inRange;
	Vector2 centre{ _curKinematic.position.GetX() + _sprites[_currentSpriteSheet]->GetFrameWidth() / 2, _curKinematic.position.GetY() + _sprites[_currentSpriteSheet]->GetFrameHeight() / 2 };
	for (auto enemy : enemies)
	{
		// Check whether the enemy is in range, offsetting their position as necessary 
		// to make sure that the distance is the same on all sides
		Vector2 enemyPos = enemy->GetPosition();
		AABB enemyAABB = enemy->GetAABB();
		if (enemyPos.GetX() < _curKinematic.position.GetX()) enemyPos.SetX(enemyPos.GetX() + enemyAABB.GetWidth());
		if (enemyPos.GetY() < _curKinematic.position.GetY()) enemyPos.SetY(enemyPos.GetY() + enemyAABB.GetHeight());

		if (centre.Distance(enemyPos) <= _mindControlRadius) inRange.push_back(enemy);
	}

	if (inRange.empty()) return;

	if (released)
	{
		for (auto enemy : inRange)
		{
			enemy->SetIsMindControlCandidate(false);
            enemy->SetSelectedForControl(false);
		}
        _selected = 0;
		return;
	}

    for (auto enemy : inRange) enemy->SetIsMindControlCandidate(true);
    inRange[_selected]->SetSelectedForControl(true);

	if (_mgr->inputManager->ActionOccurred("MIND_TOGGLE", Input::Pressed))
	{
        inRange[_selected]->SetSelectedForControl(false);
		_selected++;
		_selected %= inRange.size();
        inRange[_selected]->SetSelectedForControl(true);
	}
    if (_mgr->inputManager->ActionOccurred("LEFT", Input::Pressed) ||
        _mgr->inputManager->ActionOccurred("RIGHT", Input::Pressed))
    {
        bool controlled;
        SpriteSheet::XAxisDirection curDir = _mgr->inputManager->ActionOccurred("LEFT", Input::Pressed)
                                                ? SpriteSheet::XAxisDirection::LEFT
                                                : SpriteSheet::XAxisDirection::RIGHT;
        SpriteSheet::XAxisDirection dir;
        inRange[_selected]->GetMindControlProperties(controlled, dir);
        if (controlled && dir == curDir)
        {
            inRange[_selected]->SetMindControlProperties(false, dir);
        }
        else
        {
            inRange[_selected]->SetMindControlProperties(true, curDir);
        }
    }
}

void PlayerActor::Dig()
{
    std::shared_ptr<Level> level = _gameScreen->GetLevel();
    bool dug = false;

    // We check the tile we're intersecting and the tile directly next to it (provided it's close enough)
    switch (_digDir)
    {
    case Edge::LEFT:
    {
        for (auto tile : _collisionInfo.colIntersect)
        {
            if (tile->GetID() == Tile::dirt)
            {
                dug = true;
				level->AddDugTile(tile);
                tile->SetID(Tile::blank);
            }
        }
        if (!dug && _collisionInfo.colIntersect[0]->GetIndices().x > 0)
        {
            int nextRow = _collisionInfo.colIntersect[0]->GetIndices().y;
            int nextCol = _collisionInfo.colIntersect[0]->GetIndices().x - 1;
            float dist = _curKinematic.position.GetX() - (level->GetTileFromLevel(nextCol, nextRow)->GetWorldPosition().GetX() + level->GetTileWidth());
            if (dist < 3)
            {
                for (auto tile : _collisionInfo.colIntersect)
                {
					std::shared_ptr<Tile> neighbour = level->GetTileFromLevel(nextCol, tile->GetIndices().y);
					if (tile->GetID() == Tile::dirt)
					{
						level->AddDugTile(tile);
						tile->SetID(Tile::blank);
					}
                }
            }
        }
    }
    case Edge::RIGHT:
    {
        for (auto tile : _collisionInfo.colIntersect)
        {
            if (tile->GetID() == Tile::dirt)
            {
                dug = true;
				level->AddDugTile(tile);
                tile->SetID(Tile::blank);
            }
        }
        if (!dug && _collisionInfo.colIntersect[0]->GetIndices().x < level->GetLevelSize().x - 1)
        {
            int nextRow = _collisionInfo.colIntersect[0]->GetIndices().y;
            int nextCol = _collisionInfo.colIntersect[0]->GetIndices().x + 1;
            float dist = (level->GetTileFromLevel(nextCol, nextRow)->GetWorldPosition().GetX() + level->GetTileWidth()) - _curKinematic.position.GetX();
            if (dist < 3)
            {
                for (auto tile : _collisionInfo.colIntersect)
                {
					std::shared_ptr<Tile> neighbour = level->GetTileFromLevel(nextCol, tile->GetIndices().y);
					if (tile->GetID() == Tile::dirt)
					{
						level->AddDugTile(tile);
						tile->SetID(Tile::blank);
					}
                }
            }
        }
    }
    case Edge::TOP:
    {
        for (auto tile : _collisionInfo.rowIntersect)
        {
            if (tile->GetID() == Tile::dirt)
            {
                dug = true;
				level->AddDugTile(tile);
                tile->SetID(Tile::blank);
            }
        }
        if (!dug && _collisionInfo.rowIntersect[0]->GetIndices().y > 0)
        {
            int nextRow = _collisionInfo.rowIntersect[0]->GetIndices().y - 1;
            int nextCol = _collisionInfo.rowIntersect[0]->GetIndices().x;
            float dist = _curKinematic.position.GetY() - (level->GetTileFromLevel(nextCol, nextRow)->GetWorldPosition().GetY() + level->GetTileHeight());
            if (dist < 3)
            {
                for (auto tile : _collisionInfo.rowIntersect)
                {
					std::shared_ptr<Tile> neighbour = level->GetTileFromLevel(tile->GetIndices().x, nextRow);
					if (tile->GetID() == Tile::dirt)
					{
						level->AddDugTile(tile);
						tile->SetID(Tile::blank);
					}
                }
            }
        }
    }
    case Edge::BOTTOM:
    {
        for (auto tile : _collisionInfo.rowIntersect)
        {
            if (tile->GetID() == Tile::dirt)
            {
                dug = true;
				level->AddDugTile(tile);
                tile->SetID(Tile::blank);
            }
        }
        if (!dug && _collisionInfo.rowIntersect[0]->GetIndices().y < level->GetLevelSize().y - 1)
        {
            int nextRow = _collisionInfo.rowIntersect[0]->GetIndices().y + 1;
            int nextCol = _collisionInfo.rowIntersect[0]->GetIndices().x;
            float dist = level->GetTileFromLevel(nextCol, nextRow)->GetWorldPosition().GetY() - (_curKinematic.position.GetY() + _sprites[_currentSpriteSheet]->GetFrameHeight());
            if (dist < 3)
            {
                for (auto tile : _collisionInfo.rowIntersect)
                {
					std::shared_ptr<Tile> neighbour = level->GetTileFromLevel(tile->GetIndices().x, nextRow);
					if (tile->GetID() == Tile::dirt)
					{
						level->AddDugTile(tile);
						tile->SetID(Tile::blank);
					}
                }
            }
        }
    }
    }
}

void PlayerActor::UpdatePosition(double elapsedSecs)
{
	const std::shared_ptr<Level> level = _gameScreen->GetLevel();
	_curKinematic.position.SetX(Math::Clamp((float)_curKinematic.position.GetX() + _curKinematic.velocity.GetX() * (float)elapsedSecs, 
                                            0, 
                                            level->GetLevelSize().x * level->GetTileWidth() - _sprites[_currentSpriteSheet]->GetFrameWidth()));
	_curKinematic.position.SetY(Math::Clamp((float)_curKinematic.position.GetY() + _curKinematic.velocity.GetY() * (float)elapsedSecs, 
                                            0,
                                            level->GetLevelSize().y * level->GetTileHeight() - _sprites[_currentSpriteSheet]->GetFrameHeight()));
}

bool PlayerActor::StoppedTime() const
{
	return _stoppedTime;
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