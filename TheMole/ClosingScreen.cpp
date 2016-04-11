#include "ClosingScreen.h"
#include <cstring>
#include "GameManager.h"

using namespace SDL2pp;

const static SDL_Color NORMAL = { 255, 255, 255, 255 };
const static SDL_Color GRAY = { 100, 100, 100, 20 };

std::vector<std::string> matt{
	"Matt Ku",
	"Chief Projectile Motion Researcher",         
	"Turret Engineer",     
	"Lead Input Handling Designer",
	"Viking Hat Strengthening Associate",     
	"Senior Actor Deleter",
	"Chief Trap Implementer",
	"Senior Red Marker User",
	"Lead Fight Organizer",
};

std::vector<std::string> shane{
	"Shane Spoor",
	"Chief Refactorer",
	"Lead Speedrun Attemptor",
	"Senior Late Night Comment Author",
	"Lead Door Opener",
	"Shimmer Investigator",
	"Explosives Expert",
	"Noise Maker",
	"Chief Coffee Consumer",
};

std::vector<std::string> trevor{
	"Trevor Ware",
	"Team Lead",
	"Quadtree Implementation Technical Lead",
	"Senior Menu Artist",
	"Chief Committer",
	"Map Format Architect",
	"Dialogue Writer",
	"Cutscene Face Arranger",
	"Hat Stylist",
};

std::vector<std::string> tim{
	"Tim Wang",
	"Lead Artist",
	"Chief AI Programmer",
	"Senior Annoying Pancake Jump Placer",
	"Chief SourceTree Evangelist",
	"Lead Cameo Character Artist",
	"Preliminary Tile Collision Implementor",
	"Lead Animator",
	"Senior Practicum Developer",
};

std::vector<std::string> end{
	"Thank you for playing!!",
};

int ClosingScreen::Load()
{
	_mgr = GameManager::GetInstance();

	_closingTheme = new Music(".\\Assets\\Audio\\In-The-End.mp3");
	_mgr->GetMixer().PlayMusic(*_closingTheme);

	_font = new SDL2pp::Font(".\\Assets\\Fonts\\Exo-Regular.otf", 35);
	_backgroundTexture = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\end_bg.png");
	_backgroundTexture2 = new Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\opening_thing.png");
	_borinAndTheChicken = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\borin_end.png", 4, 0.5f);
	_borinAndTheChicken->SetScale(1.0f);

	_creditsTexture.resize(5);

	for (int j = 0; j < matt.size(); ++j)
	{
		Texture* t = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid(matt[j], NORMAL));
		_creditsTexture[0].push_back(t);
	}

	for (int j = 0; j < shane.size(); ++j)
	{
		Texture* t = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid(shane[j], NORMAL));
		_creditsTexture[1].push_back(t);
	}

	for (int j = 0; j < tim.size(); ++j)
	{
		Texture* t = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid(tim[j], NORMAL));
		_creditsTexture[2].push_back(t);
	}

	for (int j = 0; j < trevor.size(); ++j)
	{
		Texture* t = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid(trevor[j], NORMAL));
		_creditsTexture[3].push_back(t);
	}

	for (int j = 0; j < end.size(); ++j)
	{
		Texture* t = new Texture(_mgr->GetRenderer(), _font->RenderText_Solid(end[j], NORMAL));
		_creditsTexture[4].push_back(t);
	}

	_drawnCredit.push_back(_creditsTexture[_currPerson][_currDraw++]);

	return SCREEN_LOAD_SUCCESS;
}

int ClosingScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	_timer += elapsedSecs;

	_borinAndTheChicken->Update(elapsedSecs);

	if (_timer > 1.6)
	{
		_timer = 0;

		if (_currPerson < _creditsTexture.size())
		{
			if (_currDraw < _creditsTexture[_currPerson].size())
				_drawnCredit.push_back(_creditsTexture[_currPerson][_currDraw++]);
			else
				_currDraw++;

			if (_currDraw > _creditsTexture[_currPerson].size())
			{
				_currPerson++;
				_currDraw = 0;

				if (_currPerson >= 5)
				{
					_end = true;
				}
				else
				{
					_drawnCredit.clear();
				}
			}
		}
	}

	if (_end)
	{
		if (_endTimer == 0.0)
			_mgr->GetMixer().FadeOutMusic(10000);
		
		_endTimer += elapsedSecs;
		
		if (_endTimer > 10)
		{
			_mgr->SetNextScreen("menu");
			return SCREEN_FINISH;
		}
	}

	return SCREEN_CONTINUE;
}

void ClosingScreen::Draw()
{
	Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 1);
	rend.Clear();
	rend.Copy(*_backgroundTexture, NullOpt, NullOpt);

	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();

	rend.Copy(*_backgroundTexture2, NullOpt, Rect(size.GetX() * 0.5f - ((size.GetX() * 0.95f) / 2.0f), size.GetY() * 0.5f - ((size.GetY() * 0.85f) / 2.0f), size.GetX() * 0.95f, size.GetY() * 0.85f));

	int i = 0;
	for (Texture* line : _drawnCredit)
	{
		rend.Copy(*line, NullOpt, Rect(size.GetX() * 0.5f - (line->GetWidth() / 2.0f), size.GetY() * (0.1f + (0.09 * i)), line->GetWidth(), line->GetHeight()));
		++i;
	}

	_borinAndTheChicken->Draw(SDL2pp::Point(size.GetX() * 0.375f, size.GetY() * 0.855f));
	rend.Present();
}

// Could probably put this in a destructor, actually, but I guess it's nice to know when it will be called
void ClosingScreen::Unload()
{
	for (int j = 0; j < _creditsTexture.size(); ++j)
	{
		for (int i = 0; i < _creditsTexture[j].size(); ++i)
		{
			delete _creditsTexture[j][i];
		}
		_creditsTexture[j].clear();
	}
	
	_creditsTexture.clear();
	_timer = 0;
	_endTimer = 0;

	_end = false;

	_drawnCredit.clear();

	_currDraw = 0;
	_currPerson = 0;

	delete _backgroundTexture;
	delete _backgroundTexture2;

	delete _closingTheme;
}

SoundEffectBank & ClosingScreen::GetSoundBank()
{
	return _soundBank;
}