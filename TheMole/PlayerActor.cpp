#include "PlayerActor.h"
#include "GameScreen.h"

PlayerActor::PlayerActor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath)
	: Actor(position, manager, spd, texturePath, 4), _maxJumpVel(0), _jumpVelocity(0), _atGoal(false), _isDigging(false)
{
	_spriteSideDig = std::make_shared<SpriteSheet>((std::string)".\\Assets\\Textures\\Borin_sidedig_56x56.png", 4, 0.30, SpriteSheet::XAxisDirection::RIGHT);
	_spriteSideDigShadow = std::make_shared<SpriteSheet>((std::string)".\\Assets\\Textures\\Borin_sidedig_56x56.png", 4, 0.30, SpriteSheet::XAxisDirection::RIGHT);

	_spriteVerticalDig = std::make_shared<SpriteSheet>((std::string)".\\Assets\\Textures\\Borin_downdig_56x56.png", 4, 0.30, SpriteSheet::XAxisDirection::RIGHT);
	_spriteVerticalDigShadow = std::make_shared<SpriteSheet>((std::string)".\\Assets\\Textures\\Borin_downdig_56x56.png", 4, 0.30, SpriteSheet::XAxisDirection::RIGHT);

	_spriteWalk = std::make_shared<SpriteSheet>((std::string)".\\Assets\\Textures\\Borin_walk_56x56.png", 8, 1, SpriteSheet::XAxisDirection::RIGHT);
	_spriteWalkShadow = std::make_shared<SpriteSheet>((std::string)".\\Assets\\Textures\\Borin_walk_56x56.png", 8, 1, SpriteSheet::XAxisDirection::RIGHT);

	SDL_SetTextureColorMod(_spriteSideDigShadow->GetTexture().Get(), 127, 127, 127);
	SDL_SetTextureAlphaMod(_spriteSideDigShadow->GetTexture().Get(), 127);

	SDL_SetTextureColorMod(_spriteVerticalDigShadow->GetTexture().Get(), 127, 127, 127);
	SDL_SetTextureAlphaMod(_spriteVerticalDigShadow->GetTexture().Get(), 127);

	SDL_SetTextureColorMod(_spriteWalkShadow->GetTexture().Get(), 127, 127, 127);
	SDL_SetTextureAlphaMod(_spriteWalkShadow->GetTexture().Get(), 127);

	_sprite->Start();
	_spriteShadow->Start();

	_spriteSideDig->Stop();
	_spriteSideDigShadow->Stop();

	_spriteVerticalDig->Stop();
	_spriteVerticalDigShadow->Stop();

	_spriteWalk->Stop();
	_spriteWalkShadow->Stop();

	_spriteSideDig->SetRepeating(false);
	_spriteSideDigShadow->SetRepeating(false);

	_spriteVerticalDig->SetRepeating(false);
	_spriteVerticalDigShadow->SetRepeating(false);

	_currentSpriteSheet = _sprite;
	_currentSpriteSheetShadow = _spriteShadow;
}

PlayerActor::~PlayerActor()
{
}

void PlayerActor::Draw(Camera& camera)
{
	Actor::Draw(camera);

	const SDL2pp::Rect& viewport = camera.GetViewport();
	int offsetX = 4;
	int offsetY = 0;

	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	SDL2pp::Point tempPoint;

	tempPoint = { (int)_position.GetX(), (int)_position.GetY() };

	if (_spriteSideDig->IsAnimating())
	{
		_spriteSideDigShadow->Draw(tempPoint + SDL2pp::Point(offsetX - viewport.x, offsetY - viewport.y), _actorDir);
		_spriteSideDig->Draw(tempPoint + SDL2pp::Point(-viewport.x, -viewport.y), _actorDir);
	}
	else if (_spriteVerticalDig->IsAnimating())
	{
		_spriteVerticalDigShadow->Draw(tempPoint + SDL2pp::Point(offsetX - viewport.x, offsetY - viewport.y), _actorDir);
		_spriteVerticalDig->Draw(tempPoint + SDL2pp::Point(-viewport.x, -viewport.y), _actorDir);
	}
	else if(_spriteWalk->IsAnimating())
	{
		_spriteWalkShadow->Draw(tempPoint + SDL2pp::Point(offsetX - viewport.x, offsetY - viewport.y), _actorDir);
		_spriteWalk->Draw(tempPoint + SDL2pp::Point(-viewport.x, -viewport.y), _actorDir);
	}
	
}

void PlayerActor::Update(double elapsedSecs)
{
	Actor::Update(elapsedSecs);

	if ((_currentSpriteSheet == _spriteSideDig && !_currentSpriteSheet->IsAnimating()) ||
		(_currentSpriteSheet == _spriteVerticalDig && !_currentSpriteSheet->IsAnimating()))
	{
		_spriteSideDig->Stop();
		_spriteVerticalDig->Stop();
		_spriteSideDigShadow->Stop();
		_spriteVerticalDigShadow->Stop();

		SetActorDirection(SpriteSheet::XAxisDirection::RIGHT);

		_isDigging = false;
	}

	_spriteWalk->Update(elapsedSecs);
	_spriteWalkShadow->Update(elapsedSecs);

	_spriteSideDig->Update(elapsedSecs);
	_spriteSideDigShadow->Update(elapsedSecs);

	_spriteVerticalDig->Update(elapsedSecs);
	_spriteVerticalDigShadow->Update(elapsedSecs);

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
				if (_jumpVelocity != 0)
				{
					if (!(_jumpVelocity <= _maxJumpVel))
					{
						// acceleration of gravity * 64 pixels per metre * time per frame * -1 because negative is up for us
						_jumpVelocity += -9.8 * 64 * elapsedSecs * -1.0f;
					}
				}
				break;
			case Tile::dirt:
				if (canDig)
				{
					tile->SetID(Tile::blank);
					_isDigging = true;

					if (_currentSpriteSheet != _spriteVerticalDig)
					{
						_currentSpriteSheet->Stop();
						_currentSpriteSheetShadow->Stop();
						_currentSpriteSheet->SetDraw(false);
						_currentSpriteSheetShadow->SetDraw(false);

						_currentSpriteSheet = _spriteVerticalDig;
						_currentSpriteSheetShadow = _spriteVerticalDigShadow;
						
						_currentSpriteSheet->Start();
						_currentSpriteSheetShadow->Start();
						_currentSpriteSheet->SetDraw(true);
						_currentSpriteSheetShadow->SetDraw(true);
						SetSpeed(Vector2(0.0f, 0.0f));

						if(_digDir[1] == 'U') SetActorDirection(SpriteSheet::XAxisDirection::UP);
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
				if (_jumpVelocity != 0)
				{
					if (!(_jumpVelocity < _maxJumpVel))
					{
						// acceleration of gravity * 64 pixels per metre * time per frame * -1 because negative is up for us
						_jumpVelocity += -9.8 * 64 * elapsedSecs * -1.0f;
					}
				}
				break;
			case Tile::dirt:
				if (canDig)
				{
					tile->SetID(Tile::blank);
					_isDigging = true;

					if (_currentSpriteSheet != _spriteSideDig)
					{
						_currentSpriteSheet->Stop();
						_currentSpriteSheetShadow->Stop();
						_currentSpriteSheet->SetDraw(false);
						_currentSpriteSheetShadow->SetDraw(false);

						_currentSpriteSheet = _spriteSideDig;
						_currentSpriteSheetShadow = _spriteSideDigShadow;
						
						_currentSpriteSheet->Start();
						_currentSpriteSheetShadow->Start();
						_currentSpriteSheet->SetDraw(true);
						_currentSpriteSheetShadow->SetDraw(true);
						SetSpeed(Vector2(0.0f, 0.0f));
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
	if (_isDigging) return;

	_digDir[0] = ' ';
	_digDir[1] = ' ';

	_currentSpriteSheet->Pause();
	_currentSpriteSheetShadow->Pause();

	std::shared_ptr<SpriteSheet> prevSheet = _currentSpriteSheet;
	std::shared_ptr<SpriteSheet> prevSheetShadow = _currentSpriteSheetShadow;

	if (_mgr->inputManager->ActionOccurred("LEFT", Input::Held))
	{
		_digDir[0] = 'L';
		SetSpeed(Vector2(Math::Clamp(_speed.GetX() - 400.0f, -400.0f, 0.0f), _speed.GetY()));
		SetActorDirection(SpriteSheet::XAxisDirection::LEFT);

		_currentSpriteSheet = _spriteWalk;
		_currentSpriteSheetShadow = _spriteWalkShadow;
	}
	else if (_mgr->inputManager->ActionOccurred("RIGHT", Input::Held))
	{
		_digDir[0] = 'R';
		
		SetSpeed(Vector2(Math::Clamp(_speed.GetX() + 400.0f, 400.0f, 400.0f), _speed.GetY()));
		SetActorDirection(SpriteSheet::XAxisDirection::RIGHT);
	
		_currentSpriteSheet = _spriteWalk;
		_currentSpriteSheetShadow = _spriteWalkShadow;
	}
	else
	{
		SetSpeed(Vector2(0.0f, _speed.GetY()));

		_currentSpriteSheet = _sprite;
		_currentSpriteSheetShadow = _spriteShadow;
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

	if (_currentSpriteSheet != prevSheet)
	{
		prevSheet->Stop();
		prevSheetShadow->Stop();
		prevSheet->SetDraw(false);
		prevSheetShadow->SetDraw(false);
	}

	_currentSpriteSheet->Start();
	_currentSpriteSheetShadow->Start();
	_currentSpriteSheet->SetDraw(true);
	_currentSpriteSheetShadow->SetDraw(true);
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