#include <cstring>
#include "GameManager.h"
#include "MenuScreen.h"


using namespace SDL2pp;

const static SDL_Color NORMAL = { 255, 255, 255, 255 };
const static SDL_Color SELECTED = { 255, 80, 80, 255 };

#define MAX_DELAY 1.2
#define VELOCITY 450.0

int MenuScreen::Load() 
{
	_mgr = GameManager::GetInstance();	
	
	_font = new SDL2pp::Font(".\\Assets\\Fonts\\Exo-Regular.otf", 50);

	_menuItems[0] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid(" Play ",     NORMAL));
	_menuItems[1] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Options", NORMAL));
	_menuItems[2] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Credits",  NORMAL));
	_menuItems[3] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid(" Quit ",     NORMAL));
    
	_background = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\den_bg.png");
	_menuBorder = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\levelSelectBorder.png");
	_controls   = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\mainMenuControls.png");
	_devLogo    = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\logo.png");
	
	_logo = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\Borin_walk_56x56.png", 8, 1);
	_logo->SetScale(6.0);

	_curMenuItem = 0;

	_borin = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\Borin_walk_56x56.png", 8, 1);
	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();
	_borinPosition = Vector2(size.GetX() * (0.14f + ((float)_curMenuItem * 0.24f)) - (_borin->GetFrameWidth() / 2), size.GetY() * 0.75f);
	_delay = false;
	_update = false;
	_timer = 0;
	_direction = 1;
	_nextScreen = "levelSelect";

    _menuTheme = new Music(".\\Assets\\Audio\\menu_theme.ogg");
    _mgr->GetMixer().PlayMusic(*_menuTheme);

	return SCREEN_LOAD_SUCCESS;
}

int MenuScreen::Update(double elapsedSecs) 
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	_logo->Update(elapsedSecs);
	_borin->Update(elapsedSecs);

	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();

	if (_update)
	{
		_borinPosition.SetX(_borinPosition.GetX() + VELOCITY * _direction * elapsedSecs);
		if (_direction == 1 && _borinPosition.GetX() >= size.GetX() * (0.14f + ((float)_curMenuItem * 0.24f)) - (_borin->GetFrameWidth() / 2))
		{
			_update = false;
			_borinPosition = Vector2(size.GetX() * (0.14f + ((float)_curMenuItem * 0.24f)) - (_borin->GetFrameWidth() / 2), size.GetY() * 0.75f);
		}
		else if (_direction == -1 && _borinPosition.GetX() <= size.GetX() * (0.14f + ((float)_curMenuItem * 0.24f)) - (_borin->GetFrameWidth() / 2))
		{
			_update = false;
			_borinPosition = Vector2(size.GetX() * (0.14f + ((float)_curMenuItem * 0.24f)) - (_borin->GetFrameWidth() / 2), size.GetY() * 0.75f);
		}
	}

	if (_delay && _timer < MAX_DELAY)
	{
		_borinPosition.SetX(_borinPosition.GetX() + VELOCITY * _direction * elapsedSecs);
		_timer += elapsedSecs;
	}
	else if (_timer >= MAX_DELAY)
	{
		if (_nextScreen == "exit")
			exit(0);

		_mgr->GetMixer().HaltMusic();
		_mgr->SetNextScreen(_nextScreen);
		return SCREEN_FINISH;
	}

	// Change the currently selected menu item
	if (!_delay && _mgr->inputManager->ActionOccurred("ARROWRIGHT", Input::Pressed)
		|| _mgr->inputManager->ActionOccurred("RIGHT", Input::Pressed)) 
	{
		_soundBank.PlaySound("select");
		_update = true;
		_direction = 1;
		_curMenuItem++;
		_borin->SetXAxisDirection(SpriteSheet::RIGHT);
		if (_curMenuItem == NUM_MENU_ITEMS) _curMenuItem = NUM_MENU_ITEMS - 1;
	}
	else if (!_delay && _mgr->inputManager->ActionOccurred("ARROWLEFT", Input::Pressed)
		|| _mgr->inputManager->ActionOccurred("LEFT", Input::Pressed))
	{
		_soundBank.PlaySound("select");
		_update = true;
		_direction = -1;
		_curMenuItem--;
		_borin->SetXAxisDirection(SpriteSheet::LEFT);
		if (_curMenuItem < 0) _curMenuItem = 0;
	}

	// We selected a menu item; do the appropriate thing
	if (!_delay &&  (_direction == 1 && _borinPosition.GetX() >= size.GetX() * (0.14f + ((float)_curMenuItem * 0.24f)) - (_borin->GetFrameWidth() / 2)
		|| _direction == -1 && _borinPosition.GetX() <= size.GetX() * (0.14f + ((float)_curMenuItem * 0.24f)) - (_borin->GetFrameWidth() / 2))
		&& _mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
	{
		_soundBank.PlaySound("accept");
		switch (_curMenuItem) 
		{
		case 0:
			 _borin->SetXAxisDirection(SpriteSheet::LEFT);
			 _direction = -1;
			 _nextScreen = "levelSelect";
			 _delay = true;
			 break;
		case 1:
			_borin->SetXAxisDirection(SpriteSheet::LEFT);
			_direction = -1;
			_nextScreen = "options";
			_delay = true;
			break;
		case 2:
			_borin->SetXAxisDirection(SpriteSheet::RIGHT);
			_direction = 1;
			_nextScreen = "credits";
			_delay = true;
			break;
		case 3:
			_borin->SetXAxisDirection(SpriteSheet::RIGHT);
			_direction = 1;
			_nextScreen = "exit";
			_delay = true;
			break;
		}
	}

	return SCREEN_CONTINUE;
}

void MenuScreen::Draw() 
{
	Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();
	rend.Copy(*_background, NullOpt, NullOpt);

	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();

	for (size_t i = 0; i < NUM_MENU_ITEMS; ++i)
	{
		float scaleFactor = 0.0f;
		SDL_Color c;
		if (_curMenuItem == i)
		{
			c = SELECTED;
			scaleFactor = size.GetY() * 0.04f;
		}
		else
		{
			c = NORMAL;
		}
			
		_menuItems[i]->SetColorMod(c.r, c.g, c.b);
		rend.Copy(*_menuBorder, Rect(0, 0, 1, 1), Rect(size.GetX() * (0.04f + ((float)i * 0.24f)), size.GetY() * 0.88f - scaleFactor, size.GetX() * 0.2, size.GetY() * 0.25f));
		rend.Copy(*_menuItems[i], NullOpt, Rect(size.GetX() * (0.05f + (float)i * 0.24f), size.GetY() * 0.88f - scaleFactor, size.GetX() * 0.18, size.GetY() * 0.1f));
	}

	_logo->Draw(SDL2pp::Point(size.GetX() * 0.6, size.GetY() * 0.55));
	rend.Copy(*_controls, NullOpt, Rect(size.GetX() * 0.60f, 0, size.GetX() * 0.40f, size.GetY() * 0.1f));
	_borin->Draw(SDL2pp::Point((int)_borinPosition.GetX(), (int)_borinPosition.GetY()));

	rend.Copy(*_devLogo, NullOpt, Rect(size.GetX() * 0.01f, size.GetY() * 0.01f, size.GetX() * 0.1f, size.GetY() * 0.13f));

	rend.Present();
}

// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void MenuScreen::Unload() 
{
	_curMenuItem = 0;
	_delay = false;
	_update = false;
	_timer = 0;
	_direction = 1;
	_nextScreen = "levelSelect";

	delete _devLogo;
    delete _menuTheme;
	delete _background;
	delete _menuBorder;
	delete _controls;
	delete _font;

	for (int i = 0; i < NUM_MENU_ITEMS; ++i)
	{
		delete _menuItems[i];
	}
}


SoundEffectBank & MenuScreen::GetSoundBank()
{
	return _soundBank;
}

