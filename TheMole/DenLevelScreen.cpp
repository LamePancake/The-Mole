#include "DenLevelScreen.h"
#include "GameManager.h"
#include "Vector2.h"

using namespace SDL2pp;

int DenLevelScreen::Load() {
	_mgr = GameManager::GetInstance();

	// Load level one in order to render
	_level = _levelLoader.LoadLevel(".\\Assets\\Levels\\den_level.txt");
	_levelRenderer.Load(*_mgr);

	_prevKeyState = (Uint8*)std::malloc(sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(_prevKeyState, SDL_GetKeyboardState(nullptr), sizeof(Uint8) * SDL_NUM_SCANCODES);

	std::string spriteFile(".\\Assets\\Textures\\Borin_walk.png");
	_testSheet = new SpriteSheet(spriteFile, 8, 1.0);
	return SCREEN_LOAD_SUCCESS;
}

int DenLevelScreen::Update(double elasepdSecs) {
	SDL_PumpEvents();

	// Change the currently selected menu item
	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	if (keys[SDL_SCANCODE_ESCAPE]) {
		exit(0);
	}

	for (int i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Update(_level);
	}

	// Save the previous key state (temporary until InputManager actions are implemented)
	std::memcpy(_prevKeyState, keys, sizeof(Uint8) * SDL_NUM_SCANCODES);

	_testSheet->Update(elasepdSecs);
	return SCREEN_CONTINUE;
}

void DenLevelScreen::Draw() {
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();

	_levelRenderer.RenderLevel(_level);
	for (int i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Draw(_level);
	}

	_testSheet->Draw(Point(_mgr->GetWindow().GetWidth() / 2, _mgr->GetWindow().GetHeight() / 2));

	rend.Present();
}

void DenLevelScreen::Unload() {
	free(_prevKeyState);
}