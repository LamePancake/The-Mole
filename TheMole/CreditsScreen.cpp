#include "CreditsScreen.h"
#include <cstring>
#include "GameManager.h"

using namespace SDL2pp;

const static SDL_Color NORMAL = { 255, 255, 255, 255 };
const static SDL_Color SELECTED = { 255, 80, 80, 255 };

#define MAX_DELAY 1.2
#define VELOCITY 800.0

int CreditsScreen::Load()
{
	_mgr = GameManager::GetInstance();

	_font = new SDL2pp::Font(".\\Assets\\Fonts\\Exo-Regular.otf", 40);
	_headerFont = new SDL2pp::Font(".\\Assets\\Fonts\\BEBAS.ttf", 80);

	_backgroundTexture = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\den_bg.png");
	_border            = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\levelSelectBorder.png");
	_controls          = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\creditsControls.png");

	_developers[0] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Matt Ku - Programmer, Designer",          NORMAL));
	_developers[1] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Shane Spoor - Programmer, Designer",      NORMAL));
	_developers[2] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Tim Wang - Programmer, Designer, Artist", NORMAL));
	_developers[3] = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Trevor Ware - Programmer, Designer",      NORMAL));

	_back    = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Back", NORMAL));
	_back->SetColorMod(SELECTED.r, SELECTED.g, SELECTED.b);

	_credits = new Texture(_mgr->GetRenderer(), _headerFont->RenderText_Solid("Credits", NORMAL));

	_chicken = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\Chicken_walk.png", 8, 1);
	_chicken->SetScale(2);

	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();
	_chickenPosition = Vector2(size.GetX() * 0.21f, size.GetY() * 0.9f);

	_delay = false;
	_timer = 0;
	_direction = 1;

	return SCREEN_LOAD_SUCCESS;
}

int CreditsScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();

	_chicken->Update(elapsedSecs);

	if (_delay && _timer < MAX_DELAY)
	{
		_timer += elapsedSecs;
	}
	else if (_timer >= MAX_DELAY)
	{
		_mgr->SetNextScreen("menu");
		return SCREEN_FINISH;
	}

	_chickenPosition.SetX(_chickenPosition.GetX() + VELOCITY * _direction* elapsedSecs);
	if (_chickenPosition.GetX() > size.GetX() * 0.8f && !_delay)
	{
		_chicken->SetXAxisDirection(SpriteSheet::LEFT);
		_direction = -1;
	}
	if (_chickenPosition.GetX() < size.GetX() * 0.2f  && !_delay)
	{
		_chicken->SetXAxisDirection(SpriteSheet::RIGHT);
		_direction = 1;
	}

	// We selected a menu item; do the appropriate thing
	if (!_delay && _mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
	{
		_soundBank.PlaySound("accept");
		_delay = true;
	}

	return SCREEN_CONTINUE;
}

void CreditsScreen::Draw()
{
	Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();
	rend.Copy(*_backgroundTexture, NullOpt, NullOpt);

	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();

	for (size_t i = 0; i < NUM_DEVELOPERS; ++i)
	{
		rend.Copy(*_border, NullOpt, Rect(0.0f, size.GetY() * (0.25f + ((float)i * 0.13f)) - (size.GetY() * 0.025f / 2), _developers[i]->GetWidth() + size.GetX() * 0.2f, _developers[i]->GetHeight() + size.GetY() * 0.025f));
		rend.Copy(*_developers[i], NullOpt, Rect(size.GetX() * 0.05f, size.GetY() * (0.25f + ((float)i * 0.13f)), _developers[i]->GetWidth(), _developers[i]->GetHeight()));
	}

	rend.Copy(*_border, NullOpt, Rect(0.0f, size.GetY() * (0.25f + ((float)NUM_DEVELOPERS * 0.13f)) - (size.GetY() * 0.025f / 2), _back->GetWidth() + size.GetX() * 0.2f, _back->GetHeight() + size.GetY() * 0.025f));
	rend.Copy(*_back, NullOpt, Rect(size.GetX() * 0.05f, size.GetY() * (0.25f + ((float)NUM_DEVELOPERS * 0.13f)), _back->GetWidth(), _back->GetHeight()));

	rend.Copy(*_border, Rect(0, 0, 1, 1), Rect(0.0f, size.GetY() * 0.075f, size.GetX(), _credits->GetHeight()));
	rend.Copy(*_credits, NullOpt, Rect(size.GetX() * 0.02f, size.GetY() * 0.07f, _credits->GetWidth(), _credits->GetHeight()));
	rend.Copy(*_controls, NullOpt, Rect(size.GetX() * 0.7f, 0, size.GetX() * 0.40f, size.GetY() * 0.1f));

	_chicken->Draw(SDL2pp::Point((int)_chickenPosition.GetX(), (int)_chickenPosition.GetY()));

	rend.Present();
}
// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void CreditsScreen::Unload()
{
	_direction = 1;
	_delay = false;
	_timer = 0;

	delete _backgroundTexture;
	delete _back;
	delete _border;
	delete _controls;
	delete _font;
	delete _headerFont;
	delete _credits;

	for (int i = 0; i < NUM_DEVELOPERS; ++i)
	{
		delete _developers[i];
	}
}

SoundEffectBank & CreditsScreen::GetSoundBank()
{
	return _soundBank;
}