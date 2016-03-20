#pragma once
#include "Screen.h"
#include "SpriteSheet.h"
#include "Vector2.h"
#include "SoundEffectBank.h"

#define NUM_DEVELOPERS 4

class GameManager;

class CreditsScreen :public Screen
{
public:

	CreditsScreen(SoundEffectBank & effectBank)
		: _soundBank(effectBank) {}

	virtual int Load() override;
	virtual int Update(double elasepdSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

	SoundEffectBank & GetSoundBank();

private:

	GameManager* _mgr;
	SDL2pp::Texture* _backgroundTexture;
	SDL2pp::Texture* _developers[NUM_DEVELOPERS];
	SDL2pp::Texture* _back;
	SDL2pp::Texture* _border;
	SDL2pp::Texture* _controls;
	SDL2pp::Texture* _credits;

	std::shared_ptr<SpriteSheet> _chicken;
	Vector2 _chickenPosition;
	bool _delay;
	double _timer;
	float _direction;

	SDL2pp::Font* _font;
	SDL2pp::Font* _headerFont;

	SoundEffectBank & _soundBank;
};
