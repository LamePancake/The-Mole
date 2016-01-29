#pragma once
#include "Screen.h"
#include "LevelLoader.h"
#include "Level.h"
#include "LevelRenderer.h"

#define NUM_MENU_ITEMS 3

class MenuScreen : public Screen {
public:
	virtual int Load(GameManager& manager) override;
	virtual int Update(double elasepdSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

private:

	GameManager* _mgr;
	int _curMenuItem;
	SDL2pp::Texture* _menuItems[NUM_MENU_ITEMS];
	SDL2pp::Texture* _play;
	SDL2pp::Texture* _settings;
	SDL2pp::Texture* _quit;
	Uint8* _prevKeyState;
	LevelLoader _levelLoader;
	LevelRenderer _levelRenderer;
	std::shared_ptr<Level> _levelOne;
};