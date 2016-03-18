#include "ControlsScreen.h"
#include <cstring>
#include "GameManager.h"

using namespace SDL2pp;

const static SDL_Color NORMAL = { 255, 255, 255, 255 };
const static SDL_Color SELECTED = { 255, 80, 80, 255 };

int ControlsScreen::Load()
{
	_mgr = GameManager::GetInstance();

	_font = new SDL2pp::Font(".\\Assets\\Fonts\\Exo-Regular.otf", 40);
	_headerFont = new SDL2pp::Font(".\\Assets\\Fonts\\BEBAS.ttf", 80);

	_backgroundTexture = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\den_bg.png");
	_border            = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\levelSelectBorder.png");
	_controlsImage     = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\controlScreen.png");

	_back = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Back", NORMAL));
	_back->SetColorMod(SELECTED.r, SELECTED.g, SELECTED.b);

	_controls = new Texture(_mgr->GetRenderer(), _headerFont->RenderText_Solid("Controls", NORMAL));

	return SCREEN_LOAD_SUCCESS;
}

int ControlsScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	// We selected a menu item; do the appropriate thing
	if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
	{
		_mgr->SetNextScreen("menu");
		return SCREEN_FINISH;
	}

	return SCREEN_CONTINUE;
}

void ControlsScreen::Draw()
{
	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();

	Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();
	rend.Copy(*_backgroundTexture, NullOpt, NullOpt);
	rend.Copy(*_controlsImage, NullOpt, Rect(0, size.GetY() * 0.13f, size.GetX(), size.GetY() * 0.8f));

	rend.Copy(*_border, NullOpt, Rect(0.0f, size.GetY() * 0.9f - (size.GetY() * 0.025f / 2), _back->GetWidth() + size.GetX() * 0.2f, _back->GetHeight() + size.GetY() * 0.025f));
	rend.Copy(*_back, NullOpt, Rect(size.GetX() * 0.05f, size.GetY() * 0.9f, _back->GetWidth(), _back->GetHeight()));

	rend.Copy(*_controls, NullOpt, Rect(size.GetX() * 0.02f, size.GetY() * 0.07f, _controls->GetWidth(), _controls->GetHeight()));

	rend.Present();
}

// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void ControlsScreen::Unload()
{
	delete _backgroundTexture;
	delete _back;
	delete _border;
	delete _controls;
	delete _font;
	delete _headerFont;
	delete _controlsImage;
}
