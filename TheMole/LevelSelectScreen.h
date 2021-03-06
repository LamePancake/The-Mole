#pragma once
#include "Screen.h"
#include "SpriteSheet.h"
#include "Vector2.h"
#include "SoundEffectBank.h"

#define NUM_LEVELS 6

class GameManager;

class LevelSelectScreen :public Screen
{
public:

	LevelSelectScreen(SoundEffectBank & effectBank)
		: _soundBank(effectBank) {}

	virtual int Load() override;
	virtual int Update(double elasepdSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

	SoundEffectBank & GetSoundBank();

private:

	GameManager* _mgr;
	int _curMenuItem;
	SDL2pp::Texture* _backgroundTextures[NUM_LEVELS];
	SDL2pp::Texture* _levels[NUM_LEVELS];
	SDL2pp::Texture* _descriptions[NUM_LEVELS];
	SDL2pp::Texture* _levelSelect;
	SDL2pp::Texture* _lock;
	SDL2pp::Texture* _optionBorder;
	SDL2pp::Texture* _controls;
	SDL2pp::Texture* _unknown;

	std::shared_ptr<SpriteSheet> _borin;
	Vector2 _borinPosition;
	bool _delay;
	double _timer;
	std::string _nextScreen;

	SDL2pp::Font* _font;
	SDL2pp::Font* _descFont;
	SDL2pp::Font* _headerFont;

	SoundEffectBank & _soundBank;
};

