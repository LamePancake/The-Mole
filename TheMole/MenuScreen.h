#pragma once
#include "Screen.h"
#include "SpriteSheet.h"
#include "Vector2.h"
#include "SoundEffectBank.h"

#define NUM_MENU_ITEMS 4

class GameManager;

class MenuScreen : public Screen {
public:

	MenuScreen(SoundEffectBank & effectBank)
		: _soundBank(effectBank) {}

	virtual int Load() override;
	virtual int Update(double elasepdSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

	SoundEffectBank & GetSoundBank();

private:

	GameManager* _mgr;
	int _curMenuItem;
	SDL2pp::Texture* _menuItems[NUM_MENU_ITEMS];
	SDL2pp::Texture* _background;
	SDL2pp::Texture* _menuBorder;
	SDL2pp::Texture* _controls;
	SDL2pp::Texture* _devLogo;
    SDL2pp::Music* _menuTheme;
	SDL2pp::Font* _font;

	std::shared_ptr<SpriteSheet> _logo;
	std::shared_ptr<SpriteSheet> _borin;
	Vector2 _borinPosition;
	bool _delay;
	bool _update;
	float _direction;
	double _timer;

	std::string _nextScreen;

	SoundEffectBank & _soundBank;
};