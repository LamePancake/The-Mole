#include "LevelSelectScreen.h"
#include <cstring>
#include "GameManager.h"

using namespace SDL2pp;

const static std::string LEVEL_NAMES[6] = { "den", "viking", "grass", "starscape", "boos", "back" };

const static SDL_Color NORMAL   = { 255, 255, 255, 255 };
const static SDL_Color LOCKED   = { 80, 80, 80, 255 };
const static SDL_Color SELECTED = { 255, 80, 80, 255 };
const static SDL_Color LOCKED_AND_SELECTED = { 140, 80, 80, 255 };

#define MAX_DELAY 1.5
#define VELOCITY 300.0

int LevelSelectScreen::Load() 
{
	_mgr = GameManager::GetInstance();

	_mgr->ReadFile(".\\Assets\\Levels\\level_unlocks.txt");

	_font = new SDL2pp::Font(".\\Assets\\GUI\\Exo-Regular.otf", 50);
	_headerFont = new SDL2pp::Font(".\\Assets\\GUI\\BEBAS.ttf", 80);

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

	_levelSelect  = new Texture(_mgr->GetRenderer(), _headerFont->RenderText_Solid("Level Select", NORMAL));
	_lock         = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\lock.png");
	_optionBorder = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\levelSelectBorder.png");
	_controls     = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\levelSelectControls.png");

	_borin = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\Borin_walk_56x56.png", 8, 1);
	_borin->SetScale(5.0f);
	
	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();
	_borinPosition = Vector2(size.GetX() * 0.8f, size.GetY() * 0.6f);

	_delay = false;
	_timer = 0;
	_nextScreen = "menu";
	_curMenuItem = 0;

	return SCREEN_LOAD_SUCCESS;
}

int LevelSelectScreen::Update(double elapsedSecs) 
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	_borin->Update(elapsedSecs);

	if (_delay && _timer < MAX_DELAY)
	{
		_borin->SetXAxisDirection(SpriteSheet::RIGHT);
		_borinPosition.SetX(_borinPosition.GetX() + VELOCITY * elapsedSecs);
		_timer += elapsedSecs;
	}
	else if (_timer >= MAX_DELAY)
	{
		_mgr->SetNextScreen(_nextScreen);
		return SCREEN_FINISH;
	}

	if (_mgr->inputManager->ActionOccurred("ARROWDOWN", Input::Pressed) || _mgr->inputManager->ActionOccurred("DOWN", Input::Pressed))
	{
		_borin->SetXAxisDirection(_borin->GetXAxisDirection() == SpriteSheet::LEFT ? SpriteSheet::RIGHT : SpriteSheet::LEFT);
		_curMenuItem++;
		if (_curMenuItem == NUM_LEVELS) _curMenuItem = 0;
	}
	else if (_mgr->inputManager->ActionOccurred("ARROWUP", Input::Pressed) || _mgr->inputManager->ActionOccurred("UP", Input::Pressed))
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
				_nextScreen = "den";
				_delay = true;
			}
			break;
		case 1:
			if (_mgr->_unlockedLevels["viking"])
			{
				_nextScreen = "viking";
				_delay = true;
			}
			break;
		case 2:
			if (_mgr->_unlockedLevels["grass"])
			{
				_nextScreen = "grass";
				_delay = true;
			}
			break;
		case 3:
			if (_mgr->_unlockedLevels["starscape"])
			{
				_nextScreen = "starscape";
				_delay = true;
			}
			break;
		case 4:
			if (_mgr->_unlockedLevels["boss"])
			{
				_nextScreen = "boss";
				_delay = true;
			}
			break;
		case 5:
			_nextScreen = "menu";
			_delay = true;
			break;
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
		int scaleFactor = 0;
		SDL_Color c;
		if (_curMenuItem == i)
		{
			if (!_mgr->_unlockedLevels[LEVEL_NAMES[i]])
				c = LOCKED_AND_SELECTED;
			else
				c = SELECTED;
			scaleFactor = 10;
		}
		else if (!_mgr->_unlockedLevels[LEVEL_NAMES[i]])
			c = LOCKED;
		else
			c = NORMAL;

		rend.Copy(*_optionBorder, NullOpt, Rect(0.0f, size.GetY() * (0.25f + ((float)i * 0.12f)), _levels[i]->GetWidth() + size.GetX() * 0.17f, _levels[i]->GetHeight() + scaleFactor));

		_levels[i]->SetColorMod(c.r, c.g, c.b);
		rend.Copy(*_levels[i], NullOpt, Rect(size.GetX() * 0.05f, size.GetY() * (0.25f + ((float)i * 0.12f)), _levels[i]->GetWidth() + scaleFactor, _levels[i]->GetHeight() + scaleFactor));

		if (!_mgr->_unlockedLevels[LEVEL_NAMES[i]])
			rend.Copy(*_lock, NullOpt, Rect(size.GetX() * 0.07f + _levels[i]->GetWidth(), size.GetY() * (0.26f + ((float)i * 0.12f)), _lock->GetWidth() + scaleFactor, _lock->GetHeight() + scaleFactor));
	}
	
	rend.Copy(*_levelSelect, NullOpt, Rect(size.GetX() * 0.02f, size.GetY() * 0.07f, _levelSelect->GetWidth(), _levelSelect->GetHeight()));
	_borin->Draw(SDL2pp::Point((int)_borinPosition.GetX(), (int)_borinPosition.GetY()));

	rend.Copy(*_controls, NullOpt, Rect(size.GetX() * 0.60f, 0, size.GetX() * 0.40f, size.GetY() * 0.1f));

	rend.Present();
}

// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void LevelSelectScreen::Unload() 
{
	_delay = false;
	_timer = 0;
	_nextScreen = "menu";
	_curMenuItem = 0;

	free(_levelSelect);
	free(_lock);
	free(_optionBorder);
	free(_controls);

	for (int i = 0; i < NUM_LEVELS; ++i)
	{
		free(_levels[i]);
		free(_backgroundTextures[i]);
	}
}
