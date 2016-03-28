#ifndef PRE_CUTSCENE_SCREEN_H
#define PRE_CUTSCENE_SCREEN_H

#include <cstring>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include "GameScreen.h"
#include "LevelLoader.h"
#include "Level.h"
#include "LevelRenderer.h"
#include "SpriteSheet.h"
#include "Actor.h"
#include "SoundEffectBank.h"
#include "Quadtree.h"

#define NUM_MENU_ITEMS 3

class PreCutsceneScreen :
	public GameScreen
{
public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Creates a new player actor with the given parameters.</summary>
	///
	///<param name="levelPath">      Path to the text file containing the level.</param>
	///<param name="backgroundPath"> Path to the file containing the background image.</param>
	///<param name="winScreenPath">  Path to the file containing the winning screen image.</param>
	///<param name="loseScreenPath"> Path to the file containing the losing screen image.</param>
	///<param name="nextLevel">      String indicating the next screen/level to load when this one is finished.</param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	PreCutsceneScreen(std::string levelPath, std::string backgroundPath, std::string scorePath, std::string nextLevel, SoundEffectBank & effectBank)
		: GameScreen(levelPath, backgroundPath, scorePath, nextLevel, effectBank), _levelPath(levelPath), _backgroundPath(backgroundPath), _scorePath(scorePath), _nextLevel(nextLevel), _soundBank(effectBank), _paused(false), _deaths(0), _deathCounterUpdated(false), _deathTimer(0) {}

	virtual int Load();
	virtual int Update(double elapasedSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

	/**
	* @brief	Gets the level associated with this GameScreen.
	*
	* @author	Shane
	* @date	2/5/2016
	*
	* @return	The level associated with this GameScreen.d
	*/
	std::shared_ptr<Level> GetLevel() const;

	/**
	* Gets the player.
	*
	* @return	The player.
	*/
	const std::shared_ptr<PlayerActor> GetPlayer() const;

	SoundEffectBank & GetSoundBank();

protected:
	std::string _levelPath;
	std::string _backgroundPath;
	std::string _scorePath;
	std::string _nextLevel;

	bool _paused;
	Camera* _camera;
	GameManager* _mgr;
	LevelLoader _levelLoader;
	LevelRenderer _levelRenderer;
	SoundEffectBank & _soundBank;
	std::shared_ptr<Level> _level;
	std::shared_ptr<Quadtree> _levelQuadtree;
	std::shared_ptr<PlayerActor> _player;
	std::shared_ptr<SDL2pp::Texture> _background;
	std::shared_ptr<SDL2pp::Texture> _pancake;
	std::shared_ptr<SDL2pp::Texture> _pancakeMarker;
	std::shared_ptr<SDL2pp::Texture> _pancakeMarker2;
	std::shared_ptr<SDL2pp::Texture> _skull;

	int _deaths;
	double _deathTimer;
	int _curMenuItem;
	SDL2pp::Texture* _menuItems[NUM_MENU_ITEMS];
	SDL2pp::Texture* _pausedText;
	SDL2pp::Texture* _levelCompleteText;
	SDL2pp::Texture* _gameOverText;
	SDL2pp::Texture* _pressEnterToContinue;
	SDL2pp::Texture* _newRecord;
	SDL2pp::Texture* _deathCounter;
	SDL2pp::Texture* _border;
	SDL2pp::Texture* _controls;
	SDL2pp::Texture* _levelCompleteControls;

	SDL2pp::Font* _font;
	SDL2pp::Font* _headerFont;
	SDL2pp::Font* _recordFont;

	std::stringstream _stringFormatter;

	bool _deathCounterUpdated;

	int OnPauseUpdate();
	void OnPauseDraw();

	void OnLevelCompleteDraw();

	void OnGameOverDraw();
};

#endif