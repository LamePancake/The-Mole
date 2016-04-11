#pragma once
#include "Screen.h"
#include "SpriteSheet.h"
#include "Vector2.h"
#include "SoundEffectBank.h"

class GameManager;

class OpeningScreen :public Screen
{
public:

	OpeningScreen(SoundEffectBank & effectBank)
		: _soundBank(effectBank), _currScreen(0) {}

	virtual int Load() override;
	virtual int Update(double elasepdSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

	SoundEffectBank & GetSoundBank();

private:

	GameManager* _mgr;
	SDL2pp::Texture* _backgroundTexture;
	SDL2pp::Texture* _backgroundTexture2;
	SDL2pp::Font* _font;

	std::shared_ptr<SpriteSheet> _enter;

	std::vector<std::vector<SDL2pp::Texture*>> _storyTexture;
	int _currScreen;

	SoundEffectBank & _soundBank;
};

