#include "CutsceneScreen.h"
#include "GameManager.h"
#include <time.h>

using namespace SDL2pp;

#define SKIP_TIME 1.1
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
	_nextDialogProtag = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\dialog_enter.png", 4, 0.5f);
	_nextDialogNPC    = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\dialog_enter.png", 4, 0.5f);

	_promptFont = new SDL2pp::Font(".\\Assets\\Fonts\\BEBAS.ttf", 20);
	_dialogFont = new SDL2pp::Font(".\\Assets\\Fonts\\Exo-Regular.otf", 45);
	_headerFont = new SDL2pp::Font(".\\Assets\\Fonts\\BEBAS.ttf", 25);

	_dialog = OpenDialog(_dialogFilePath);
	UpdateDialog();

	int idx = rand() % 3;
	std::string key = _currentlySpeaking == PROTAG ? _protagVoices[idx] : _npcVoices[idx];
	_soundBank.PlaySound(key);

	_NPC->SetScale(0.8);

	srand(time(NULL));
	return SCREEN_LOAD_SUCCESS;
}

int CutsceneScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	if (_currentlySpeaking == PROTAG)
	{
		_protagonist->Update(elapsedSecs);
		_nextDialogProtag->Update(elapsedSecs);
	}
	else
	{
		_NPC->Update(elapsedSecs);
		_nextDialogNPC->Update(elapsedSecs);
	}
		
	//_openingAnimation->Update(elapsedSecs);

	if (_mgr->inputManager->ActionOccurred("SKIP", Input::Held))
	{
  		_skipTimer += elapsedSecs;
		
		if (_skipTimer >= SKIP_TIME)
		{
			_soundBank.PlaySound("accept");
			_currentProtagDialog = " ";
			_mgr->SetNextScreen(_nextScreen);
			return SCREEN_FINISH;
		}
	}
	else
	{
		_skipTimer = 0;
	}

	// We selected a menu item; do the appropriate thing
	if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
	{
		_soundBank.PlaySound("select");
		_dialogIndex++;

		if (_dialogIndex >= _dialog.size())
		{
			_mgr->SetNextScreen(_nextScreen);
			return SCREEN_FINISH;
		}

		UpdateDialog();

		int idx = rand() % 3;
		std::string key = _currentlySpeaking == PROTAG ? _protagVoices[idx] : _npcVoices[idx];
		_soundBank.PlaySound(key);
	}

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
	SDL_Texture* protagRawTexture          = _protagonist->GetTexture().Get();
	SDL_Texture* npcRawTexture             = _NPC->GetTexture().Get();
	SDL_Texture* npcDialogBoxRawTexture    = _npcDialogBox->Get();
	SDL_Texture* protagDialogBoxRawTexture = _protagDialogBox->Get();

	SDL2pp::Texture npcHeader(rend, _headerFont->RenderText_Solid(_npcName, SDL_Color{ 255, 255, 255, 255 }));
	SDL2pp::Texture protagHeader(rend, _headerFont->RenderText_Solid(_protagName, SDL_Color{ 255, 255, 255, 255 }));
	SDL2pp::Texture protagDialog(rend, _dialogFont->RenderText_Solid(_currentProtagDialog, SDL_Color{ 255, 255, 255, 255 }));
	SDL2pp::Texture npcDialog(rend, _dialogFont->RenderText_Solid(_currentNPCDialog, SDL_Color{ 255, 255, 255, 255 }));

	SDL_Texture* npcHeaderRawTexture    = npcHeader.Get();
	SDL_Texture* protagHeaderRawTexture = protagHeader.Get();
	SDL_Texture* protagDialogRawTexture = protagDialog.Get();
	SDL_Texture* npcDialogRawTexture    = npcDialog.Get();

	SDL_Texture* npcNextRawTexture      = _nextDialogNPC->GetTexture().Get();
	SDL_Texture* protagNextRawTexture   = _nextDialogProtag->GetTexture().Get();

	if (_currentlySpeaking == PROTAG)
	{
		_protagonist->SetColourMod({ TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2], 255 });
		SDL_SetTextureColorMod(protagRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);
		SDL_SetTextureColorMod(protagDialogBoxRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);
		SDL_SetTextureColorMod(protagDialogRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);
		SDL_SetTextureColorMod(protagHeaderRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);
		SDL_SetTextureColorMod(protagNextRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);

		_NPC->SetColourMod({ NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2], 255 });
		SDL_SetTextureColorMod(npcRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
		SDL_SetTextureColorMod(npcDialogBoxRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
		SDL_SetTextureColorMod(npcHeaderRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
		SDL_SetTextureColorMod(npcDialogRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
		SDL_SetTextureColorMod(npcNextRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
	}
	else
	{
		_NPC->SetColourMod({ TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2], 255 });
		SDL_SetTextureColorMod(npcDialogBoxRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);
		SDL_SetTextureColorMod(npcHeaderRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);
		SDL_SetTextureColorMod(npcDialogRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);
		SDL_SetTextureColorMod(npcNextRawTexture, TALKING_COLOUR[0], TALKING_COLOUR[1], TALKING_COLOUR[2]);

		_protagonist->SetColourMod({ NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2], 255 });
		SDL_SetTextureColorMod(protagDialogBoxRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
		SDL_SetTextureColorMod(protagDialogRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
		SDL_SetTextureColorMod(protagHeaderRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
		SDL_SetTextureColorMod(protagNextRawTexture, NOTTALKING_COLOUR[0], NOTTALKING_COLOUR[1], NOTTALKING_COLOUR[2]);
	}
		
	// Render the protagonist dialog, sprite, and header
	SDL2pp::Point protagPos(0, dim.GetY() * 0.75f - _protagonist->GetFrameHeight());
	_protagonist->Draw(protagPos);
	rend.Copy(*_protagDialogBox, SDL2pp::NullOpt, Rect(0, dim.GetY() * 0.75f, dim.GetX() * 0.7f, dim.GetY() * 0.2f));

	rend.Copy(protagHeader, NullOpt, Rect(dim.GetX() * 0.05f, dim.GetY() * 0.78f, protagHeader.GetWidth(), protagHeader.GetHeight()));
	rend.Copy(protagDialog, NullOpt, Rect(dim.GetX() * 0.05f, dim.GetY() * 0.78f + protagHeader.GetHeight(), protagDialog.GetWidth(), protagDialog.GetHeight()));

	SDL2pp::Point nextDialogProtagPos(dim.GetX() * 0.605f, dim.GetY() * 0.835f);
	_nextDialogProtag->SetScale(0.35f);
	_nextDialogProtag->Draw(nextDialogProtagPos);

	// Render the NPC dialog, sprite, and header
	SDL2pp::Point NPCPos(dim.GetX() - _NPC->GetFrameWidth(), dim.GetY() * 0.30 - _NPC->GetFrameHeight());
	_NPC->Draw(NPCPos);
	rend.Copy(*_npcDialogBox, SDL2pp::NullOpt, Rect(dim.GetX() * 0.3f, dim.GetY() * 0.30f, dim.GetX() * 0.7f, dim.GetY() * 0.2f));

	rend.Copy(npcHeader, NullOpt, Rect(dim.GetX() * 0.35f, dim.GetY() * 0.33f, npcHeader.GetWidth(), npcHeader.GetHeight()));
	rend.Copy(npcDialog, NullOpt, Rect(dim.GetX() * 0.35f, dim.GetY() * 0.33f + npcHeader.GetHeight(), npcDialog.GetWidth(), npcDialog.GetHeight()));

	SDL2pp::Point nextDialogNPCPos(dim.GetX() * 0.905f, dim.GetY() * 0.385f);
	_nextDialogNPC->SetScale(0.35f);
	_nextDialogNPC->Draw(nextDialogNPCPos);

	// Draw skip prompt
	SDL2pp::Texture holdToSkip(rend, _promptFont->RenderText_Solid("Hold TAB to Skip", SDL_Color{ 255, 255, 255, 255 }));
	rend.Copy(holdToSkip, NullOpt, Rect(dim.GetX() * 0.97f - holdToSkip.GetWidth(), dim.GetY()  * 0.97f - holdToSkip.GetHeight(), holdToSkip.GetWidth(), holdToSkip.GetHeight()));
	rend.FillRect(Rect(dim.GetX() * 0.97f - holdToSkip.GetWidth(), dim.GetY()  * 0.97f, holdToSkip.GetWidth() * _skipTimer / SKIP_TIME, holdToSkip.GetHeight() * 0.3f));

	rend.Present();
}

// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void CutsceneScreen::Unload()
{
	_dialogIndex = 0;
	_skipTimer = 0;
	_currentNPCDialog = " ";
	_currentProtagDialog = " ";

    delete _background;
    delete _protagDialogBox;
    delete _npcDialogBox;
    delete _promptFont;
    delete _headerFont;
    delete _dialogFont;
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

	std::ostringstream oss;
	oss << iss.rdbuf();
	_dialog[_dialogIndex] = oss.str();

	if (firstWord == "Borin:")
	{
		_currentProtagDialog = _dialog[_dialogIndex].substr(1).append(_dialog[_dialogIndex].substr(0, 1));
		_currentlySpeaking = PROTAG;
	}
	else
	{
		_currentNPCDialog = _dialog[_dialogIndex].substr(1).append(_dialog[_dialogIndex].substr(0, 1));
		_currentlySpeaking = NPC;
	}
}

SoundEffectBank & CutsceneScreen::GetSoundBank()
{
	return _soundBank;
}

CutsceneScreen::~CutsceneScreen()
{
}

