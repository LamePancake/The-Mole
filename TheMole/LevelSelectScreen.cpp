#include "LevelSelectScreen.h"
#include <cstring>
#include "GameManager.h"
#include "Vector2.h"

using namespace SDL2pp;

int LevelSelectScreen::Load() 
{
	_mgr = GameManager::GetInstance();

	_denLevel       = new Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Level_Select\\level_den.png");
	_vikingLevel    = new Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Level_Select\\level_viking.png");
	_grassLevel     = new Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Level_Select\\level_grass.png");
	_starscapeLevel = new Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Level_Select\\level_starscape.png");
	_bossLevel      = new Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Level_Select\\level_boss.png");
	_back           = new Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Level_Select\\level_back.png");

	_curMenuItem = 0;
	_levelItems[0] = _denLevel;
	_levelItems[1] = _vikingLevel;
	_levelItems[2] = _grassLevel;
	_levelItems[3] = _starscapeLevel;
	_levelItems[4] = _bossLevel;
	_levelItems[5] = _back;

	_prevKeyState = (Uint8*)std::malloc(sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(_prevKeyState, SDL_GetKeyboardState(nullptr), sizeof(Uint8) * SDL_NUM_SCANCODES);
	return SCREEN_LOAD_SUCCESS;
}

int LevelSelectScreen::Update(double elapsedSecs) 
{
	SDL_PumpEvents();

	// Change the currently selected menu item
	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	if (keys[SDL_SCANCODE_DOWN] && !_prevKeyState[SDL_SCANCODE_DOWN]) 
	{
		_curMenuItem++;
		if (_curMenuItem == NUM_LEVELS) _curMenuItem = 0;
	}
	else if (keys[SDL_SCANCODE_UP] && !_prevKeyState[SDL_SCANCODE_UP]) 
	{
		_curMenuItem--;
		if (_curMenuItem < 0) _curMenuItem = NUM_LEVELS - 1;
	}

	// We selected a menu item; do the appropriate thing
	if (keys[SDL_SCANCODE_RETURN] && !_prevKeyState[SDL_SCANCODE_RETURN])
	{
		switch (_curMenuItem) 
		{
		case 0:
			_mgr->SetNextScreen("denlevel");
			return SCREEN_FINISH;
		case 1:
			_mgr->SetNextScreen("vikinglevel");
			return SCREEN_FINISH;
		case 2:
			_mgr->SetNextScreen("grasslevel");
			return SCREEN_FINISH;
		case 3:
			_mgr->SetNextScreen("starscapelevel");
			return SCREEN_FINISH;
		case 4:
			_mgr->SetNextScreen("bosslevel");
			return SCREEN_FINISH;
		case 5:
			_mgr->SetNextScreen("menu");
			return SCREEN_FINISH;
		}
	}

	// Save the previous key state (temporary until InputManager actions are implemented)
	std::memcpy(_prevKeyState, keys, sizeof(Uint8) * SDL_NUM_SCANCODES);
	return SCREEN_CONTINUE;
}

void LevelSelectScreen::Draw() 
{
	Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();
	rend.Copy(*_levelItems[_curMenuItem], NullOpt, NullOpt);

	rend.Present();
}

// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void LevelSelectScreen::Unload() 
{
	delete _denLevel;
	delete _vikingLevel;
	delete _grassLevel;
	delete _starscapeLevel;
	delete _bossLevel;
	delete _back;
	free(_prevKeyState);
}

