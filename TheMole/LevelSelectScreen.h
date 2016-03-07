#pragma once
#include "Screen.h"

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
	SDL2pp::Texture* _levelItems[NUM_LEVELS];
	
	SDL2pp::Texture* _denLevel;
	SDL2pp::Texture* _vikingLevel;
	SDL2pp::Texture* _grassLevel;
	SDL2pp::Texture* _starscapeLevel;
	SDL2pp::Texture* _bossLevel;
	SDL2pp::Texture* _back;
};

