#include "DenLevelScreen.h"
#include "GameManager.h"
#include "Vector2.h"

int DenLevelScreen::Load() {
	_mgr = GameManager::GetInstance();

	//_testAI1 = SimpleAgent(Vector2(100.0f, 0.0f), *_mgr);
	//_testAI2 = SimpleAgent(Vector2(-100.0f, 0.0f), *_mgr);

	// Load level one in order to render
	_level = _levelLoader.LoadLevel(".\\Assets\\Levels\\den_level.txt");
	_levelRenderer.Load(*_mgr);

	_prevKeyState = (Uint8*)std::malloc(sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(_prevKeyState, SDL_GetKeyboardState(nullptr), sizeof(Uint8) * SDL_NUM_SCANCODES);

	return SCREEN_LOAD_SUCCESS;
}

int DenLevelScreen::Update(double elasepdSecs) {
	SDL_PumpEvents();
	_mgr->inputManager.UpdateKeyboardState();

	// Change the currently selected menu item
	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	if (keys[SDL_SCANCODE_ESCAPE]) {
		exit(0);
	}

	for (int i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Update(_level);
	}

	//_testAI1.Update(_levelOne);
	//_testAI1.SetPosition(Vector2((_testAI1.GetPosition().GetX() - 0.1f), _testAI1.GetPosition().GetY()));
	//std::cout << _testAI1.GetPosition().GetX() << ", " << _testAI1.GetPosition().GetY() << "AI1\n";

	//_testAI2.Update();
	//_testAI2.SetPosition(Vector2((_testAI2.GetPosition().GetX() + 0.1f), _testAI2.GetPosition().GetY()));
	//std::cout << _testAI2.GetPosition().GetX() << ", " << _testAI2.GetPosition().GetY() << "AI2\n";

	//if (_testAI1.CollisionCheck(_testAI2))
	//{
	//	std::cout << "TITANIC HIT THE ICEBERG\n";
	//}

	// Save the previous key state (temporary until InputManager actions are implemented)
	std::memcpy(_prevKeyState, keys, sizeof(Uint8) * SDL_NUM_SCANCODES);
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

	rend.Present();
}

void DenLevelScreen::Unload() {
	free(_prevKeyState);
}