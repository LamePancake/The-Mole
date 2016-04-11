#pragma once
#include "Screen.h"
#include "SoundEffectBank.h"
#include "SpriteSheet.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

class GameManager;

#define PROTAG 0
#define NPC 1

class CutsceneScreen : public Screen
{

public:

	CutsceneScreen(std::string dialogBoxPath, std::string backgroundPath, 
		std::string protagSprite, double protagNumFrames, double protagDuration, 
		std::string npcSpritePath, double npcNumFrames, double npcDuration, 
		SoundEffectBank & effectBank, std::string nextScreen, std::string dialogFilePath, std::string protagName, std::string npcName,
		std::vector<std::string>& protagVoices, std::vector<std::string>& npcVoices, std::string musicPath)
		: _dialogBoxPath(dialogBoxPath), _backgroundPath(backgroundPath), 
		  _protagSpritePath(protagSprite), _protagNumFrames(protagNumFrames), _protagDuration(protagDuration),
		  _npcSpritePath(npcSpritePath), _npcNumFrames(npcNumFrames), _npcDuration(npcDuration), 
		  _nextScreen(nextScreen), _soundBank(effectBank), _dialogFilePath(dialogFilePath), _skipTimer(0),
		  _currentlySpeaking(0), _currentProtagDialog(" "), _currentNPCDialog(" "), _dialogIndex(0),
		  _protagName(protagName), _npcName(npcName), _protagVoices{ protagVoices }, _npcVoices{ npcVoices }, _musicPath(musicPath){}
	
	virtual int Load() override;
	virtual int Update(double elasepdSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

	SoundEffectBank & GetSoundBank();

    virtual ~CutsceneScreen();

private:

	SoundEffectBank & _soundBank;

	std::string _backgroundPath;
	std::string _dialogBoxPath;
	std::string _protagSpritePath;
	std::string _npcSpritePath;
	std::string _nextScreen;
	std::string _dialogFilePath;

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
	std::shared_ptr<SpriteSheet> _nextDialogProtag;
	std::shared_ptr<SpriteSheet> _nextDialogNPC;

	std::vector<std::string> _dialog;
	std::vector<std::string> _protagVoices;
	std::vector<std::string> _npcVoices;

	double _skipTimer;

	SDL2pp::Font* _headerFont;
	SDL2pp::Font* _dialogFont;
	SDL2pp::Font* _promptFont;

	std::string _protagName;
	std::string _npcName;

	int _currentlySpeaking;
	std::string _currentProtagDialog;
	std::string _currentNPCDialog;
	int _dialogIndex;

	std::vector<std::string> OpenDialog(std::string dialogFilePath);
	void UpdateDialog();

	std::string _musicPath;
};

