#include "CutsceneScreen.h"
#include "GameManager.h"

using namespace SDL2pp;

#define SKIP_TIME 1.5
const static Uint8 NOTTALKING_COLOUR[3] = { 127, 127, 127 };
const static Uint8 TALKING_COLOUR[3] = { 255, 255, 255 };

int CutsceneScreen::Load()
{
	_mgr              = GameManager::GetInstance();

	_background       = new Texture(_mgr->GetRenderer(), _backgroundPath);
	_protagDialogBox  = new Texture(_mgr->GetRenderer(), _dialogBoxPath);
	_npcDialogBox     = new Texture(_mgr->GetRenderer(), _dialogBoxPath);
	_protagonist      = std::make_shared<SpriteSheet>(std::move(_protagSpritePath), _protagNumFrames, _protagDuration);
	_NPC              = std::make_shared<SpriteSheet>(std::move(_npcSpritePath), _npcNumFrames, _npcDuration);
	_openingAnimation = std::make_shared<SpriteSheet>(std::move(_openingSpriteSheetPath), _openingNumFrames, _openingDuration);
	
	_font = new SDL2pp::Font(".\\Assets\\GUI\\BEBAS.ttf", 20); // SDL_ttf font

	_dialog = OpenDialog(_dialogFilePath);
	UpdateDialog();

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
		
		if (_skipTimer >= SKIP_TIME)
		{
			_skipTimer = 0;
			_mgr->SetNextScreen(_nextScreen);
			return SCREEN_FINISH;
		}
	}
	else
	{
		_skipTimer = 0;
	}

	// We selected a menu item; do the appropriate thing
	if (keys[SDL_SCANCODE_RETURN] && !_prevKeyState[SDL_SCANCODE_RETURN])
	{
		_dialogIndex++;

		if (_dialogIndex >= _dialog.size())
		{
			_dialogIndex = 0;
			_currentNPCDialog = " ";
			_currentProtagDialog = " ";
			_mgr->SetNextScreen(_nextScreen);
			return SCREEN_FINISH;
		}

		UpdateDialog();
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
	
	SDL2pp::Texture protagHeader(rend, _font->RenderText_Solid("Borin", SDL_Color{ 255, 255, 255, 255 }));
	rend.Copy(protagHeader, NullOpt, Rect(dim.GetX() * 0.06f, dim.GetY() * 0.78f, dim.GetX() * 0.5f / (protagHeader.GetWidth() / protagHeader.GetHeight()), dim.GetY() * 0.03f / (protagHeader.GetWidth() / protagHeader.GetHeight())));

	SDL2pp::Texture protagDialog(rend, _font->RenderText_Solid(_currentProtagDialog, SDL_Color{ 255, 255, 255, 255 }));
	rend.Copy(protagDialog, NullOpt, Rect(dim.GetX() * 0.06f, dim.GetY() * 0.78f + protagHeader.GetHeight(), dim.GetX() * 0.5f, dim.GetY() * 0.1f));

	// Render the NPC and its dialog box
	_NPC->Draw(Rect(dim.GetX() - _NPC->GetFrameWidth(), dim.GetY() * 0.25f - _NPC->GetFrameHeight(), _NPC->GetFrameWidth(), _NPC->GetFrameHeight()), SpriteSheet::XAxisDirection::LEFT, SpriteSheet::YAxisDirection::UP);
	rend.Copy(*_npcDialogBox, SDL2pp::NullOpt, Rect(dim.GetX() - dim.GetX() * 0.7f, dim.GetY() * 0.25f, dim.GetX() * 0.7f, dim.GetY() * 0.2f));
	
	SDL2pp::Texture npcDialog(rend, _font->RenderText_Solid(_currentNPCDialog, SDL_Color{ 255, 255, 255, 255 }));
	rend.Copy(npcDialog, NullOpt, Rect(dim.GetX() - dim.GetX() * 0.85f, dim.GetY() * 0.35f, npcDialog.GetWidth() *  dim.GetX() * 0.7f, npcDialog.GetHeight() * dim.GetY() * 0.2f));

	// Draw skip prompt
	SDL2pp::Texture holdToSkip(rend, _font->RenderText_Solid("Press SPACE to Skip", SDL_Color{ 255, 255, 255, 255 }));
	rend.Copy(holdToSkip, NullOpt, Rect(dim.GetX() * 0.97f - holdToSkip.GetWidth(), dim.GetY()  * 0.97f - holdToSkip.GetHeight(), holdToSkip.GetWidth(), holdToSkip.GetHeight()));
	rend.FillRect(Rect(dim.GetX() * 0.97f - holdToSkip.GetWidth(), dim.GetY()  * 0.97f, holdToSkip.GetWidth() * _skipTimer / SKIP_TIME, holdToSkip.GetHeight() * 0.3f));

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

std::vector<std::string> CutsceneScreen::OpenDialog(std::string dialogFilePath)
{
	std::string line; // Stores a line to be read
	std::ifstream inFile(dialogFilePath.c_str());
	std::vector<std::string> dialog;

	while (std::getline(inFile, line))
	{
		dialog.push_back(line);
	}

	inFile.close();
	return dialog;
}

void CutsceneScreen::UpdateDialog()
{
	std::string firstWord;
	std::istringstream iss{ _dialog[_dialogIndex] };
	iss >> firstWord;

	if (firstWord == "Borin:")
	{
		_currentProtagDialog = _dialog[_dialogIndex];
		_currentlySpeaking = PROTAG;
	}
	else
	{
		_currentNPCDialog = _dialog[_dialogIndex];
		_currentlySpeaking = NPC;
	}
}
