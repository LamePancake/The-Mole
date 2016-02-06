#include "GameScreen.h"
#include "GameManager.h"
#include "Vector2.h"

const std::shared_ptr<Level> GameScreen::GetLevel() const
{
	return _level;
}

const std::shared_ptr<PlayerActor> GameScreen::GetPlayer() const
{
	return _player;
}

int GameScreen::Load(std::string levelAddress, std::string textureAddress)
{
	_mgr = GameManager::GetInstance();

	// Load level one in order to render
	_level = _levelLoader.LoadLevel(levelAddress, _player);
	_levelRenderer.Load(*_mgr);

	SDL2pp::Point playerPos((int)(_player->GetPosition().GetX()), (int)(_player->GetPosition().GetY()));
	SDL2pp::Point viewportSize = _mgr->GetWindow().GetSize();
	SDL2pp::Point levelSize = _level->GetLevelSize();
	levelSize.x *= _level->GetTileWidth();
	levelSize.y *= _level->GetTileHeight();
	_camera = new Camera(playerPos, viewportSize, levelSize);

	_background = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), textureAddress);

	return SCREEN_LOAD_SUCCESS;
}

int GameScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	if (_mgr->inputManager->ActionOccurred("QUIT", Input::Pressed)) {
		exit(0);
	}

	if (_player->IsDead() || _player->AtGoal())
	{
		GameManager::GetInstance()->SetNextScreen("menu");
		return SCREEN_FINISH;
	}

	if (_mgr->inputManager->ActionOccurred("LEFT", Input::Held))
	{
		if (!_player->Dig('L', _level))
		{
			_player->SetSpeed(Vector2(Math::Clamp(_player->GetSpeed().GetX() - 5.0f, -120.0f, 0.0f), _player->GetSpeed().GetY()));
			_player->SetActorDirection(SpriteSheet::XAxisDirection::LEFT);
		}
	}
	else if (_mgr->inputManager->ActionOccurred("RIGHT", Input::Held))
	{
		if (!_player->Dig('R', _level))
		{
			_player->SetSpeed(Vector2(Math::Clamp(_player->GetSpeed().GetX() + 5.0f, 0.0f, 120.0f), _player->GetSpeed().GetY()));
			_player->SetActorDirection(SpriteSheet::XAxisDirection::RIGHT);
		}
	}
	else
	{
		_player->SetSpeed(Vector2(0.0f, _player->GetSpeed().GetY()));
	}

	if (_mgr->inputManager->ActionOccurred("UP", Input::Held))
	{
		if (!_player->Dig('U', _level))
		{
			_player->SetSpeed(Vector2(_player->GetSpeed().GetX(), Math::Clamp(_player->GetSpeed().GetY() - 120.0f, 0.0f, -120.0f)));
		}
	}
	else if (_mgr->inputManager->ActionOccurred("DOWN", Input::Held))
	{
		if (!_player->Dig('D', _level))
		{
			_player->SetSpeed(Vector2(_player->GetSpeed().GetX(), Math::Clamp(_player->GetSpeed().GetY() + 120.0f, 0.0f, 120.0f)));
		}
	}
	else if (_mgr->inputManager->ActionOccurred("JUMP", Input::Pressed))
	{
		_player->SetJumpVelocity(800.0f);
		_player->SetMaximumJumpVelocity(800.0f);
	}
	else
	{
		_player->SetSpeed(Vector2(_player->GetSpeed().GetX(), 0.0f));
	}

	if (_player->GetJumpVelocity() > -_player->GetMaximumJumpVelocity())
	{
		_player->SetSpeed(Vector2(_player->GetSpeed().GetX(), _player->GetSpeed().GetY() - _player->GetJumpVelocity()));
		_player->SetJumpVelocity(_player->GetJumpVelocity() - elapsedSecs * 100.0f);
	}

	// Update Enemies
	for (size_t i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Update(elapsedSecs, _level);
	}

	// Update objects
	for (size_t i = 0; i < _level->GetActorObjectSize(); ++i)
	{
		_level->GetActorObject(i)->Update(elapsedSecs, _level);
	}

	// Update NPCs
	for (size_t i = 0; i < _level->GetNPCSize(); ++i)
	{
		_level->GetNPC(i)->Update(elapsedSecs, _level);
	}

	//Update Player
	_player->Update(elapsedSecs, _level);

	return SCREEN_CONTINUE;
}

void GameScreen::Draw()
{
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();

	rend.Copy(*_background, SDL2pp::NullOpt, SDL2pp::NullOpt);
	_camera->CentreView(_player->GetPosition());

	// Render Level
	_levelRenderer.RenderLevel(_level, *_camera);

	// Render enemies
	for (size_t i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Draw(*_camera);
	}

	// Render objects
	for (size_t i = 0; i < _level->GetActorObjectSize(); ++i)
	{
		_level->GetActorObject(i)->Draw(*_camera);
	}

	// Render NPCs
	for (size_t i = 0; i < _level->GetNPCSize(); ++i)
	{
		_level->GetNPC(i)->Draw(*_camera);
	}

	// Render Player
	_player->Draw(*_camera);

	rend.Present();
}

void GameScreen::Unload()
{
	free(_prevKeyState);
	delete _camera;
}

