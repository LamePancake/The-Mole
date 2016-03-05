#include "CutsceneScreen.h"
#include "GameManager.h"

using namespace SDL2pp;

const static Uint8 NOTTALKING_COLOUR[3] = { 127, 127, 127 };
const static Uint8 TALKING_COLOUR[3] = { 255, 255, 255 };

int CutsceneScreen::Load()
{
	_mgr              = GameManager::GetInstance();

	_background       = new Texture(_mgr->GetRenderer(), _backgroundPath);
	_protagDialogBox  = new Texture(_mgr->GetRenderer(), _dialogPath);
	_npcDialogBox     = new Texture(_mgr->GetRenderer(), _dialogPath);
	_protagonist      = std::make_shared<SpriteSheet>(std::move(_protagSpritePath), _protagNumFrames, _protagDuration);
	_NPC              = std::make_shared<SpriteSheet>(std::move(_npcSpritePath), _npcNumFrames, _npcDuration);
	_openingAnimation = std::make_shared<SpriteSheet>(std::move(_openingSpriteSheetPath), _openingNumFrames, _openingDuration);
	
	_font = new SDL2pp::Font(".\\Assets\\GUI\\BEBAS.ttf", 60); // SDL_ttf font

	_prevKeyState = (Uint8*)std::malloc(sizeof(Uint8) * SDL_NUM_SCANCODES);
	std::memcpy(_prevKeyState, SDL_GetKeyboardState(nullptr), sizeof(Uint8) * SDL_NUM_SCANCODES);
	return SCREEN_LOAD_SUCCESS;
}

int CutsceneScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();

	if (_currentlySpeaking == PROTAG)
		_protagonist->Update(elapsedSecs);

	if(_currentlySpeaking == NPC)
		_NPC->Update(elapsedSecs);

	_openingAnimation->Update(elapsedSecs);

	// Change the currently selected menu item
	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	if (keys[SDL_SCANCODE_SPACE] && _prevKeyState[SDL_SCANCODE_SPACE])
	{
  		_skipTimer += elapsedSecs;
		
		if (_skipTimer >= 1.5)
		{
			_mgr->SetNextScreen(_nextScreen);
			return SCREEN_FINISH;
		}
	}

	// We selected a menu item; do the appropriate thing
	if (keys[SDL_SCANCODE_RETURN] && !_prevKeyState[SDL_SCANCODE_RETURN])
	{
		_mgr->SetNextScreen(_nextScreen);
		return SCREEN_FINISH;
	}

	// Save the previous key state (temporary until InputManager actions are implemented)
	std::memcpy(_prevKeyState, keys, sizeof(Uint8) * SDL_NUM_SCANCODES);
	return SCREEN_CONTINUE;
}

void CutsceneScreen::Draw()
{
	Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();

	SDL2pp::Point dim = GameManager::GetInstance()->GetWindow().GetSize();
	
	// Render the background
	rend.Copy(*_background, SDL2pp::NullOpt, SDL2pp::NullOpt);

	// Render the protagonist and its dialog box
	SDL_Texture* protagRawTexture       = _protagonist->GetTexture().Get();
	SDL_Texture* npcRawTexture          = _NPC->GetTexture().Get();
	SDL_Texture* npcDialogRawTexture    = _npcDialogBox->Get();
	SDL_Texture* protagDialogRawTexture = _protagDialogBox->Get();

	if (_currentlySpeaking == PROTAG)
	{
		SDL_SetTextureColorMod(protagRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);
		SDL_SetTextureColorMod(npcRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
		
		SDL_SetTextureColorMod(protagDialogRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);
		SDL_SetTextureColorMod(npcDialogRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
	}
	else
	{
		SDL_SetTextureColorMod(protagRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
		SDL_SetTextureColorMod(npcRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);

		SDL_SetTextureColorMod(protagDialogRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
		SDL_SetTextureColorMod(npcDialogRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);
	}
		
	_protagonist->Draw(Rect(0, dim.GetY() * 0.75f - _protagonist->GetFrameHeight(), _protagonist->GetFrameWidth(), _protagonist->GetFrameHeight()), SpriteSheet::XAxisDirection::RIGHT, SpriteSheet::YAxisDirection::UP);
	rend.Copy(*_protagDialogBox, SDL2pp::NullOpt, Rect(0, dim.GetY() * 0.75f, dim.GetX() * 0.7f, dim.GetY() * 0.2f));
	
	//SDL2pp::Texture protagName(rend, _font->RenderText_Solid("Borin", SDL_Color{ 255, 255, 255, 255 }));
	//rend.Copy(protagName, NullOpt, Rect(0, dim.GetY() * 0.75f, dim.GetX() * 0.7f, dim.GetY() * 0.2f));

	// Render the NPC and its dialog box
	_NPC->Draw(Rect(dim.GetX() - _NPC->GetFrameWidth(), dim.GetY() * 0.25f - _NPC->GetFrameHeight(), _NPC->GetFrameWidth(), _NPC->GetFrameHeight()), SpriteSheet::XAxisDirection::LEFT, SpriteSheet::YAxisDirection::UP);
	rend.Copy(*_npcDialogBox, SDL2pp::NullOpt, Rect(dim.GetX() - dim.GetX() * 0.7f, dim.GetY() * 0.25f, dim.GetX() * 0.7f, dim.GetY() * 0.2f));
	
	//SDL2pp::Texture npcName(rend, _font->RenderText_Solid("Viking", SDL_Color{ 255, 255, 255, 255 }));
	//rend.Copy(npcName, NullOpt, Rect(dim.GetX() - dim.GetX() * 0.7f, dim.GetY() * 0.25f, dim.GetX() * 0.7f, dim.GetY() * 0.2f));

	rend.Present();
}

// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void CutsceneScreen::Unload()
{
	delete _background;
	delete _protagDialogBox;
	delete _npcDialogBox;
	delete _font;
	free(_prevKeyState);
}