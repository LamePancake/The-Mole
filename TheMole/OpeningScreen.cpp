#include "OpeningScreen.h"
#include <cstring>
#include "GameManager.h"

using namespace SDL2pp;

const static SDL_Color NORMAL = { 255, 255, 255, 255 };
const static SDL_Color GRAY = { 100, 100, 100, 20 };

std::vector<std::string> story {
	"After a crushing defeat by the Grayliens,",
	"the Vikings are in dire straits.",
	"To end the fight once and for all,",
	"the Grayliens, led by the Underwatch have installed mind-control",
	"devices on a colony of moles living under the Viking stronghold.",
	"They're using the moles to identify weaknesses in the Viking defenses",
	"so that they can bomb them back to the 1100's.",
	"You are Borin, a mole living in the the colony of the mind-controlled moles.",
	"Today, like any other day, you are doing your rounds",
	"when you encounter your friend Toad.",
};

int OpeningScreen::Load()
{
	_mgr = GameManager::GetInstance();

	_currScreen = 0;

	_storyTexture.resize(2);

	_font = new SDL2pp::Font(".\\Assets\\Fonts\\Exo-Regular.otf", 35);
	_backgroundTexture = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\opening_bg.png");
	_backgroundTexture2 = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\opening_thing.png");
	_enter = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\dialog_enter.png", 4, 0.5f);
	_enter->SetScale(1.0f);

	for (int i = 0; i < story.size(); ++i)
	{
		if (i >= story.size() / 2.0f)
			_currScreen = 1;

		Texture* t = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid(story[i], NORMAL));
		_storyTexture[_currScreen].push_back(t);
	}
		
	_currScreen = 0;
	return SCREEN_LOAD_SUCCESS;
}

int OpeningScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	_enter->Update(elapsedSecs);
	
	if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
	{
		_currScreen++;

		if (_currScreen > 1)
		{
			_soundBank.PlaySound("accept");
			_mgr->SetNextScreen("menu");
			return SCREEN_FINISH;
		}
	}

	return SCREEN_CONTINUE;
}

void OpeningScreen::Draw()
{
	Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 1);
	rend.Clear();
	rend.Copy(*_backgroundTexture, NullOpt, NullOpt);

	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();

	rend.Copy(*_backgroundTexture2, NullOpt, Rect(size.GetX() * 0.5f - ((size.GetX() * 0.95f) / 2.0f), size.GetY() * 0.5f - ((size.GetY() * 0.65f) / 2.0f), size.GetX() * 0.95f, size.GetY() * 0.65f));

	int i = 0;
	for (Texture* line : _storyTexture[_currScreen])
	{
		rend.Copy(*line, NullOpt, Rect(size.GetX() * 0.5f - (line->GetWidth() / 2.0f), size.GetY() * (0.25f + (0.1 * i)), line->GetWidth(), line->GetHeight()));
		++i;
	}
	
	_enter->Draw(SDL2pp::Point(size.GetX() * 0.9f, size.GetY() * 0.9f));
	rend.Present();
}

// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void OpeningScreen::Unload()
{
	for (int i = 0; i < _storyTexture.size(); ++i)
	{
		for (int j = 0; j < _storyTexture[i].size(); ++j)
		{
			delete _storyTexture[i][j];
		}
	}

	_currScreen = 0;
	delete _backgroundTexture;
	delete _backgroundTexture2;
}

SoundEffectBank & OpeningScreen::GetSoundBank()
{
	return _soundBank;
}