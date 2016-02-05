#ifndef DEN_LEVEL_SCREEN_H
#define DEN_LEVEL_SCREEN_H
#include <cstring>
#include "GameScreen.h"

class GameManager;

/**
 * @brief	A den level screen.
 *
 * @author	Shane/Matt
 * @date	2/2/2016
 */
class DenLevelScreen : public GameScreen {
public:
	virtual int Load() override;
	virtual int Update(double elapsedSecs) override;
	virtual void Draw() override;
	virtual void Unload() override;

private:
	Camera* _camera;
	//GameManager* _mgr;
	//Uint8* _prevKeyState;
	//LevelLoader _levelLoader;
	//LevelRenderer _levelRenderer;
	//std::shared_ptr<Level> _level;
	std::shared_ptr<Actor> _player;
	std::shared_ptr<SDL2pp::Texture> _background;
};

#endif
