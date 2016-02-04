#ifndef DEN_LEVEL_SCREEN_H
#define DEN_LEVEL_SCREEN_H
#include <cstring>
#include "Screen.h"
#include "LevelLoader.h"
#include "Level.h"
#include "LevelRenderer.h"
#include "Actor.h"

class GameManager;

/**
 * @brief	A den level screen.
 *
 * @author	Shane
 * @date	2/2/2016
 */
class DenLevelScreen : public Screen {
public:
	virtual int Load() override;
	virtual int Update(double elasepdSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

private:
	GameManager* _mgr;
	Uint8* _prevKeyState;
	LevelLoader _levelLoader;
	LevelRenderer _levelRenderer;
	std::shared_ptr<Level> _level;
	std::shared_ptr<Actor> _player;
};

#endif
