#include "DenLevelScreen.h"
#include "GameManager.h"
#include "Vector2.h"
#include "Math.h"

using namespace SDL2pp;

int DenLevelScreen::Load() {
	GameScreen::Load(".\\Assets\\Levels\\den_level.txt", ".\\Assets\\Textures\\den_bg.png");

	//_mgr = GameManager::GetInstance();

	//// Load level one in order to render
	////_level = _levelLoader.LoadLevel(".\\Assets\\Levels\\den_level.txt", _player);
	////GameScreen::SetPlayer(_player);
	//_levelRenderer.Load(*_mgr);

	//SDL2pp::Point playerPos((int)(_player->GetPosition().GetX()), (int)(_player->GetPosition().GetY()));
	//SDL2pp::Point viewportSize = _mgr->GetWindow().GetSize();
	//SDL2pp::Point levelSize = _level->GetLevelSize();
	//levelSize.x *= _level->GetTileWidth();
	//levelSize.y *= _level->GetTileHeight();
	//_camera = new Camera(playerPos, viewportSize, levelSize);

	//_background = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), ".\\Assets\\Textures\\den_bg.png");

	return SCREEN_LOAD_SUCCESS;
}

int DenLevelScreen::Update(double elapsedSecs)
{
	//SDL_PumpEvents();
	//_mgr->inputManager->UpdateKeyboardState();
	GameScreen::Update(elapsedSecs);

	return SCREEN_CONTINUE;
}

void DenLevelScreen::Draw() 
{
	GameScreen::Draw();
	//SDL2pp::Renderer& rend = _mgr->GetRenderer();
	//rend.SetDrawColor(100, 100, 100, 255);
	//rend.Clear();

	//rend.Copy(*_background, SDL2pp::NullOpt, SDL2pp::NullOpt);
	//_camera->CentreView(_player->GetPosition());

	//_levelRenderer.RenderLevel(_level, *_camera);
	//for (size_t i = 0; i < _level->GetEnemySize(); ++i)
	//{
	//	_level->GetEnemy(i)->Draw(*_camera);
	//}
	//_player->Draw(*_camera);

	//rend.Present();
}

void DenLevelScreen::Unload() {
	GameScreen::Unload();
}