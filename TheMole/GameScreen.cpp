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
	_player->SetSpeed({ 400.f, 400.f });

	return SCREEN_LOAD_SUCCESS;
}

int GameScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	//Update Player
	_player->Update(elapsedSecs);

	if (_mgr->inputManager->ActionOccurred("QUIT", Input::Pressed)) {
		exit(0);
	}

	if (_player->IsDead() || _player->AtGoal())
	{
		GameManager::GetInstance()->SetNextScreen("menu");
		return SCREEN_FINISH;
	}

	if (_player->GetJumpVelocity() > -_player->GetMaximumJumpVelocity())
	{
		_player->SetSpeed(Vector2(_player->GetSpeed().GetX(), _player->GetSpeed().GetY() - _player->GetJumpVelocity()));
		_player->SetJumpVelocity(_player->GetJumpVelocity() - elapsedSecs * 100.0f);
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

