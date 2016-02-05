#include "DenLevelScreen.h"
#include "GameManager.h"
#include "Vector2.h"

using namespace SDL2pp;

int DenLevelScreen::Load() {
	_mgr = GameManager::GetInstance();

	// Load level one in order to render
	_level = _levelLoader.LoadLevel(".\\Assets\\Levels\\den_level.txt", _player);
	_levelRenderer.Load(*_mgr);

	_prevKeyState = (Uint8*)std::malloc(sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(_prevKeyState, SDL_GetKeyboardState(nullptr), sizeof(Uint8) * SDL_NUM_SCANCODES);

	std::string spriteFile(".\\Assets\\Textures\\Borin_walk.png");
	_testSheet = new SpriteSheet(spriteFile, 8, 1.0);

	SDL2pp::Point playerPos(_level->GetEnemy(1)->GetPosition().GetX(), _level->GetEnemy(1)->GetPosition().GetY());
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

	// Change the currently selected menu item
	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	if (keys[SDL_SCANCODE_ESCAPE]) {
		exit(0);
	}

	if (_mgr->inputManager->ActionOccured("LEFT", Input::Held)) 
	{
		_player->SetSpeed(Vector2(-2.0f, _player->GetSpeed().GetY()));
	}
	else if (_mgr->inputManager->ActionOccured("RIGHT", Input::Held))
	{
		_player->SetSpeed(Vector2(2.0f, _player->GetSpeed().GetY()));
	}
	else
	{
		_player->SetSpeed(Vector2(0.0f, _player->GetSpeed().GetY()));
	}

	for (size_t i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Update(_level);
	}
	_player->Update(_level);

	// Save the previous key state (temporary until InputManager actions are implemented)
	std::memcpy(_prevKeyState, keys, sizeof(Uint8) * SDL_NUM_SCANCODES);

	_testSheet->Update(elasepdSecs);
	return SCREEN_CONTINUE;
}

void DenLevelScreen::Draw() 
{
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();
	_camera->CentreView(_player->GetPosition());

	for (size_t i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Draw(*_camera);
	}
	_player->Draw(*_camera);
	_levelRenderer.RenderLevel(_level, *_camera);

	_testSheet->Draw(Point(_mgr->GetWindow().GetWidth() / 2, _mgr->GetWindow().GetHeight() / 2));

	rend.Present();
}

void DenLevelScreen::Unload() {
	free(_prevKeyState);
}