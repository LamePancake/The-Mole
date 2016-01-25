#include <cstring>
#include "GameManager.h"
#include "MenuScreen.h"
#include "Agent.h"

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

	Agent testAI;
	testAI.Update();

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
	rend.Present();
}

// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void MenuScreen::Unload() {
	delete _play;
	delete _quit;
	delete _settings;
	free(_prevKeyState);
}