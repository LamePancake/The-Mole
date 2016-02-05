#include "DenLevelScreen.h"
#include "GameManager.h"
#include "Vector2.h"

using namespace SDL2pp;

int DenLevelScreen::Load() {
	_mgr = GameManager::GetInstance();

	// Load level one in order to render
	_level = _levelLoader.LoadLevel(".\\Assets\\Levels\\den_level.txt", _player);
	_levelRenderer.Load(*_mgr);

	SDL2pp::Point playerPos((int)(_player->GetPosition().GetX()), (int)(_player->GetPosition().GetY()));
	SDL2pp::Point viewportSize = _mgr->GetWindow().GetSize();
	SDL2pp::Point levelSize = _level->GetLevelSize();
	levelSize.x *= _level->GetTileWidth();
	levelSize.y *= _level->GetTileHeight();
	_camera = new Camera(playerPos, viewportSize, levelSize);

	_background = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), ".\\Assets\\Textures\\den_bg.png");

	return SCREEN_LOAD_SUCCESS;
}

int DenLevelScreen::Update(double elasepdSecs)
{
	SDL_PumpEvents();
	_mgr->inputManager->UpdateKeyboardState();

	if (_mgr->inputManager->ActionOccured("QUIT", Input::Pressed)) {
		exit(0);
	}

	if (_mgr->inputManager->ActionOccured("LEFT", Input::Held)) 
	{
		_player->SetSpeed(Vector2(-120.0f, _player->GetSpeed().GetY()));
		_player->SetActorDirection(SpriteSheet::XAxisDirection::LEFT);
	}
	else if (_mgr->inputManager->ActionOccured("RIGHT", Input::Held))
	{
		_player->SetSpeed(Vector2(120.0f, _player->GetSpeed().GetY()));
		_player->SetActorDirection(SpriteSheet::XAxisDirection::RIGHT);
	}
	else
	{
		_player->SetSpeed(Vector2(0.0f, _player->GetSpeed().GetY()));
	}

	for (size_t i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Update(elasepdSecs, _level);
	}
	_player->Update(elasepdSecs, _level);
	return SCREEN_CONTINUE;
}

void DenLevelScreen::Draw() 
{
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	rend.SetDrawColor(100, 100, 100, 255);
	rend.Clear();

	rend.Copy(*_background, SDL2pp::NullOpt, SDL2pp::NullOpt);
	_camera->CentreView(_player->GetPosition());

	_levelRenderer.RenderLevel(_level, *_camera);
	for (size_t i = 0; i < _level->GetEnemySize(); ++i)
	{
		_level->GetEnemy(i)->Draw(*_camera);
	}
	_player->Draw(*_camera);

	rend.Present();
}

void DenLevelScreen::Unload() {
	free(_prevKeyState);
	delete _camera;
}