#include <cstring>
#include "GameManager.h"
#include "MenuScreen.h"
#include "Vector2.h"

using namespace SDL2pp;

int MenuScreen::Load() {
	_mgr = GameManager::GetInstance();	
	
    _play = new Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Main_Menu\\main_menu_play.png");
	_quit = new Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Main_Menu\\main_menu_quit.png");
	_settings = new Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Main_Menu\\main_menu_settings.png");
    
    _menuTheme = new Music(".\\Assets\\Audio\\menu_theme.ogg");
    _mgr->GetMixer().PlayMusic(*_menuTheme);

	_curMenuItem = 0;
	_menuItems[0] = _play;
	_menuItems[1] = _settings;
	_menuItems[2] = _quit;

	return SCREEN_LOAD_SUCCESS;
}

int MenuScreen::Update(double elapsedSecs) {
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	// Change the currently selected menu item
	if (_mgr->inputManager->ActionOccurred("ARROWRIGHT", Input::Pressed)) { //keys[SDL_SCANCODE_RIGHT] && !_prevKeyState[SDL_SCANCODE_RIGHT]
		_curMenuItem++;
		if (_curMenuItem == NUM_MENU_ITEMS) _curMenuItem = 0;
	}
	else if (_mgr->inputManager->ActionOccurred("ARROWLEFT", Input::Pressed)) {
		_curMenuItem--;
		if (_curMenuItem < 0) _curMenuItem = NUM_MENU_ITEMS - 1;
	}

	// We selected a menu item; do the appropriate thing
	if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed)) {
		switch (_curMenuItem) {
		case 0:
            _mgr->GetMixer().HaltMusic();
			_mgr->SetNextScreen("levelSelect");
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
    delete _menuTheme;
}
