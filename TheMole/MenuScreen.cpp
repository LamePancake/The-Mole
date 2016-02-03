#include <cstring>
#include "GameManager.h"
#include "MenuScreen.h"
#include "Vector2.h"

using namespace SDL2pp;

int MenuScreen::Load(GameManager& manager) {
	_mgr = &manager;	
	_play = new Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Main_Menu\\main_menu_play.png");
	_quit = new Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Main_Menu\\main_menu_quit.png");
	_settings = new Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Main_Menu\\main_menu_settings.png");

	_curMenuItem = 0;
	_menuItems[0] = _play;
	_menuItems[1] = _settings;
	_menuItems[2] = _quit;

	_prevKeyState = (Uint8*)std::malloc(sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(_prevKeyState, SDL_GetKeyboardState, sizeof(Uint8) * SDL_NUM_SCANCODES);

	//_testAI1 = SimpleAgent(Vector2(1024.0f, 1024.0f), manager, Vector2(-0.1f, 0));
	//_testAI2 = SimpleAgent(Vector2(-100.0f, 0.0f), manager, Vector2(0.1f, 0));

	// Load level one in order to render
	_levelOne = _levelLoader.LoadLevel(".\\Assets\\Levels\\den_level.txt");
	_levelRenderer.Load(manager);

	return SCREEN_LOAD_SUCCESS;
}

int MenuScreen::Update(double elapsedSecs) {
	SDL_PumpEvents();

	// Change the currently selected menu item
	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	if (keys[SDL_SCANCODE_RIGHT] && !_prevKeyState[SDL_SCANCODE_RIGHT]) {
		_curMenuItem++;
		if (_curMenuItem == NUM_MENU_ITEMS) _curMenuItem = 0;
	}
	else if (keys[SDL_SCANCODE_LEFT] && !_prevKeyState[SDL_SCANCODE_LEFT]) {
		_curMenuItem--;
		if (_curMenuItem < 0) _curMenuItem = NUM_MENU_ITEMS - 1;
	}


	//_testAI1.Update();
	//_testAI1.SetSpeed(Vector2(-0.1f, 0));
	//std::cout << _testAI1.GetPosition().GetX() << ", " << _testAI1.GetPosition().GetY() << "AI1\n";

	//_testAI2.Update();
	//_testAI2.SetSpeed(Vector2(0.1f, 0));
	//std::cout << _testAI2.GetPosition().GetX() << ", " << _testAI2.GetPosition().GetY() << "AI2\n";

	//if (_testAI1.CollisionCheck(_testAI2))
	//{
	//	  std::cout << "TITANIC HIT THE ICEBERG\n";
	//}

	// We selected a menu item; do the appropriate thing
	if (keys[SDL_SCANCODE_RETURN]) {
		switch (_curMenuItem) {
		case 0:
			_mgr->SetNextScreen("level1");
			return SCREEN_FINISH;
		case 1:
			_mgr->SetNextScreen("settings");
			return SCREEN_OVERLAY;
		case 2:
			// We just tried to quit
			// So for now we're going to do it the dirty way
			exit(0);
		}
		
	}

	std::memcpy(_prevKeyState, keys, sizeof(Uint8) * SDL_NUM_SCANCODES);
	return SCREEN_CONTINUE;
}

void MenuScreen::Draw() {
	Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();
	rend.Copy(*_menuItems[_curMenuItem], NullOpt, NullOpt);
	
	_levelRenderer.RenderLevel(_levelOne);

	rend.Present();
}

// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void MenuScreen::Unload() {
	delete _play;
	delete _quit;
	delete _settings;
	free(_prevKeyState);
}