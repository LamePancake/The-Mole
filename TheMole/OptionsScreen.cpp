#include "OptionsScreen.h"
#include <cstring>
#include "GameManager.h"

using namespace SDL2pp;

const static SDL_Color NORMAL = { 255, 255, 255, 255 };
const static SDL_Color SELECTED = { 255, 80, 80, 255 };
const static SDL_Color WARNING = { 255, 250, 80, 255 };

int OptionsScreen::Load()
{
	_mgr = GameManager::GetInstance();

	_font = new SDL2pp::Font(".\\Assets\\Fonts\\Exo-Regular.otf", 50);
	_headerFont = new SDL2pp::Font(".\\Assets\\Fonts\\BEBAS.ttf", 80);
	_warningFont = new SDL2pp::Font(".\\Assets\\Fonts\\Exo-Regular.otf", 35);

	_backgroundTexture = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\den_bg.png");

	_optionsText = new Texture(_mgr->GetRenderer(), _headerFont->RenderText_Solid("Options", NORMAL));
	_areYouSure = new Texture(_mgr->GetRenderer(), _warningFont->RenderText_Solid("Are you sure you want to erase all data?", WARNING));
	_dataHasBeenCleared = new Texture(_mgr->GetRenderer(), _warningFont->RenderText_Solid("Saved data has been cleared.", NORMAL));
	_border = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\levelSelectBorder.png");
	_controls = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\levelSelectControls.png");

	_options[0] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Controls", NORMAL));
	_options[1] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Clear Saved Data", NORMAL));
	_options[2] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Back", NORMAL));

	_yesOrNo[0] = new Texture(_mgr->GetRenderer(), _warningFont->RenderText_Solid("Yes", NORMAL));
	_yesOrNo[1] = new Texture(_mgr->GetRenderer(), _warningFont->RenderText_Solid("No", NORMAL));

	_curMenuItem = 0;

	_confirmClearing = false;
	_dataErased = false;
	_curEraseMenuItem = 1;

	return SCREEN_LOAD_SUCCESS;
}

int OptionsScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	if (!_confirmClearing && _mgr->inputManager->ActionOccurred("ARROWDOWN", Input::Pressed) || _mgr->inputManager->ActionOccurred("DOWN", Input::Pressed))
	{
		_soundBank.PlaySound("select");
		_curMenuItem++;
		if (_curMenuItem == NUM_OPTIONS) _curMenuItem = 0;
	}
	else if (!_confirmClearing && _mgr->inputManager->ActionOccurred("ARROWUP", Input::Pressed) || _mgr->inputManager->ActionOccurred("UP", Input::Pressed))
	{
		_soundBank.PlaySound("select");
		_curMenuItem--;
		if (_curMenuItem < 0) _curMenuItem = NUM_OPTIONS - 1;
	}

	// We selected a menu item; do the appropriate thing
	if (!_confirmClearing && _mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
	{
		switch (_curMenuItem)
		{
		case 0:
			_soundBank.PlaySound("accept");
			_mgr->SetNextScreen("controls");
			return SCREEN_FINISH;
		case 1:
			_soundBank.PlaySound("warning");
			_confirmClearing = true;
			break;
		case 2:
			_soundBank.PlaySound("accept");
			_mgr->SetNextScreen("menu");
			return SCREEN_FINISH;
		}
	}
	else if (_confirmClearing && _mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
	{
		_soundBank.PlaySound("accept");
		switch (_curEraseMenuItem)
		{
		case 0:
			_mgr->ClearSavedData();
			_confirmClearing = false;
			_dataErased = true;
			break;
		case 1:
			_confirmClearing = false;
			_dataErased = false;
			break;
		}
	}

	if (_confirmClearing && _mgr->inputManager->ActionOccurred("ARROWRIGHT", Input::Pressed) || _mgr->inputManager->ActionOccurred("RIGHT", Input::Pressed))
	{
		_soundBank.PlaySound("select");
		_curEraseMenuItem++;
		if (_curEraseMenuItem == 2) _curEraseMenuItem = 0;
	}
	else if (_confirmClearing && _mgr->inputManager->ActionOccurred("ARROWLEFT", Input::Pressed) || _mgr->inputManager->ActionOccurred("LEFT", Input::Pressed))
	{
		_soundBank.PlaySound("select");
		_curEraseMenuItem--;
		if (_curEraseMenuItem < 0) _curEraseMenuItem = 2 - 1;
	}

	return SCREEN_CONTINUE;
}

void OptionsScreen::Draw()
{
	Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();
	rend.Copy(*_backgroundTexture, NullOpt, NullOpt);

	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();

	for (size_t i = 0; i < NUM_OPTIONS; ++i)
	{
		int scaleFactor = 0;
		SDL_Color c;
		if (_curMenuItem == i)
		{
			c = SELECTED;
			scaleFactor = 15;
		}
		else
			c = NORMAL;

		rend.Copy(*_border, NullOpt, Rect(0.0f, size.GetY() * (0.25f + ((float)i * 0.12f)) - (scaleFactor / 2), _options[i]->GetWidth() + size.GetX() * 0.17f, _options[i]->GetHeight() + scaleFactor));

		_options[i]->SetColorMod(c.r, c.g, c.b);
		rend.Copy(*_options[i], NullOpt, Rect(size.GetX() * 0.05f, size.GetY() * (0.25f + ((float)i * 0.12f)) - (scaleFactor / 2), _options[i]->GetWidth() + scaleFactor, _options[i]->GetHeight() + scaleFactor));
	}

	rend.Copy(*_border, Rect(0, 0, 1, 1), Rect(0.0f, size.GetY() * 0.075f, size.GetX(), _optionsText->GetHeight()));
	rend.Copy(*_optionsText, NullOpt, Rect(size.GetX() * 0.02f, size.GetY() * 0.07f, _optionsText->GetWidth(), _optionsText->GetHeight()));
	rend.Copy(*_controls, NullOpt, Rect(size.GetX() * 0.60f, 0, size.GetX() * 0.40f, size.GetY() * 0.1f));

	if (_confirmClearing)
	{
		_border->SetAlphaMod(80);
		rend.Copy(*_border, SDL2pp::Rect(0, 0, 1, 1), SDL2pp::NullOpt);
		_border->SetAlphaMod(255);

		rend.Copy(*_border, SDL2pp::Rect(0, 0, 1, 1), SDL2pp::Rect(size.GetX() * 0.5 - (_areYouSure->GetWidth()  / 2.0) - 25, size.GetY() * 0.5 - (_areYouSure->GetHeight() / 2.0), _areYouSure->GetWidth() + 50, _areYouSure->GetHeight() + _yesOrNo[0]->GetHeight()));
		rend.Copy(*_areYouSure, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.5 - (_areYouSure->GetWidth() / 2), size.GetY() * 0.5f - (_areYouSure->GetHeight() / 2), _areYouSure->GetWidth(), _areYouSure->GetHeight()));
	
		for (size_t i = 0; i < 2; ++i)
		{
			SDL_Color c;
			if (_curEraseMenuItem == i)
				c = SELECTED;
			else
				c = NORMAL;

			_yesOrNo[i]->SetColorMod(c.r, c.g, c.b);
			float width = (_yesOrNo[0]->GetWidth() + _yesOrNo[1]->GetWidth() + (size.GetX() * 0.2)) / 2.0;
			rend.Copy(*_yesOrNo[i], NullOpt, Rect(size.GetX() * (0.5 + (0.2 * i)) - width, size.GetY() * 0.5f + (_areYouSure->GetHeight() / 2), _yesOrNo[i]->GetWidth(), _yesOrNo[i]->GetHeight()));
		}
	}

	if(_dataErased)
		rend.Copy(*_dataHasBeenCleared, NullOpt, Rect(size.GetX() * 0.98f - _dataHasBeenCleared->GetWidth(), size.GetY() * 0.92f, _dataHasBeenCleared->GetWidth(), _dataHasBeenCleared->GetHeight()));
		
	rend.Present();
}

// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void OptionsScreen::Unload()
{
	_curEraseMenuItem = 1;
	_curMenuItem = 0;
	_confirmClearing = false;
	_dataErased = false;

	delete _optionsText;
	delete _areYouSure;
	delete _dataHasBeenCleared;

	delete _border;
	delete _controls;

	delete _font;
	delete _headerFont;
	delete _warningFont;

	delete _backgroundTexture;

	for (int i = 0; i < NUM_OPTIONS; ++i)
	{
		delete _options[i];
	}

	delete _yesOrNo[0];
	delete _yesOrNo[1];
}

SoundEffectBank & OptionsScreen::GetSoundBank()
{
	return _soundBank;
}