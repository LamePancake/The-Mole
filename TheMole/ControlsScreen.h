#pragma once
#include "Screen.h"
#include "Vector2.h"

class GameManager;

class ControlsScreen :public Screen
{
public:
	virtual int Load() override;
	virtual int Update(double elasepdSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

private:

	GameManager* _mgr;
	SDL2pp::Texture* _backgroundTexture;
	SDL2pp::Texture* _back;
	SDL2pp::Texture* _border;
	SDL2pp::Texture* _controls;
	SDL2pp::Texture* _controlsImage;

	SDL2pp::Font* _font;
	SDL2pp::Font* _headerFont;
};

