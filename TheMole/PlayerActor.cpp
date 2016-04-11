#include "PlayerActor.h"
#include "AIActor.h"
#include "DoorActor.h"
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
	_stoppedTime(false),
	_selected(0),
	_jumpBoost(600),
	_jumpBoosted(false)
{
	_shieldActive = false;///Start shield in inactive state
	_shieldReleased = true;
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

	if (!_gameScreen->isIntroScreen && _gameScreen->GetLevel()->IsHatAvailable("SHIELD"))
	{
		for (int i = 0; i < _shieldStr; i++) {
			rend.FillRect(SDL2pp::Rect(dim.GetX() * (0.90f + (i * 0.02f))
				, dim.GetY()  * 0.97f
				, 10.0f
				, 10.0f));
		}

		//if (_shieldActive)
		//	rend.FillRect(SDL2pp::Rect(dim.GetX() * (0.2f)
		//		, dim.GetY()  * 0.2f
		//		, 20.0f
		//		, 20.0f));
	}
}

void PlayerActor::Update(double elapsedSecs)
{
	bool wasDigging = _digDir != Edge::NONE;

	UpdateInput(elapsedSecs);
	if (_isDestroyed || !_isActive || _stoppedTime) return;

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
	if (wasDigging && _digDir == Edge::NONE)
	{
		_sprites[_currentSpriteSheet]->Stop();
		_currentSpriteSheet = _curKinematic.velocity.GetX() > 1 ? "walk" : "idle";
		SetActorYDirection(SpriteSheet::YAxisDirection::UP);
		_sprites[_currentSpriteSheet]->Start();
	}

	UpdatePosition(elapsedSecs);
	_aabb.UpdatePosition(*this);

	for (auto actor : _gameScreen->GetLevel()->GetActors())
	{
		if (!actor->IsActive() || actor.get() == (Actor*)this) continue;

		if (CollisionCheck(*actor))
		{
			Type type = actor->GetType();
			switch (type)
			{
			case Type::enemy:
			case Type::bombenemy:
				SetHealth(0);
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
			case Tile::tunneltrig:
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
	Edge newDigDir = Edge::NONE;
	_digDir = Edge::NONE;
	_stoppedTime = false;
	shared_ptr<Level> level = _gameScreen->GetLevel();

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

	if (_mgr->inputManager->ActionOccurred("DIG", Input::Down))
	{
		if (_mgr->inputManager->ActionOccurred("LEFT", Input::Down) ||
			_mgr->inputManager->ActionOccurred("RIGHT", Input::Down))
		{
			_digDir = _mgr->inputManager->ActionOccurred("RIGHT", Input::Down) ? Edge::RIGHT : Edge::LEFT;
			SetActorXDirection(_digDir == Edge::LEFT ? SpriteSheet::XAxisDirection::LEFT : SpriteSheet::XAxisDirection::RIGHT);
			SetActorYDirection(SpriteSheet::YAxisDirection::UP);
			if (_currentSpriteSheet != "sideDig")
			{
				_sprites[_currentSpriteSheet]->Stop();
				_currentSpriteSheet = "sideDig";
				_sprites[_currentSpriteSheet]->Start();
			}
		}
		else if (_mgr->inputManager->ActionOccurred("UP", Input::Down) ||
			_mgr->inputManager->ActionOccurred("DOWN", Input::Down))
		{
			_digDir = _mgr->inputManager->ActionOccurred("UP", Input::Down) ? Edge::TOP : Edge::BOTTOM;
			SetActorYDirection(_digDir == Edge::TOP ? SpriteSheet::YAxisDirection::UP : SpriteSheet::YAxisDirection::DOWN);
			if (_currentSpriteSheet != "verticalDig")
			{
				_sprites[_currentSpriteSheet]->Stop();
				_currentSpriteSheet = "verticalDig";
				_sprites[_currentSpriteSheet]->Start();
			}
		}
	}
	else
	{
		SetActorYDirection(SpriteSheet::YAxisDirection::UP);
		if (_mgr->inputManager->ActionOccurred("LEFT", Input::Down))
		{
			SetActorXDirection(SpriteSheet::XAxisDirection::LEFT);
			SetSpeed(Vector2(Math::Clamp(_curKinematic.velocity.GetX() - 50.0f, -300.0f, -50.0f), _curKinematic.velocity.GetY()));
			if (_currentSpriteSheet != "walk")
			{
				_sprites[_currentSpriteSheet]->Stop();
				_currentSpriteSheet = "walk";
				_sprites[_currentSpriteSheet]->Start();
			}
		}
		else if (_mgr->inputManager->ActionOccurred("RIGHT", Input::Down))
		{
			SetActorXDirection(SpriteSheet::XAxisDirection::RIGHT);
			SetSpeed(Vector2(Math::Clamp(_curKinematic.velocity.GetX() + 50.0f, 50.0f, 300.0f), _curKinematic.velocity.GetY()));

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
			SetSpeed(Vector2(0.0f, _curKinematic.velocity.GetY()));
			if (_currentSpriteSheet == "walk")
			{
				_sprites[_currentSpriteSheet]->Stop();
				_currentSpriteSheet = "idle";
				_sprites[_currentSpriteSheet]->Start();
			}
		}
	}

	if (_mgr->inputManager->ActionOccurred("JUMP", Input::Pressed))
	{
		if (_wasOnGround)
		{
			// jump 7.5 tiles tall of 1 metre each, at 64 pixels per metre, multiplied by -1 because positive moves down in our world
			_jumped = true;
			_gameScreen->GetSoundBank().PlaySound("jump");
			SetJumpVelocity(7.5f * 1.0f * 64.0f * -1.0f);
		}
	}

	//COMMENT OUT THIS LINE TO DISABLE JUMP
	SetSpeed(Vector2(_curKinematic.velocity.GetX(), GetJumpVelocity()));

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
		if (_shieldReleased)
		{
			_shieldActive = true;
			_shieldReleased = false;
		}
	}

	if (_mgr->inputManager->ActionOccurred("SHIELD", Input::Held) && level->IsHatAvailable("SHIELD"))
	{
		//_shieldReleased = false;
	}

	if (_mgr->inputManager->ActionOccurred("SHIELD", Input::Released) && level->IsHatAvailable("SHIELD"))
	{
		///Deactivate Shield
		//_shieldReleased = true;
	}

}

void PlayerActor::UpdateShieldStatus(double deltaTime)
{
	//if (_shieldActive)
	//{

	if (_shieldActive && _shieldTimer < 2)
	{
		_shieldTimer += deltaTime;
		_shieldStr = 4 - (_shieldTimer * 2);
	}

	if (_shieldTimer > 2)
	{
		_shieldTimer += deltaTime;
		_shieldActive = false;
		_shieldStr = 0 + (_shieldTimer - 2) * 2;
	}

	if (_shieldTimer > 4)
	{
		_shieldReleased = true;
		_shieldTimer = 0;
		_shieldStr = 4;
	}


	//}
}

void PlayerActor::ShieldHit()
{
	//_shieldStr--;
}

void PlayerActor::ProjectileHit(Actor *prj)
{
	if (_lastPrj != prj)
	{
		if (_shieldActive)
		{

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
		if (actor->GetType() == Type::enemy || actor->GetType() == Type::bombenemy)
		{
			// Check whether the enemy is in range, offsetting their position as necessary 
			// to make sure that the distance is the same on all sides
			shared_ptr<AIActor> enemy = dynamic_pointer_cast<AIActor>(actor);
			Vector2 enemyPos = enemy->GetPosition();
			AABB enemyAABB = enemy->GetAABB();
			if (enemyPos.GetX() < _curKinematic.position.GetX()) enemyPos.SetX(enemyPos.GetX() + enemyAABB.GetWidth());
			if (enemyPos.GetY() < _curKinematic.position.GetY()) enemyPos.SetY(enemyPos.GetY() + enemyAABB.GetHeight());

			if (centre.Distance(enemyPos) <= _mindControlRadius) inRange.push_back(enemy);
		}
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
	bool isX = _digDir == Edge::LEFT || _digDir == Edge::RIGHT;
	bool hasNeighbour = false;

	// We need to copy tiles by value so as not to modify the stuff in DetectTileCollisions
	// This is basically entirely due to digging upward. Oh well
	vector<shared_ptr<Tile>> tiles;
	Bounds playerBounds;
	GetBounds(_curKinematic, playerBounds);
	GetTilesAlongEdge(_digDir, playerBounds, tiles);

	float dist;
	float distMargin; // How close can we be to be considered "touching"?
	SDL2pp::Rect playerRect{ (int)_aabb.GetX(), (int)_aabb.GetY(), (int)_aabb.GetWidth(), (int)_aabb.GetHeight() };
	SDL2pp::Rect tileRect{ 0, 0, (int)tiles[0]->GetWidth(), (int)tiles[0]->GetHeight() };

	switch (_digDir)
	{
	case Edge::LEFT:
	{
		hasNeighbour = level->HasNeighbourTile(tiles[0], Edge::LEFT);
		if (hasNeighbour)
		{
			std::shared_ptr<Tile> neighbour = level->GetNeighbourTile(tiles[0], Edge::LEFT);
			dist = _curKinematic.position.GetX() - (neighbour->GetWorldPosition().GetX() + level->GetTileWidth());
		}
		break;
	}
	case Edge::RIGHT:
	{
		hasNeighbour = level->HasNeighbourTile(tiles[0], Edge::RIGHT);
		if (hasNeighbour)
		{
			std::shared_ptr<Tile> neighbour = level->GetNeighbourTile(tiles[0], Edge::RIGHT);
			dist = neighbour->GetWorldPosition().GetX() - (_curKinematic.position.GetX() + _sprites[_currentSpriteSheet]->GetFrameWidth());
		}
		break;
	}
	case Edge::TOP:
	{
		hasNeighbour = level->HasNeighbourTile(tiles[0], Edge::TOP);
		if (hasNeighbour)
		{
			std::shared_ptr<Tile> neighbour = level->GetNeighbourTile(tiles[0], Edge::TOP);
			dist = _curKinematic.position.GetY() - (neighbour->GetWorldPosition().GetY() + level->GetTileHeight());
		}
		break;
	}
	case Edge::BOTTOM:
	{
		hasNeighbour = level->HasNeighbourTile(tiles[0], Edge::BOTTOM);
		if (hasNeighbour)
		{
			std::shared_ptr<Tile> neighbour = level->GetNeighbourTile(tiles[0], Edge::BOTTOM);
			dist = neighbour->GetWorldPosition().GetY() - (_curKinematic.position.GetY() + _sprites[_currentSpriteSheet]->GetFrameHeight());
		}
		break;
	}
	}

	// Actually dig applicable tiles
	for (auto tile : tiles)
	{
		tileRect.x = tile->GetWorldPosition().GetX();
		tileRect.y = tile->GetWorldPosition().GetY();
		float intersect = isX ? std::min(playerRect.y + playerRect.h, tileRect.y + tileRect.h) - std::max(playerRect.y, tileRect.y)
			: std::min(playerRect.x + playerRect.w, tileRect.x + tileRect.w) - std::max(playerRect.x, tileRect.x);
		float alignment = isX ? intersect / tile->GetHeight() : intersect / tile->GetWidth();

		if (tile->GetID() == Tile::dirt && (_digDir == Edge::TOP || _digDir == Edge::BOTTOM || alignment > 0.3f))
		{
			dug = true;
			level->AddDugTile(tile);
			tile->SetID(Tile::blank);
		}
	}

	// Check neighbouring tiles that are "close enough" to see if they're diggable
	// Tiles above the player are given a wider margin because reasons
	if (!dug && hasNeighbour && dist < (_digDir == Edge::TOP ? 13 : 3))
	{
		for (auto tile : tiles)
		{
			std::shared_ptr<Tile> neighbour = level->GetNeighbourTile(tile, _digDir);
			tileRect.x = neighbour->GetWorldPosition().GetX();
			tileRect.y = neighbour->GetWorldPosition().GetY();
			float intersect = isX ? std::min(playerRect.y + playerRect.h, tileRect.y + tileRect.h) - std::max(playerRect.y, tileRect.y)
				: std::min(playerRect.x + playerRect.w, tileRect.x + tileRect.w) - std::max(playerRect.x, tileRect.x);
			float alignment = isX ? intersect / tile->GetHeight() : intersect / tile->GetWidth();

			if (neighbour->GetID() == Tile::dirt && alignment > 0.3f)
			{
				dug = true;
				level->AddDugTile(neighbour);
				neighbour->SetID(Tile::blank);
			}
		}
	}

	if (dug)
	{
		if (_digDir == Edge::TOP)
		{
			SetJumpVelocity(7.5f * 1.0f * 64.0f * -1.0f);
		}
		_gameScreen->GetSoundBank().PlaySound("dig");
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

Actor * PlayerActor::Clone()
{
	return nullptr;
}