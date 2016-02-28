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
	_winScreen = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), _winScreenPath);
	_loseScreen = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), _loseScreenPath);
	_pancake = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), ".\\Assets\\Textures\\Pancake.png");

	return SCREEN_LOAD_SUCCESS;
}

int GameScreen::Update(double elapsedSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	if (_mgr->inputManager->ActionOccurred("QUIT", Input::Pressed)) {
		exit(0);
	}

	if (_player->IsDead())
	{
		if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
		{
			_level->Reset();
			_player->Reset(_level->GetSpawnPoint());
		}
		return SCREEN_CONTINUE;
	}

	if (_player->AtGoal())
	{
		if (_mgr->inputManager->ActionOccurred("CONFIRM", Input::Pressed))
		{
			_mgr->SetNextScreen(_nextLevel);
			return SCREEN_FINISH;
		}
		return SCREEN_CONTINUE;
	}

	_player->Update(elapsedSecs);

	if (_player->StoppedTime())	return SCREEN_CONTINUE;

	_player->Update(elapsedSecs);
	if (_player->StoppedTime())
	{
		std::cout << ":SALKDJF:LSDKJF" << std::endl;
		return SCREEN_CONTINUE;
	}

	// Update Enemies
	for (size_t i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Update(elapsedSecs);
	}

	// Update objects
	for (size_t i = 0; i < _level->GetActorObjectSize(); ++i)
	{
		_level->GetActorObject(i)->Update(elapsedSecs);
	}

	// Update NPCs
	for (size_t i = 0; i < _level->GetNPCSize(); ++i)
	{
		_level->GetNPC(i)->Update(elapsedSecs);
	}

	std::shared_ptr<BossActor> boss = _level->GetBoss();
	if(boss) boss->Update(elapsedSecs);

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

	// Render enemies
	for (size_t i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Draw(*_camera);
	}

	// Render objects
	for (size_t i = 0; i < _level->GetActorObjectSize(); ++i)
	{
		_level->GetActorObject(i)->Draw(*_camera);
	}

	// Render NPCs
	for (size_t i = 0; i < _level->GetNPCSize(); ++i)
	{
		_level->GetNPC(i)->Draw(*_camera);
	}

	for (size_t i = 0; i < _level->GetProjectileActorSize(); ++i)
	{
		_level->GetProjectile(i)->Draw(*_camera);
	}

	// Render Player
	_player->Draw(*_camera);

	if (_player->IsDead())
	{
		SDL2pp::Point dim = GameManager::GetInstance()->GetWindow().GetSize();
		rend.Copy(*_loseScreen, SDL2pp::NullOpt, SDL2pp::Rect((dim.x / 2) - (dim.x * 0.6 / 2), (dim.y / 2) - (dim.y * 0.6 / 2), dim.x * 0.6, dim.y * 0.6));
		int counter = 0;
		for (int i = 0; i < _level->GetActorObjectSize(); i++)
		{
			if (!_level->GetActorObject(i)->IsVisible())
				counter++;
		}

		if (counter > 0)
		{
			rend.Copy(*_pancake, SDL2pp::NullOpt, SDL2pp::Rect((dim.x / 2) - (_pancake->GetWidth() / 2), dim.y * 0.55 - (_pancake->GetHeight() / 2), _pancake->GetWidth(), _pancake->GetHeight()));
		}
	}

	if (_player->AtGoal())
	{
		SDL2pp::Point dim = GameManager::GetInstance()->GetWindow().GetSize();
		rend.Copy(*_winScreen, SDL2pp::NullOpt, SDL2pp::Rect((dim.x / 2) - (dim.x * 0.6 / 2), (dim.y / 2) - (dim.y * 0.6 / 2), dim.x * 0.6, dim.y * 0.6));
		int counter = 0;
		for (int i = 0; i < _level->GetActorObjectSize(); i++)
		{
			if (!_level->GetActorObject(i)->IsVisible())
				counter++;
		}

		if (counter > 0)
		{
			rend.Copy(*_pancake, SDL2pp::NullOpt, SDL2pp::Rect((dim.x / 2) - (_pancake->GetWidth() / 2), dim.y * 0.55 - (_pancake->GetHeight() / 2), _pancake->GetWidth(), _pancake->GetHeight()));
		}
	}

	rend.Present();
}

void GameScreen::Unload()
{
	free(_prevKeyState);
	delete _camera;
}

