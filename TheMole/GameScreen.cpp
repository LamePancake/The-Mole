#include "GameScreen.h"
#include "GameManager.h"
#include "Vector2.h"

std::shared_ptr<Level> GameScreen::GetLevel() const
{
	return _level;
}

const std::shared_ptr<PlayerActor> GameScreen::GetPlayer() const
{
	return _player;
}

int GameScreen::Load()
{
	_mgr = GameManager::GetInstance();

	// Load level one in order to render
	_level = _levelLoader.LoadLevel(_levelPath, _player);
	_levelRenderer.Load(*_mgr);

	SDL2pp::Point playerPos((int)(_player->GetPosition().GetX()), (int)(_player->GetPosition().GetY()));
	SDL2pp::Point viewportSize = _mgr->GetWindow().GetSize();
	SDL2pp::Point levelSize = _level->GetLevelSize();
	levelSize.x *= _level->GetTileWidth();
	levelSize.y *= _level->GetTileHeight();
	_camera = new Camera(playerPos, viewportSize, levelSize);

	_background = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), _backgroundPath);

	return SCREEN_LOAD_SUCCESS;
}

int GameScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	if (_mgr->inputManager->ActionOccurred("QUIT", Input::Pressed)) {
		exit(0);
	}

	_player->Update(elapsedSecs);
	if (_player->StoppedTime())	return SCREEN_CONTINUE;
	

	if (_player->IsDead())
	{
		GameManager::GetInstance()->SetNextScreen("menu");
		return SCREEN_FINISH;
	}

	if (_player->AtGoal())
	{
		_mgr->SetNextScreen(_nextLevel);
		return SCREEN_FINISH;
	}

	// Update Enemies
	for (size_t i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Update(elapsedSecs);
	}

	// Update objects
	for (size_t i = 0; i < _level->GetActorObjectSize(); ++i)
	{
		_level->GetActorObject(i)->Update(elapsedSecs);
	}

	// Update NPCs
	for (size_t i = 0; i < _level->GetNPCSize(); ++i)
	{
		_level->GetNPC(i)->Update(elapsedSecs);
	}

	_level->GetBoss()->Update(elapsedSecs);

	_level->UpdateDugTile(elapsedSecs);

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

	for (size_t i = 0; i < _level->GetProjectileActorSize(); ++i)
	{
		_level->GetProjectile(i)->Draw(*_camera);
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

