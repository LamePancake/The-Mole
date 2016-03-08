#pragma once
#include "Screen.h"

#define NUM_MENU_ITEMS 3

class GameManager;

class MenuScreen : public Screen {
public:
	virtual int Load() override;
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
    SDL2pp::Music* _menuTheme;
	Uint8* _prevKeyState;

protected:
	virtual void Reset() override;
};