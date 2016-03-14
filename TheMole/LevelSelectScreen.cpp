#include "LevelSelectScreen.h"
#include <cstring>
#include "GameManager.h"
#include "Vector2.h"

using namespace SDL2pp;

const static std::string LEVEL_NAMES[6] = { "den", "viking", "grass", "starscape", "boos", "back" };

const static SDL_Color NORMAL   = { 255, 255, 255, 255 };
const static SDL_Color LOCKED   = { 80, 80, 80, 255 };
const static SDL_Color SELECTED = { 255, 80, 80, 255 };
const static SDL_Color LOCKED_AND_SELECTED = { 140, 80, 80, 255 };

int LevelSelectScreen::Load() 
{
	_mgr = GameManager::GetInstance();

	_mgr->ReadFile(".\\Assets\\Levels\\level_unlocks.txt");

	_font = new SDL2pp::Font(".\\Assets\\GUI\\Exo-Regular.otf", 50);
	_headerFont = new SDL2pp::Font(".\\Assets\\GUI\\BEBAS.ttf", 70);

	_backgroundTextures[0] = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\den_bg.png");
	_backgroundTextures[1] = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\viking_bg.png");
	_backgroundTextures[2] = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\grasslands_bg.png");
	_backgroundTextures[3] = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\starscape_bg.png");
	_backgroundTextures[4] = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\boss_bg.png");
	_backgroundTextures[5] = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\back_bg.png");

	_levels[0] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Borin's Den",        NORMAL));
	_levels[1] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Viking Stronghold",  NORMAL));
	_levels[2] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("The Grasslands",     NORMAL));
	_levels[3] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Abandoned Theatre",  NORMAL));
	_levels[4] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("The Final Showdown", NORMAL));
	_levels[5] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Back",               NORMAL));

	_levelSelect = new Texture(_mgr->GetRenderer(), _headerFont->RenderText_Solid("Level Select", NORMAL));
	_lock        = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\lock.png");

	_borin = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\Borin_walk_56x56.png", 8, 1);
	_borin->SetScale(5.0f);

	_curMenuItem = 0;

	return SCREEN_LOAD_SUCCESS;
}

int LevelSelectScreen::Update(double elapsedSecs) 
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	_borin->Update(elapsedSecs);

	if (_mgr->inputManager->ActionOccurred("ARROWDOWN", Input::Pressed))
	{
		_borin->SetXAxisDirection(_borin->GetXAxisDirection() == SpriteSheet::LEFT ? SpriteSheet::RIGHT : SpriteSheet::LEFT);
		_curMenuItem++;
		if (_curMenuItem == NUM_LEVELS) _curMenuItem = 0;
	}
	else if (_mgr->inputManager->ActionOccurred("ARROWUP", Input::Pressed))
	{
		_borin->SetXAxisDirection(_borin->GetXAxisDirection() == SpriteSheet::LEFT ? SpriteSheet::RIGHT : SpriteSheet::LEFT);
		_curMenuItem--;
		if (_curMenuItem < 0) _curMenuItem = NUM_LEVELS - 1;
	}

	// We selected a menu item; do the appropriate thing
	if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
	{
		switch (_curMenuItem) 
		{
		case 0:
			if (_mgr->_unlockedLevels["den"])
			{
				_mgr->SetNextScreen("denIntro");
				return SCREEN_FINISH;
			}
			break;
		case 1:
			if (_mgr->_unlockedLevels["viking"])
			{
				_mgr->SetNextScreen("vikingIntro");
				return SCREEN_FINISH;
			}
			break;
		case 2:
			if (_mgr->_unlockedLevels["grass"])
			{
				_mgr->SetNextScreen("grassIntro");
				return SCREEN_FINISH;
			}
			break;
		case 3:
			if (_mgr->_unlockedLevels["starscape"])
			{
				_mgr->SetNextScreen("starscapeIntro");
				return SCREEN_FINISH;
			}
			break;
		case 4:
			if (_mgr->_unlockedLevels["boss"])
			{
				_mgr->SetNextScreen("bossIntro");
				return SCREEN_FINISH;
			}
			break;
		case 5:
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
	rend.Copy(*_backgroundTextures[_curMenuItem], NullOpt, NullOpt);

	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();

	for (size_t i = 0; i < NUM_LEVELS; ++i)
	{
		SDL_Color c;
		if (_curMenuItem == i)
			if (!_mgr->_unlockedLevels[LEVEL_NAMES[i]])
				c = LOCKED_AND_SELECTED;
			else
				c = SELECTED;
		else if (!_mgr->_unlockedLevels[LEVEL_NAMES[i]])
			c = LOCKED;
		else
			c = NORMAL;

		_levels[i]->SetColorMod(c.r, c.g, c.b);
		rend.Copy(*_levels[i], NullOpt, Rect(size.GetX() * 0.05f, size.GetY() * (0.25f + ((float)i * 0.1f)), _levels[i]->GetWidth(), _levels[i]->GetHeight()));

		if (!_mgr->_unlockedLevels[LEVEL_NAMES[i]])
			rend.Copy(*_lock, NullOpt, Rect(size.GetX() * 0.07f + _levels[i]->GetWidth(), size.GetY() * (0.25f + ((float)i * 0.1f)), _lock->GetWidth(), _lock->GetHeight()));
	}
	
	rend.Copy(*_levelSelect, NullOpt, Rect(size.GetX() * 0.5f - _levelSelect->GetWidth() / 2, size.GetY() * 0.10f, _levelSelect->GetWidth(), _levelSelect->GetHeight()));
	_borin->Draw(SDL2pp::Point(size.GetX() * 0.8f, size.GetY() * 0.65f));

	rend.Present();
}

// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void LevelSelectScreen::Unload() 
{
	free(_levelSelect);
	free(_lock);

	for (int i = 0; i < NUM_LEVELS; ++i)
	{
		free(_levels[i]);
		free(_backgroundTextures[i]);
	}
}
