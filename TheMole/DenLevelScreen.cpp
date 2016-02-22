#include "DenLevelScreen.h"
#include "GameManager.h"
#include "Vector2.h"
#include "Math.h"

using namespace SDL2pp;

int DenLevelScreen::Load() {
	GameScreen::Load(".\\Assets\\Levels\\den_level.txt", ".\\Assets\\Textures\\den_bg.png");

	return SCREEN_LOAD_SUCCESS;
}

int DenLevelScreen::Update(double elapsedSecs)
{
	if (GameScreen::Update(elapsedSecs) == SCREEN_FINISH) return SCREEN_FINISH;

	return SCREEN_CONTINUE;
}

void DenLevelScreen::Draw() 
{
	GameScreen::Draw();
}

void DenLevelScreen::Unload() {
	GameScreen::Unload();
}