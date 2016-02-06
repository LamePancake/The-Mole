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
	virtual int Load() = 0;
	int Load(std::string levelAddress, std::string textureAddress);
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
	Camera* _camera;
	GameManager* _mgr;
	Uint8* _prevKeyState;
	LevelLoader _levelLoader;
	LevelRenderer _levelRenderer;
	std::shared_ptr<Level> _level;
	std::shared_ptr<PlayerActor> _player;
	std::shared_ptr<SDL2pp::Texture> _background;
};

#endif
