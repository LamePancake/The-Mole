#pragma once
#include "Screen.h"
#include "SpriteSheet.h"

#define NUM_LEVELS 6

class GameManager;

class LevelSelectScreen :public Screen
{
public:
	virtual int Load() override;
	virtual int Update(double elasepdSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

private:

	GameManager* _mgr;
	int _curMenuItem;
	SDL2pp::Texture* _backgroundTextures[NUM_LEVELS];
	SDL2pp::Texture* _levels[NUM_LEVELS];
	SDL2pp::Texture* _levelSelect;
	SDL2pp::Texture* _lock;

	std::shared_ptr<SpriteSheet> _borin;

	SDL2pp::Font* _font;
	SDL2pp::Font* _headerFont;
};

