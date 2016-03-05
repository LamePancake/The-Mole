#pragma once
#include "Screen.h"
#include "SoundEffectBank.h"
#include "SpriteSheet.h"

class GameManager;

#define PROTAG 0
#define NPC 1

class CutsceneScreen : public Screen
{

public:

	CutsceneScreen(std::string dialogPath, std::string backgroundPath, std::string openingSprite, double openingNumFrames, double openingDuration,
		std::string protagSprite, double protagNumFrames, double protagDuration, std::string npcSpritePath, double npcNumFrames, double npcDuration, SoundEffectBank & effectBank, std::string nextScreen, std::string dialogFile)
		: _dialogPath(dialogPath), _backgroundPath(backgroundPath), 
		  _openingSpriteSheetPath(openingSprite), _openingNumFrames(openingNumFrames), _openingDuration(openingDuration),
		  _protagSpritePath(protagSprite), _protagNumFrames(protagNumFrames), _protagDuration(protagDuration),
		  _npcSpritePath(npcSpritePath), _npcNumFrames(npcNumFrames), _npcDuration(npcDuration), 
		  _nextScreen(nextScreen), _soundBank(effectBank), _dialogFile(dialogFile), _skipTimer(0), _currentlySpeaking(0) {}
	
	virtual int Load() override;
	virtual int Update(double elasepdSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

private:

	SoundEffectBank & _soundBank;

	std::string _backgroundPath;
	std::string _dialogPath;
	std::string _openingSpriteSheetPath;
	std::string _protagSpritePath;
	std::string _npcSpritePath;
	std::string _nextScreen;
	std::string _dialogFile;

	double _openingNumFrames;
	double _openingDuration;

	double _protagNumFrames;
	double _protagDuration;

	double _npcNumFrames;
	double _npcDuration;

	GameManager* _mgr;

	SDL2pp::Texture* _background;
	SDL2pp::Texture* _protagDialogBox;
	SDL2pp::Texture* _npcDialogBox;
	
	std::shared_ptr<SpriteSheet> _protagonist;
	std::shared_ptr<SpriteSheet> _NPC;
	std::shared_ptr<SpriteSheet> _openingAnimation;

	Uint8* _prevKeyState;

	double _skipTimer;

	SDL2pp::Font* _font;

	int _currentlySpeaking;
};

