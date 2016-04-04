#include "PlayerActor.h"
#include "GameScreen.h"

using std::vector;
using std::shared_ptr;

PlayerActor::PlayerActor(Vector2 position, GameManager& manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
	const std::string&& startSprite, SpriteSheet::XAxisDirection startXDir, SpriteSheet::YAxisDirection startYDir)
	: Actor(position, manager, spd, sprites, std::move(startSprite), startXDir, startYDir),
	_prevDirection(startXDir),
	_atGoal(false),
	_triggeredIntro(false),
    _jumpVelocity(0),
    _maxJumpVel(400),
	_digDir{ Edge::NONE },
    _jumped(false),
    _jumpDuration(0.75),
    _jumpTimeElapsed(0),
    _godMode(false),
    _stoppedTime(false),
    _selected(0),
    _jumpBoost(600), 
    _jumpBoosted(false)
{
	_shieldActive = false;///Start shield in inactive state
	_shieldReleased = false;
	_shieldStr = 4;
	_shieldTimer = 0;
	_lastPrj = nullptr;
}

PlayerActor::~PlayerActor()
{
}

void PlayerActor::Draw(Camera& camera)
{
	Actor::Draw(camera);
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	SDL2pp::Point dim = GameManager::GetInstance()->GetWindow().GetSize();

	for (size_t i = 0; i < _shieldStr; i++) {
		rend.FillRect(SDL2pp::Rect(dim.GetX() * (0.90f + (i * 0.02f))
			, dim.GetY()  * 0.97f
			, 10.0f
			, 10.0f));
	}

	if(_shieldActive)
		rend.FillRect(SDL2pp::Rect(dim.GetX() * (0.2f)
			, dim.GetY()  * 0.2f
			, 20.0f
			, 20.0f));
}

void PlayerActor::Update(double elapsedSecs)
{
	UpdateInput(elapsedSecs);
	if (_stoppedTime) return;

	Actor::Update(elapsedSecs);

	UpdateShieldStatus(elapsedSecs);

    // Slow gravity if we're gliding
	if (_gliding && _jumpVelocity > 0)
	{
		_jumpVelocity += -5.9 * 64 * elapsedSecs;
	}
	if (_wasOnGround)
	{
		_jumpBoosted = false;
	}

    _jumpVelocity -= -9.8 * 64 * elapsedSecs;
    _jumpVelocity = Math::Clamp(_jumpVelocity, -_maxJumpVel, _maxJumpVel);


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

	for (auto actor : _gameScreen->GetLevel()->GetActors())
	{
		if (CollisionCheck(*actor))
		{
			Type type = actor->GetType();
			switch (type)
			{
			case Type::enemy:
				if (CollisionCheck(*actor))
				{
					SetHealth(0);
				}
				break;
			case Type::door:
				shared_ptr<DoorActor> door = dynamic_pointer_cast<DoorActor>(actor);
				if (!door->IsOpen())
				{
					Edge edge = door->GetEdge();
					bool affectsY = edge == Edge::BOTTOM || edge == Edge::TOP;
					Vector2 overlap = _aabb.GetOverlap(actor->GetAABB(), true);
					// Push our hero out of the door
					if (affectsY)
					{
						_curKinematic.position.SetY(_curKinematic.position.GetY() + overlap.GetY());
						_prevKinematic.position.SetY(_curKinematic.position.GetY()); // Tile collision will screw up otherwise...
					}
					else
					{
						_curKinematic.position.SetX(_curKinematic.position.GetX() + overlap.GetX());
						_prevKinematic.position.SetX(_curKinematic.position.GetX()); // Tile collision will screw up otherwise...
					}
				}
				break;
			}
		}
	}

    DetectTileCollisions(_collisionInfo, _gameScreen->GetLevel());
	DigDiggableTiles();
	UpdateCollisions(elapsedSecs);
    _prevKinematic = _curKinematic;
    _collisionInfo.colIntersect.clear();
    _collisionInfo.rowIntersect.clear();
}

void PlayerActor::UpdateCollisions(double elapsedSecs)
{
	std::shared_ptr<Level> level = _gameScreen->GetLevel();

    _wasOnGround = false;

	if (_collisionInfo.shouldCorrectY)
	{
		float correctedYPos = _curKinematic.position.GetY();
		if (_collisionInfo.rowEdge == Edge::BOTTOM) correctedYPos -= _collisionInfo.rowPenetration;
		else if (_collisionInfo.rowEdge == Edge::TOP) correctedYPos += _collisionInfo.rowPenetration;
		DefaultTileCollisionHandler(_collisionInfo.rowIntersect, _collisionInfo.rowEdge, correctedYPos);
	}
    else
    {
        // Check the tiles immediately below the ones we're intersecting to see if we're "close enough" to be on the ground
        shared_ptr<Level> level = _gameScreen->GetLevel();
        if (!_wasOnGround &&
            _collisionInfo.rowEdge == Edge::BOTTOM &&
            level->HasNeighbourTile(_collisionInfo.rowIntersect[0], Edge::BOTTOM))
        {
            for (auto & tile : _collisionInfo.rowIntersect)
            {
                // If we collided with x and this is the corner tile, then we don't want to consider the neighbour for collision
                if (_collisionInfo.shouldCorrectX && tile == _collisionInfo.corner)
                    continue;

                // Determine how far we are from the neighbouring tile
                shared_ptr<Tile> neighbour = level->GetNeighbourTile(tile, Edge::BOTTOM);
                float bottomBound = _curKinematic.position.GetY() + _sprites[_currentSpriteSheet]->GetFrameHeight();
                float distToNeighbour = neighbour->GetWorldPosition().GetY() - bottomBound;

                if (neighbour->GetID() != Tile::blank && (distToNeighbour < 3))
                {
                    _wasOnGround = true;
                    break;
                }
            }
        }
    }

	if (_collisionInfo.shouldCorrectX)
	{
		float correctedXPos = _curKinematic.position.GetX();
		if (_collisionInfo.colEdge == Edge::RIGHT) correctedXPos -= _collisionInfo.colPenetration;
		else if (_collisionInfo.colEdge == Edge::LEFT) correctedXPos += _collisionInfo.colPenetration;
		DefaultTileCollisionHandler(_collisionInfo.colIntersect, _collisionInfo.colEdge, correctedXPos);
	}
}

void PlayerActor::StopJumping()
{
	_jumped = false;
	_jumpVelocity = 0;
	_jumpTimeElapsed = 0;
}

void PlayerActor::DefaultTileCollisionHandler(std::vector<std::shared_ptr<Tile>>& tiles, Edge edge, float correctedPos)
{
	// Did you think I was done hacking? You were wrong
	// Still confusing af with naming here... If we're processing columns of tiles, i.e. things that would be collided with by travelling along the x axis, then we're looking at the x direction
	bool isXDirection = edge == Edge::RIGHT || edge == Edge::LEFT;
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
				SetHealth(0);
				break;
			case Tile::introtrig:
				_triggeredIntro = true;
				break;
			default:
				if (isXDirection) _curKinematic.position.SetX(correctedPos);
				else
				{
					_wasOnGround = edge == Edge::BOTTOM;
					_curKinematic.position.SetY(correctedPos); 
                    if (_jumped && !_gliding)
                    {
                        StopJumping();
                    }
				}
				break;
			}
		}
	}
}

void PlayerActor::UpdateInput(double elapsedSecs)
{
	shared_ptr<Level> level = _gameScreen->GetLevel();

	if (_digDir != Edge::NONE) return;

	_stoppedTime = false;
	if (_mgr->inputManager->ActionOccurred("MIND_CONTROL", Input::Held) && level->IsHatAvailable("MIND_CONTROL"))
	{
		_stoppedTime = true;
        UpdateMindControlSelection(false);
		return;
	}
	else if (_mgr->inputManager->ActionOccurred("MIND_CONTROL", Input::Released) && level->IsHatAvailable("MIND_CONTROL"))
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

	if (_mgr->inputManager->ActionOccurred("JUMP", Input::Pressed))
	{
        if(_wasOnGround)
        {
		    // jump 7.5 tiles tall of 1 metre each, at 64 pixels per metre, multiplied by -1 because positive moves down in our world
	    	_jumped = true;
		    SetJumpVelocity(7.5f * 1.0f * 64.0f * -1.0f);
		    //SetMaximumJumpVelocity(3.0f * 1.0f * 64.0f * -1.0f);
		}
	}

	if(_mgr->inputManager->ActionOccurred("GODMODE", Input::Pressed))
	{
		_godMode = !_godMode;
		_health = 40000000;
	}

	if (!_godMode)
	{
		//COMMENT OUT THIS LINE TO DISABLE JUMP
		SetSpeed(Vector2(_curKinematic.velocity.GetX(), GetJumpVelocity()));
	}

	if (_mgr->inputManager->ActionOccurred("CHICKEN", Input::Pressed) && !_jumpBoosted  && level->IsHatAvailable("CHICKEN"))
	{
		_jumpVelocity -= _jumpBoost;
		_jumpBoosted = true;
	}
	if (_mgr->inputManager->ActionOccurred("CHICKEN", Input::Down) && level->IsHatAvailable("CHICKEN"))
	{
		_gliding = true;
	}
	else
	{
		_gliding = false;
	}

	if (_mgr->inputManager->ActionOccurred("SHIELD", Input::Pressed) && level->IsHatAvailable("SHIELD"))
	{
		///Activate shield
		_shieldActive = true;
		_shieldReleased = false;
	}
	//if (_mgr->inputManager->ActionOccurred("SHIELD", Input::Held))
	//{
	//	///Drain shield
	//	_shieldActive = true;
	//	//_shieldReleased = false;
	//}
	if (_mgr->inputManager->ActionOccurred("SHIELD", Input::Released) && level->IsHatAvailable("SHIELD"))
	{
		///Deactivate Shield
		_shieldReleased = true;
	}
}

void PlayerActor::UpdateShieldStatus(double deltaTime)
{
	if (_shieldActive)
	{
		_shieldTimer += deltaTime;

		if (_shieldTimer > 1)
		{
			if (_shieldReleased)
			{
				_shieldActive = false;
				_shieldReleased = false;
			}
			if (!_shieldHit) 
				_shieldStr--;
			_shieldTimer = 0;
		}

		if (_shieldStr <= 0)
		{
			_shieldActive = false;
			_shieldTimer = 0;
		}
	}
	else if(_shieldStr < 4)
	{
		_shieldTimer += deltaTime;
		if (_shieldTimer > 3)
		{
			_shieldStr++;
			_shieldTimer = 0;
		}
	}
	_shieldHit = false;
}

void PlayerActor::ShieldHit()
{
	_shieldStr--;
}

void PlayerActor::ProjectileHit(Actor *prj)
{
	if (_lastPrj != prj)
	{
		if (_shieldActive)
		{
			_shieldStr--;
			_shieldHit = true;
			_shieldTimer = 0;
			if (_shieldStr <= 0)
				SetHealth(0);
			if (_shieldReleased)
				_shieldActive = false;
		}
		else
		{
			SetHealth(0);
		}
		_lastPrj = prj;
	}
}

void PlayerActor::UpdateMindControlSelection(bool released)
{
	shared_ptr<Level> level = _gameScreen->GetLevel();
	vector<shared_ptr<AIActor>> inRange;
	Vector2 centre{ _curKinematic.position.GetX() + _sprites[_currentSpriteSheet]->GetFrameWidth() / 2, _curKinematic.position.GetY() + _sprites[_currentSpriteSheet]->GetFrameHeight() / 2 };
	for (auto actor : level->GetActors())
	{
		if (actor->GetType() != Type::enemy) continue;
		// Check whether the enemy is in range, offsetting their position as necessary 
		// to make sure that the distance is the same on all sides
		shared_ptr<AIActor> enemy = dynamic_pointer_cast<AIActor>(actor);
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

	// Cycle through the controllable enemies
	if (_mgr->inputManager->ActionOccurred("MIND_CONTROL_SELECT", Input::Pressed))
	{
        inRange[_selected]->SetSelectedForControl(false);
		_selected++;
		_selected %= inRange.size();
        inRange[_selected]->SetSelectedForControl(true);
	}

	// Toggle whether the enemy is under mind control
	if (_mgr->inputManager->ActionOccurred("MIND_CONTROL_STOP", Input::Pressed))
	{
		inRange[_selected]->StopMindControl();
	}
	// Give the selected enemy a new direction in which to travel (and automatically enable mind control)
    else if (_mgr->inputManager->ActionOccurred("LEFT", Input::Pressed) ||
        _mgr->inputManager->ActionOccurred("RIGHT", Input::Pressed))
    {
        SpriteSheet::XAxisDirection dir = _mgr->inputManager->ActionOccurred("LEFT", Input::Pressed)
                                                ? SpriteSheet::XAxisDirection::LEFT
                                                : SpriteSheet::XAxisDirection::RIGHT;

		bool controlled = inRange[_selected]->IsUnderMindControl();
		inRange[_selected]->SetMindControlDirection(dir);
    }
}

void PlayerActor::DigDiggableTiles()
{
	if (_digDir == Edge::NONE) return;

    std::shared_ptr<Level> level = _gameScreen->GetLevel();
    bool dug = false;

	vector<shared_ptr<Tile>>& tiles = _digDir == Edge::LEFT || _digDir == Edge::RIGHT ? _collisionInfo.colIntersect : _collisionInfo.rowIntersect;
	bool hasNeighbour = false;
	SDL2pp::Point neighbourIndices;
	float dist;

	switch (_digDir)
	{
	case Edge::LEFT:
	{
		hasNeighbour = tiles[0]->GetIndices().x > 0;
		neighbourIndices.x = tiles[0]->GetIndices().x - 1;
		neighbourIndices.y = tiles[0]->GetIndices().y;
		if (hasNeighbour)
		{
			dist = _curKinematic.position.GetX() - (level->GetTileFromLevel(neighbourIndices.x, neighbourIndices.y)->GetWorldPosition().GetX() + level->GetTileWidth());
		}
	}
	case Edge::RIGHT:
	{
		hasNeighbour = tiles[0]->GetIndices().x < level->GetLevelSize().x - 1;
		neighbourIndices.x = tiles[0]->GetIndices().x + 1;
		neighbourIndices.y = tiles[0]->GetIndices().y;
		if (hasNeighbour)
		{
			dist = (level->GetTileFromLevel(neighbourIndices.x, neighbourIndices.y)->GetWorldPosition().GetX() + level->GetTileWidth()) - _curKinematic.position.GetX();
		}
	}
	case Edge::TOP:
	{
		hasNeighbour = tiles[0]->GetIndices().y > 0;
		neighbourIndices.x = tiles[0]->GetIndices().x;
		neighbourIndices.y = tiles[0]->GetIndices().y - 1;
		if (hasNeighbour)
		{
			dist = _curKinematic.position.GetY() - (level->GetTileFromLevel(neighbourIndices.x, neighbourIndices.y)->GetWorldPosition().GetY() + level->GetTileHeight());
		}
	}
	case Edge::BOTTOM:
	{
		hasNeighbour = tiles[0]->GetIndices().y < level->GetLevelSize().y - 1;
		neighbourIndices.x = tiles[0]->GetIndices().x;
		neighbourIndices.y = tiles[0]->GetIndices().y + 1;
		if (hasNeighbour)
		{
			dist = level->GetTileFromLevel(neighbourIndices.x, neighbourIndices.y)->GetWorldPosition().GetY() - (_curKinematic.position.GetY() + _sprites[_currentSpriteSheet]->GetFrameHeight());
		}
	}
	}

	// Actually dig applicable tiles
    for (auto tile : tiles)
    {
        if (tile->GetID() == Tile::dirt)
        {
            dug = true;
			_gameScreen->GetSoundBank().PlaySound("dig");
			level->AddDugTile(tile);
            tile->SetID(Tile::blank);
        }
    }
    if (!dug && hasNeighbour && dist < 3)
    {
        for (auto tile : tiles)
        {
			std::shared_ptr<Tile> neighbour = level->GetTileFromLevel(neighbourIndices.x, neighbourIndices.y);
			if (tile->GetID() == Tile::dirt)
			{
				_gameScreen->GetSoundBank().PlaySound("dig");
				level->AddDugTile(tile);
				tile->SetID(Tile::blank);
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
	return _health <= 0 ? true : false;
}

bool PlayerActor::AtGoal()
{
	return _atGoal;
}

bool PlayerActor::TriggeredIntro()
{
	return _triggeredIntro;
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

void PlayerActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
	_jumped = false; 
	_godMode = false;
	_stoppedTime = false;
	_atGoal = false;
	_triggeredIntro = false;
	_wasOnGround = true;

	SetSpeed(Vector2(0.0f, 341.3f));

	_sprites[_currentSpriteSheet]->Stop();
	_currentSpriteSheet = "idle";
	_sprites[_currentSpriteSheet]->Start();

	_curKinematic = KinematicState(pos, GetSpeed(), 0.f, 0.f);
	_prevKinematic = _curKinematic;

	_collisionInfo.colEdge = Edge::NONE;
	_collisionInfo.colIntersect.clear();
	_collisionInfo.colPenetration = 0;
	_collisionInfo.rowEdge = Edge::NONE;
	_collisionInfo.rowIntersect.clear();
	_collisionInfo.rowPenetration = 0;

	_jumpVelocity = 0.0f;
	_digDir = Edge::NONE;
}
