#pragma once
#include "Screen.h"
#include "SpriteSheet.h"
#include "Vector2.h"
#include "SoundEffectBank.h"

class GameManager;

class ClosingScreen :public Screen
{
public:

	ClosingScreen(SoundEffectBank & effectBank)
		: _soundBank(effectBank), _currDraw(0), _currPerson(0), _timer(0.0){}

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

	std::shared_ptr<SpriteSheet> _borinAndTheChicken;

	std::vector<std::vector<SDL2pp::Texture*>> _creditsTexture;
	std::vector<SDL2pp::Texture*> _drawnCredit;

	double _timer;

	int _currPerson;
	int _currDraw;
	SoundEffectBank & _soundBank;
};

