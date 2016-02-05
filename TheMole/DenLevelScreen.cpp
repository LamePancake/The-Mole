#include "DenLevelScreen.h"
#include "GameManager.h"
#include "Vector2.h"

using namespace SDL2pp;

int DenLevelScreen::Load() {
	_mgr = GameManager::GetInstance();

	// Load level one in order to render
	_level = _levelLoader.LoadLevel(".\\Assets\\Levels\\den_level.txt", _player);
	_levelRenderer.Load(*_mgr);

	SDL2pp::Point playerPos((int)(_player->GetPosition().GetX()), (int)(_player->GetPosition().GetY()));
	SDL2pp::Point viewportSize = _mgr->GetWindow().GetSize();
	SDL2pp::Point levelSize = _level->GetLevelSize();
	levelSize.x *= _level->GetTileWidth();
	levelSize.y *= _level->GetTileHeight();
	_camera = new Camera(playerPos, viewportSize, levelSize);

	return SCREEN_LOAD_SUCCESS;
}

int DenLevelScreen::Update(double elasepdSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	for (size_t i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Update(_level);
	}
	_player->Update(_level);
	return SCREEN_CONTINUE;
}

void DenLevelScreen::Draw() 
{
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();

	for (size_t i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Draw(*_camera);
	}
	_player->Draw(*_camera);
	_levelRenderer.RenderLevel(_level, *_camera);
	
	rend.Present();
}

void DenLevelScreen::Unload() {
	free(_prevKeyState);
	delete _camera;
}