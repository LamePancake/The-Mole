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

	return SCREEN_LOAD_SUCCESS;
}

int LevelSelectScreen::Update(double elapsedSecs) 
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	if (_mgr->inputManager->ActionOccurred("ARROWDOWN", Input::Pressed))
	{
		_curMenuItem++;
		if (_curMenuItem == NUM_LEVELS) _curMenuItem = 0;
	}
	else if (_mgr->inputManager->ActionOccurred("ARROWUP", Input::Pressed))
	{
		_curMenuItem--;
		if (_curMenuItem < 0) _curMenuItem = NUM_LEVELS - 1;
	}

	// We selected a menu item; do the appropriate thing
	if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
	{
		switch (_curMenuItem) 
		{
		case 0:
			Reset();
			_mgr->SetNextScreen("denIntro");
			return SCREEN_FINISH;
		case 1:
			Reset();
			_mgr->SetNextScreen("vikingIntro");
			return SCREEN_FINISH;
		case 2:
			Reset();
			_mgr->SetNextScreen("grassIntro");
			return SCREEN_FINISH;
		case 3:
			Reset();
			_mgr->SetNextScreen("starscapeIntro");
			return SCREEN_FINISH;
		case 4:
			Reset();
			_mgr->SetNextScreen("bossIntro");
			return SCREEN_FINISH;
		case 5:
			Reset();
			_mgr->SetNextScreen("menu");
			return SCREEN_FINISH;
		}
	}

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
}

void LevelSelectScreen::Reset()
{
}

