#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <cstring>
#include <string>
#include "Screen.h"
#include "LevelLoader.h"
#include "Level.h"
#include "LevelRenderer.h"
#include "SpriteSheet.h"
#include "Actor.h"

class GameScreen :
	public Screen
{
public:
	GameScreen(std::string levelPath, std::string backgroundPath, std::string winScreenPath, std::string loseScreenPath, std::string nextLevel)
		: _levelPath(levelPath), _backgroundPath(backgroundPath), _winScreenPath(winScreenPath), _loseScreenPath(loseScreenPath), _nextLevel(nextLevel) {}
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

protected:
	std::string _levelPath;
	std::string _backgroundPath;
	std::string _winScreenPath;
	std::string _loseScreenPath;
	std::string _nextLevel;

	Camera* _camera;
	GameManager* _mgr;
	Uint8* _prevKeyState;
	LevelLoader _levelLoader;
	LevelRenderer _levelRenderer;
	std::shared_ptr<Level> _level;
	std::shared_ptr<PlayerActor> _player;
	std::shared_ptr<SDL2pp::Texture> _background;
	std::shared_ptr<SDL2pp::Texture> _winScreen;
	std::shared_ptr<SDL2pp::Texture> _loseScreen;
	std::shared_ptr<SDL2pp::Texture> _pancake;
};

#endif
