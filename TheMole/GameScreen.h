#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <cstring>
#include <string>
#include <cstdint>
#include "Screen.h"
#include "LevelLoader.h"
#include "Level.h"
#include "LevelRenderer.h"
#include "SpriteSheet.h"
#include "Actor.h"
#include "SoundEffectBank.h"

class GameScreen :
	public Screen
{
public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Creates a new player actor with the given parameters.</summary>
	///
	///<param name="levelPath">      Path to the text file containing the level.</param>
	///<param name="backgroundPath"> Path to the file containing the background image.</param>
	///<param name="nextLevel">      String indicating the next screen/level to load when this one is finished.</param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	GameScreen(std::string levelPath, std::string backgroundPath, std::string nextLevel, SoundEffectBank & effectBank)
		: _levelPath(levelPath), _backgroundPath(backgroundPath), _nextLevel(nextLevel), _soundBank(effectBank) {}
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
	 * @return	The level associated with this GameScreen.
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
	std::string _nextLevel;

	Camera* _camera;
	GameManager* _mgr;
	Uint8* _prevKeyState;
	LevelLoader _levelLoader;
	LevelRenderer _levelRenderer;
	SoundEffectBank & _soundBank;
	std::shared_ptr<Level> _level;
	std::shared_ptr<PlayerActor> _player;
	std::shared_ptr<SDL2pp::Texture> _background;
};

#endif
