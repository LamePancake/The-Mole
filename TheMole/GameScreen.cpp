#include "GameScreen.h"
#include "GameManager.h"
#include "Vector2.h"

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
	_levelRenderer.Load(*_mgr);

	SDL2pp::Point playerPos((int)(_player->GetPosition().GetX()), (int)(_player->GetPosition().GetY()));
	SDL2pp::Point viewportSize = _mgr->GetWindow().GetSize();
	SDL2pp::Point levelSize = _level->GetLevelSize();
	levelSize.x *= _level->GetTileWidth();
	levelSize.y *= _level->GetTileHeight();
	_camera = new Camera(playerPos, viewportSize, levelSize);

	_background = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), _backgroundPath);
	_winScreen  = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), _winScreenPath);
	_loseScreen = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), _loseScreenPath);
	_pancake    = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), ".\\Assets\\Textures\\Pancake.png");

	_return      = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Pause_Menu\\pause_return.png");
	_mainMenu    = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Pause_Menu\\pause_main.png");
	_levelSelect = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\GUI\\Pause_Menu\\pause_levelselect.png");

	_curMenuItem  = 0;
	_menuItems[0] = _levelSelect;
	_menuItems[1] = _mainMenu;
	_menuItems[2] = _return;

	return SCREEN_LOAD_SUCCESS;
}

int GameScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	// Check if the player pauses the game and handle the pause
	if (_mgr->inputManager->ActionOccurred("QUIT", Input::Pressed)
		&& (!_player->IsDead() || _player->AtGoal()))  _paused = true;

	if (_paused) return OnPause();

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
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();

	rend.Copy(*_background, SDL2pp::NullOpt, SDL2pp::NullOpt);
	_camera->CentreView(_player->GetPosition());

	// Render Level
	_levelRenderer.RenderLevel(_level, *_camera);

	// Draw all actors
	for (auto actor : _level->GetActors())
	{
		actor->Draw(*_camera);
	}

	// Draw the win or lose screen
	if (_player->IsDead() || _player->AtGoal())
	{
		SDL2pp::Point dim = GameManager::GetInstance()->GetWindow().GetSize();
		rend.Copy(_player->IsDead() ? *_loseScreen : *_winScreen, SDL2pp::NullOpt, SDL2pp::Rect((dim.x / 2) - (dim.x * 0.6 / 2), (dim.y / 2) - (dim.y * 0.6 / 2), dim.x * 0.6, dim.y * 0.6));
		//Draw pancakes later
	}

	// Draw the pause menu
	if (_paused)
	{
		SDL2pp::Point dim = GameManager::GetInstance()->GetWindow().GetSize();
		rend.Copy(*_menuItems[_curMenuItem], SDL2pp::NullOpt, SDL2pp::Rect((dim.x / 2) - (dim.x * 0.6 / 2), (dim.y / 2) - (dim.y * 0.6 / 2), dim.x * 0.6, dim.y * 0.6));
	}

	rend.Present();
}

void GameScreen::Unload()
{
	delete _mainMenu;
	delete _levelSelect;
	delete _return;
	free(_prevKeyState);
	delete _camera;
}

int GameScreen::OnPause()
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
			_paused = false;
			return SCREEN_FINISH;
		case 1:
			_mgr->SetNextScreen("menu");
			_paused = false;
			return SCREEN_FINISH;
		case 2:
			_paused = false;
		}
	}

	return SCREEN_CONTINUE;
}
