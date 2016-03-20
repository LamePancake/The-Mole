#pragma once
#include "Screen.h"
#include "SpriteSheet.h"
#include "Vector2.h"
#include "SoundEffectBank.h"

class GameManager;


#define NUM_OPTIONS 3

class OptionsScreen :public Screen
{
public:

	OptionsScreen(SoundEffectBank & effectBank)
		: _soundBank(effectBank) {}

	virtual int Load() override;
	virtual int Update(double elasepdSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

	SoundEffectBank & GetSoundBank();

private:

	GameManager* _mgr;
	int _curMenuItem;
	SDL2pp::Texture* _backgroundTexture;
	SDL2pp::Texture* _optionsText;
	SDL2pp::Texture* _border;
	SDL2pp::Texture* _controls;
	SDL2pp::Texture* _options[NUM_OPTIONS];
	SDL2pp::Texture* _areYouSure;
	SDL2pp::Texture* _yesOrNo[2];
	SDL2pp::Texture* _dataHasBeenCleared;

	SDL2pp::Font* _font;
	SDL2pp::Font* _headerFont;
	SDL2pp::Font* _warningFont;

	bool _confirmClearing;
	bool _dataErased;
	int _curEraseMenuItem;

	SoundEffectBank & _soundBank;
};

