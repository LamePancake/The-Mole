#include "GameScreen.h"
#include "GameManager.h"
#include "Vector2.h"
#include "Util.h"

const static SDL_Color NORMAL = { 255, 255, 255, 255 };
const static SDL_Color SELECTED = { 255, 80, 80, 255 };
const static SDL_Color COMPLETE = { 150, 255, 150, 255 };
const static SDL_Color GAMEOVER = { 255, 20, 20, 255 };
const static SDL_Color RECORD = { 255, 153, 51, 255 };

#define GAMEOVER_DELAY 2.0
#define SKIP_TIME 1.1

std::shared_ptr<Level> GameScreen::GetLevel() const
{
	return _level;
}

const std::shared_ptr<PlayerActor> GameScreen::GetPlayer() const
{
	return _player;
}

SoundEffectBank & GameScreen::GetSoundBank()
{
	return _soundBank;
}

int GameScreen::Load()
{
	_mgr = GameManager::GetInstance();

	_mgr->ReadHighScoreFile(_scorePath);

	// Load level one in order to render
	if (_levelPath == "./Assets/Levels/den_level.txt"
		|| _levelPath == "./Assets/Levels/preden_level.txt"
		|| _levelPath == "./Assets/Levels/postden_level.txt"
		|| _levelPath == "./Assets/Levels/previking_level.txt")
	{
		_level = _levelLoader.LoadLevel(_levelPath, _player, 0);
	}	
	else if (_levelPath == "./Assets/Levels/postgrass_level.txt")
	{
		_level = _levelLoader.LoadLevel(_levelPath, _player, 1);
	}
	else
	{
		_level = _levelLoader.LoadLevel(_levelPath, _player);
	}
		
	SDL2pp::Point levelSize = _level->GetLevelSize();

	// Inialize the quadtree
	_levelQuadtree = std::make_shared<Quadtree>();
	_levelQuadtree->Initialize(4, levelSize.GetX(), levelSize.GetY());

	for (int height = 0; height < levelSize.y; ++height)
	{
		for (int width = 0; width < levelSize.x; ++width)
		{
			std::shared_ptr<Tile> tempTile = _level->GetTileFromLevel(width, height);
			char id = tempTile->GetID();

			if (id == Tile::blank || id == Tile::enemy)
				continue;
			_levelQuadtree->Insert(tempTile, width, height);
		}
	}

   	_levelRenderer.Load(*_mgr);

	SDL2pp::Point playerPos((int)(_player->GetPosition().GetX()), (int)(_player->GetPosition().GetY()));
	SDL2pp::Point viewportSize = _mgr->GetWindow().GetSize();
	levelSize.x *= _level->GetTileWidth();
	levelSize.y *= _level->GetTileHeight();
	_camera = new Camera(playerPos, viewportSize, levelSize);

	_font = new SDL2pp::Font(".\\Assets\\Fonts\\Exo-Regular.otf", 50);
	_headerFont = new SDL2pp::Font(".\\Assets\\Fonts\\BEBAS.ttf", 80);
	_recordFont = new SDL2pp::Font(".\\Assets\\Fonts\\Exo-Regular.otf", 25);
	_promptFont = new SDL2pp::Font(".\\Assets\\Fonts\\BEBAS.ttf", 20);

	_background     = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), _backgroundPath);
	_pancake        = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), ".\\Assets\\Textures\\Pancake.png");
	_pancakeMarker  = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), ".\\Assets\\Textures\\Pancakemarker.png");
	_pancakeMarker2 = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), ".\\Assets\\Textures\\Pancakemarker2.png");
	_skull          = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), ".\\Assets\\Textures\\Skull.png");

	_menuItems[0] = new SDL2pp::Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Level Select", NORMAL));
	_menuItems[1] = new SDL2pp::Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Main Menu", NORMAL));
	_menuItems[2] = new SDL2pp::Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Back", NORMAL));

	_pausedText = new SDL2pp::Texture(_mgr->GetRenderer(), _headerFont->RenderText_Solid("Paused", NORMAL));
	_levelCompleteText = new SDL2pp::Texture(_mgr->GetRenderer(), _headerFont->RenderText_Solid("Level Complete", COMPLETE));
	_gameOverText = new SDL2pp::Texture(_mgr->GetRenderer(), _headerFont->RenderText_Solid("Game Over", GAMEOVER));

	_pressEnterToContinue = new SDL2pp::Texture(_mgr->GetRenderer(), _font->RenderText_Solid("               Continue               ", SELECTED));
	_newRecord = new SDL2pp::Texture(_mgr->GetRenderer(), _recordFont->RenderText_Solid("NEW RECORD!", RECORD));

	_stringFormatter << " : " << setfill('0') << setw(3) << to_string(_deaths);
	_deathCounter = new SDL2pp::Texture(_mgr->GetRenderer(), _font->RenderText_Solid(_stringFormatter.str(), NORMAL));
	_stringFormatter.str(std::string());
	_stringFormatter.clear();

	_border = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\levelSelectBorder.png");
	_controls = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\levelSelectControls.png");
	_levelCompleteControls = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\CreditsControls.png");

	_curMenuItem  = 0;
	_mgr->PlayMusic(_backgroundTrack);
	return SCREEN_LOAD_SUCCESS;
}

int GameScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	// Check if the player pauses the game and handle the pause
	if (_mgr->inputManager->ActionOccurred("QUIT", Input::Pressed) && !isIntroScreen)
	{
		if (!_player->IsDead() && !_player->AtGoal())
			_paused = true;
	}

	if (_paused) return OnPauseUpdate();

	// Check if the player is dead
	if (_player->IsDead())
	{
		if (!_deathSoundPlayed)
		{
			_mgr->GetMixer().PauseMusic();
			_soundBank.PlaySound("lose");
			_deathSoundPlayed = true;
		}
			
		_deathTimer += elapsedSecs;

		if (_deathTimer >= GAMEOVER_DELAY / 2.0)
		{
			if (!_deathCounterUpdated)
			{
				_deaths++;
				if (_deaths < 1000)
					_stringFormatter << " : " << setfill('0') << setw(3) << to_string(_deaths);
				else
					_stringFormatter << " : " << "???";
				
				_deathCounter = new SDL2pp::Texture(_mgr->GetRenderer(), _font->RenderText_Solid(_stringFormatter.str(), NORMAL));
				_stringFormatter.str(std::string());
				_stringFormatter.clear();
				_deathCounterUpdated = true;
			}
		}
		
		if (_deathTimer >= GAMEOVER_DELAY)
		{
			_deathTimer = 0;
			_deathCounterUpdated = false;
			_level->Reset();
			_deathSoundPlayed = false;
			_mgr->GetMixer().ResumeMusic();
		}
		return SCREEN_CONTINUE;
	}

	// Check if the player got to the goal
	if (_player->AtGoal())
	{
		if (!_winSoundPlayed)
		{
			_mgr->GetMixer().PauseMusic();
			_soundBank.PlaySound("win");
			_winSoundPlayed = true;
		}
			
		if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
		{
			if (_deaths < _mgr->_bestDeathCount)
				_mgr->_bestDeathCount = _deaths;

			int count = 0;
			for (auto e : _level->GetPancakes())
				if (e)
					count++;

			if (count > _mgr->_bestPancakeCount)
				_mgr->_bestPancakeCount = count;

			_deaths = 0;
			_mgr->_unlockedLevels[_nextLevel] = true;
			_mgr->WriteLevelUnlockFile(".\\Assets\\SavedData\\level_unlocks.txt");
			_mgr->WriteHighScoreFile(_scorePath);
			_soundBank.PlaySound("accept");
			_mgr->SetNextScreen(_nextLevel);
			_winSoundPlayed = false;
			_mgr->GetMixer().ResumeMusic();
			return SCREEN_FINISH;
		}
		return SCREEN_CONTINUE;
	}

	if (_player->TriggeredIntro())
	{
		_mgr->SetNextScreen(_nextLevel);
		return SCREEN_FINISH;
	}

	// Update player
	_player->Update(elapsedSecs);
	if (_player->StoppedTime())	return SCREEN_CONTINUE;

	for (auto actor : _level->GetActors())
	{
		// A bit hacky, to be sure
		if (actor->GetType() == Actor::Type::player || !actor->IsActive()) continue;
		actor->Update(elapsedSecs);
	}

	_level->Update(elapsedSecs);

	if (isIntroScreen)
	{
		if (_mgr->inputManager->ActionOccurred("SKIP", Input::Held))
		{
			_skipTimer += elapsedSecs;

			if (_skipTimer >= SKIP_TIME)
			{
				_soundBank.PlaySound("accept");
				_mgr->SetNextScreen(_nextLevel);
				return SCREEN_FINISH;
			}
		}
		else
		{
			_skipTimer = 0;
		}
	}
	
	return SCREEN_CONTINUE;
}

void GameScreen::Draw()
{
	SDL2pp::Point dim = GameManager::GetInstance()->GetWindow().GetSize();
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(200, 100, 100, 255);
	rend.Clear();

	float levelWidth  = _level->GetLevelSize().x * _level->GetTileWidth();
	float levelHeight = _level->GetLevelSize().y * _level->GetTileHeight();

	//float xNew      = ((float)_camera->GetViewport().x / levelWidth) * _background->GetWidth();
	//float yNew      = ((float)_camera->GetViewport().y / levelHeight) * _background->GetHeight();
	//float newWidth  = ((float)_camera->GetViewport().GetW() / levelWidth) * _background->GetWidth();
	//float newHeight = ((float)_camera->GetViewport().GetH() / levelHeight) * _background->GetHeight();

	//rend.Copy(*_background, SDL2pp::Rect(xNew, yNew, newWidth, newHeight), SDL2pp::NullOpt);
	rend.Copy(*_background, SDL2pp::NullOpt, SDL2pp::NullOpt);
	_camera->CentreView(_player->GetPosition());

	// Render Level
	//_levelRenderer.RenderLevel(_level, *_camera);
	_levelRenderer.RenderLevel(_levelQuadtree, *_camera, _level->GetTileWidth(), _level->GetTileHeight());

	// Sort all actors by z index
	Sort(_level->GetActors());
	// Draw all actors
	for (auto actor : _level->GetActors())
	{
		actor->Draw(*_camera);
	}

	if (_player->IsDead())
		OnGameOverDraw();
	else if (_player->AtGoal())
		OnLevelCompleteDraw();
	else if (_paused)
		OnPauseDraw();
	else if (isIntroScreen)
	{
		// Draw skip prompt
		SDL2pp::Texture holdToSkip(rend, _promptFont->RenderText_Solid("Hold TAB to Skip", SDL_Color{ 255, 255, 255, 255 }));
		rend.Copy(holdToSkip, SDL2pp::NullOpt, SDL2pp::Rect(dim.GetX() * 0.97f - holdToSkip.GetWidth(), dim.GetY()  * 0.97f - holdToSkip.GetHeight(), holdToSkip.GetWidth(), holdToSkip.GetHeight()));
		rend.FillRect(SDL2pp::Rect(dim.GetX() * 0.97f - holdToSkip.GetWidth(), dim.GetY()  * 0.97f, holdToSkip.GetWidth() * _skipTimer / SKIP_TIME, holdToSkip.GetHeight() * 0.3f));
	}
	else
	{
		// Render pancakes
		for (int i = 0; i < _level->GetPancakes().size(); ++i)
		{
			if (_level->GetPancakes()[i])
				rend.Copy(*_pancake, SDL2pp::NullOpt, SDL2pp::Rect(dim.GetX() * 0.01 + (_pancake->GetWidth() * 0.64 * i), dim.GetY() * 0.99 - (_pancake->GetHeight() * 0.6), _pancake->GetWidth() * 0.6, _pancake->GetHeight() * 0.6));
			else
				rend.Copy(*_pancakeMarker, SDL2pp::NullOpt, SDL2pp::Rect(dim.GetX() * 0.01 + (_pancake->GetWidth() * 0.64 * i), dim.GetY() * 0.99 - (_pancake->GetHeight() * 0.6), _pancake->GetWidth() * 0.6, _pancake->GetHeight() * 0.6));
		}

		// Render death counter
		rend.Copy(*_skull, SDL2pp::NullOpt, SDL2pp::Rect(dim.GetX() * 0.01, dim.GetY() * 0.01, _skull->GetWidth() * 0.5, _skull->GetHeight() * 0.5));
		rend.Copy(*_deathCounter, SDL2pp::NullOpt, SDL2pp::Rect(dim.GetX() * 0.01 + (_skull->GetWidth() * 0.5), dim.GetY() * 0.01, _deathCounter->GetWidth() * 0.5, _deathCounter->GetHeight() * 0.5));
	}		

	rend.Present();
}

void GameScreen::Unload()
{
	_mgr->ClearHighScores();
	_deathSoundPlayed = false;
	_winSoundPlayed = false;
	_stringFormatter.str(std::string());
	_stringFormatter.clear();
	_deathTimer = 0.0;
	_deathCounterUpdated = false;
	_curMenuItem = 0;
	_paused = false;
	_deaths = 0;
	_skipTimer = 0;
	
	for (int i = 0; i < NUM_MENU_ITEMS; ++i)
	{
		delete _menuItems[i];
	}

    delete _camera;
    
    delete _border;
	delete _controls;
	delete _levelCompleteControls;

	delete _font;
	delete _headerFont;
	delete _recordFont;
	delete _promptFont;

	delete _pausedText;
	delete _levelCompleteText;
	delete _pressEnterToContinue;
	delete _newRecord;
	delete _deathCounter;
	delete _gameOverText;
}

int GameScreen::OnPauseUpdate()
{
	if (_mgr->inputManager->ActionOccurred("ARROWDOWN", Input::Pressed) || _mgr->inputManager->ActionOccurred("DOWN", Input::Pressed))
	{
		_soundBank.PlaySound("select");
		_curMenuItem++;
		if (_curMenuItem == NUM_MENU_ITEMS) _curMenuItem = 0;
	}
	else if (_mgr->inputManager->ActionOccurred("ARROWUP", Input::Pressed) || _mgr->inputManager->ActionOccurred("UP", Input::Pressed))
	{
		_soundBank.PlaySound("select");
		_curMenuItem--;
		if (_curMenuItem < 0) _curMenuItem = NUM_MENU_ITEMS - 1;
	}
	else if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
	{
		_soundBank.PlaySound("accept");
		switch (_curMenuItem)
		{
		case 0:
			_mgr->SetNextScreen("levelSelect");
			return SCREEN_FINISH;
		case 1:
			_mgr->SetNextScreen("menu");
			return SCREEN_FINISH;
		case 2:
			_paused = false;
			_curMenuItem = 0;
		}
	}

	return SCREEN_CONTINUE;
}

void GameScreen::OnPauseDraw()
{
	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();
	SDL2pp::Renderer& rend = _mgr->GetRenderer();

	_border->SetAlphaMod(120);
	rend.Copy(*_border, SDL2pp::Rect(0, 0, 1, 1), SDL2pp::NullOpt);
	_border->SetAlphaMod(255);

	for (size_t i = 0; i < NUM_MENU_ITEMS; ++i)
	{
		int scaleFactor = 0;
		SDL_Color c;
		if (_curMenuItem == i)
		{
			c = SELECTED;
			scaleFactor = 15;
		}
		else
		{
			c = NORMAL;
		}
			
		rend.Copy(*_border, SDL2pp::NullOpt, SDL2pp::Rect(0.0f, size.GetY() * (0.25f + ((float)i * 0.12f)) - (scaleFactor / 2), _menuItems[i]->GetWidth() + size.GetX() * 0.17f, _menuItems[i]->GetHeight() + scaleFactor));

		_menuItems[i]->SetColorMod(c.r, c.g, c.b);
		rend.Copy(*_menuItems[i], SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.05f, size.GetY() * (0.25f + ((float)i * 0.12f)) - (scaleFactor / 2), _menuItems[i]->GetWidth() + scaleFactor, _menuItems[i]->GetHeight() + scaleFactor));
	}

	rend.Copy(*_border, SDL2pp::Rect(0, 0, 1, 1), SDL2pp::Rect(0.0f, size.GetY() * 0.075f, size.GetX(), _pausedText->GetHeight()));
	rend.Copy(*_pausedText, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.02f, size.GetY() * 0.07f, _pausedText->GetWidth(), _pausedText->GetHeight()));
	rend.Copy(*_controls, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.60f, 0, size.GetX() * 0.40f, size.GetY() * 0.1f));
}

void GameScreen::OnLevelCompleteDraw()
{
	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();
	SDL2pp::Renderer& rend = _mgr->GetRenderer();

	_border->SetAlphaMod(120);
	rend.Copy(*_border, SDL2pp::Rect(0, 0, 1, 1), SDL2pp::NullOpt);
	_border->SetAlphaMod(255);

	rend.Copy(*_levelCompleteText, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.5 - (_levelCompleteText->GetWidth() / 2), size.GetY() * 0.15f, _levelCompleteText->GetWidth(), _levelCompleteText->GetHeight()));

	int pancakeWidth = _level->GetPancakes().size() * _pancake->GetWidth() * 2.5;
	int pancakeCount = 0;
	for (size_t i = 0; i < _level->GetPancakes().size(); ++i)
	{
		if (_level->GetPancakes()[i])
		{
			pancakeCount++;
			rend.Copy(*_pancake, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.5 - (pancakeWidth / 2) + (_pancake->GetWidth() * 2.5 * i), size.GetY() * 0.35f, _pancake->GetWidth() * 2, _pancake->GetHeight() * 2));
		}
		else
			rend.Copy(*_pancakeMarker2, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.5 - (pancakeWidth / 2) + (_pancake->GetWidth() * 2.5 * i), size.GetY() * 0.35f, _pancake->GetWidth() * 2, _pancake->GetHeight() * 2));
	}

	if (pancakeCount > _mgr->_bestPancakeCount)
		rend.Copy(*_newRecord, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.87 - (_newRecord->GetWidth() / 2), size.GetY() * 0.38f, _newRecord->GetWidth(), _newRecord->GetHeight()));

	rend.Copy(*_skull, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.35, size.GetY() * 0.55f, _skull->GetWidth() * 2.1, _skull->GetHeight() * 2.1));
	rend.Copy(*_deathCounter, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.35 + (_skull->GetWidth() * 2.1), size.GetY() * 0.55f, _deathCounter->GetWidth() * 2.1, _deathCounter->GetHeight() * 2.1));
	
	if (_deaths < _mgr->_bestDeathCount)
		rend.Copy(*_newRecord, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.87 - (_newRecord->GetWidth() / 2), size.GetY() * 0.58f, _newRecord->GetWidth(), _newRecord->GetHeight()));

	rend.Copy(*_border, SDL2pp::Rect(0, 0, 1, 1), SDL2pp::Rect(size.GetX() * 0.5 - (_pressEnterToContinue->GetWidth() / 2), size.GetY() * 0.88f, _pressEnterToContinue->GetWidth(), size.GetY() * 0.13));
	rend.Copy(*_pressEnterToContinue, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.5 - (_pressEnterToContinue->GetWidth() / 2), size.GetY() * 0.88f, _pressEnterToContinue->GetWidth(), _pressEnterToContinue->GetHeight()));

	rend.Copy(*_levelCompleteControls, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.7f, 0, size.GetX() * 0.40f, size.GetY() * 0.1f));
}

void GameScreen::OnGameOverDraw()
{
	SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();
	SDL2pp::Renderer& rend = _mgr->GetRenderer();

	_border->SetAlphaMod(120);
	rend.Copy(*_border, SDL2pp::Rect(0, 0, 1, 1), SDL2pp::NullOpt);
	_border->SetAlphaMod(255);

	rend.Copy(*_gameOverText, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.5 - (_gameOverText->GetWidth() / 2), size.GetY() * 0.15f, _gameOverText->GetWidth(), _gameOverText->GetHeight()));

	rend.Copy(*_skull, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.35, size.GetY() * 0.45f, _skull->GetWidth() * 2.1, _skull->GetHeight() * 2.1));
	rend.Copy(*_deathCounter, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.35 + (_skull->GetWidth() * 2.1), size.GetY() * 0.45f, _deathCounter->GetWidth() * 2.1, _deathCounter->GetHeight() * 2.1));
}
