#include "GameScreen.h"
#include "GameManager.h"
#include "Vector2.h"

const static SDL_Color NORMAL = { 255, 255, 255, 255 };
const static SDL_Color SELECTED = { 255, 80, 80, 255 };

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

	// Load level one in order to render
    _level = _levelLoader.LoadLevel(_levelPath, _player);
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

	_background = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), _backgroundPath);
	_winScreen  = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), _winScreenPath);
	_loseScreen = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), _loseScreenPath);
	_pancake    = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), ".\\Assets\\Textures\\Pancake.png");

	_menuItems[0] = new SDL2pp::Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Level Select", NORMAL));
	_menuItems[1] = new SDL2pp::Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Main Menu", NORMAL));
	_menuItems[2] = new SDL2pp::Texture(_mgr->GetRenderer(), _font->RenderText_Solid("Back", NORMAL));

	_pausedText = new SDL2pp::Texture(_mgr->GetRenderer(), _headerFont->RenderText_Solid("Paused", NORMAL));;
	_border = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\levelSelectBorder.png");
	_controls = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\levelSelectControls.png");

	_curMenuItem  = 0;

	return SCREEN_LOAD_SUCCESS;
}

int GameScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	// Check if the player pauses the game and handle the pause
	if (_mgr->inputManager->ActionOccurred("QUIT", Input::Pressed))
	{
		if (!_player->IsDead() && !_player->AtGoal())
			_paused = true;
	}

	if (_paused) return OnPauseUpdate();

	// Check if the player is dead
	if (_player->IsDead())
	{
		if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
		{
			_level->Reset();
		}

		return SCREEN_CONTINUE;
	}

	// Check if the player got to the goal
	if (_player->AtGoal())
	{
		if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
		{
			_mgr->_unlockedLevels[_nextLevel] = true;
			_mgr->WriteFile(".\\Assets\\Levels\\level_unlocks.txt");
			_mgr->SetNextScreen(_nextLevel);
			return SCREEN_FINISH;
		}
		return SCREEN_CONTINUE;
	}

	// Update player
	_player->Update(elapsedSecs);
	if (_player->StoppedTime())	return SCREEN_CONTINUE;

	for (auto actor : _level->GetActors())
	{
		// A bit hacky, to be sure
		if (actor->GetType() == Actor::Type::player) continue;
		actor->Update(elapsedSecs);
	}

	_level->Update(elapsedSecs);

	return SCREEN_CONTINUE;
}

void GameScreen::Draw()
{
	SDL2pp::Point dim = GameManager::GetInstance()->GetWindow().GetSize();
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();

	float levelWidth  = _level->GetLevelSize().x * _level->GetTileWidth();
	float levelHeight = _level->GetLevelSize().y * _level->GetTileHeight();

	float xNew      = ((float)_camera->GetViewport().x / levelWidth) * _background->GetWidth();
	float yNew      = ((float)_camera->GetViewport().y / levelHeight) * _background->GetHeight();
	float newWidth  = ((float)_camera->GetViewport().GetW() / levelWidth) * _background->GetWidth();
	float newHeight = ((float)_camera->GetViewport().GetH() / levelHeight) * _background->GetHeight();

	rend.Copy(*_background, SDL2pp::Rect(xNew, yNew, newWidth, newHeight), SDL2pp::NullOpt);
	_camera->CentreView(_player->GetPosition());

	// Render Level
	//_levelRenderer.RenderLevel(_level, *_camera);
	_levelRenderer.RenderLevel(_levelQuadtree, *_camera, _level->GetTileWidth(), _level->GetTileHeight());

	// Draw all actors
	for (auto actor : _level->GetActors())
	{
		actor->Draw(*_camera);
	}

	// Draw the win or lose screen
	if (_player->IsDead() || _player->AtGoal())
	{
		rend.Copy(_player->IsDead() ? *_loseScreen : *_winScreen, SDL2pp::NullOpt, SDL2pp::Rect((dim.x / 2) - (dim.x * 0.6 / 2), (dim.y / 2) - (dim.y * 0.6 / 2), dim.x * 0.6, dim.y * 0.6));
		//Draw pancakes later
	}

	// Draw the pause menu
	if (_paused)
		OnPauseDraw();

	rend.Present();
}

void GameScreen::Unload()
{
	_paused = false;
	
	for (int i = 0; i < NUM_MENU_ITEMS; ++i)
	{
		delete _menuItems[i];
	}

	delete _pausedText;
	delete _border;
	delete _controls;
	delete _camera;

	delete _font;
	delete _headerFont;
}

int GameScreen::OnPauseUpdate()
{
	if (_mgr->inputManager->ActionOccurred("ARROWDOWN", Input::Pressed) || _mgr->inputManager->ActionOccurred("DOWN", Input::Pressed))
	{
		_curMenuItem++;
		if (_curMenuItem == NUM_MENU_ITEMS) _curMenuItem = 0;
	}
	else if (_mgr->inputManager->ActionOccurred("ARROWUP", Input::Pressed) || _mgr->inputManager->ActionOccurred("UP", Input::Pressed))
	{
		_curMenuItem--;
		if (_curMenuItem < 0) _curMenuItem = NUM_MENU_ITEMS - 1;
	}
	else if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
	{
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

	rend.Copy(*_pausedText, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.02f, size.GetY() * 0.07f, _pausedText->GetWidth(), _pausedText->GetHeight()));
	rend.Copy(*_controls, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.60f, 0, size.GetX() * 0.40f, size.GetY() * 0.1f));
}
