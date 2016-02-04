#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <SDL2pp\SDL2pp.hh>
#include <string>
#include <cstdio>
#include "GameManager.h"

class SpriteSheet {

public:

	/**
	 * @brief	Constructor.
	 *
	 * @author	Shane
	 * @date	2/3/2016
	 *
	 * @param 	filename	Filename of the spritesheet file.
	 * @param	numFrames	The number of frames in the animation.
	 * @param	duration	The duration (in seconds) for the animation.
	 */
	SpriteSheet(std::string& filename, int numFrames, double duration);

	/**
	 * @brief	Updates the sprite sheet based on the current elapsed time.
	 *
	 * @author	Shane
	 * @date	2/3/2016
	 *
	 * @param	elapsedSecs	The elapsed time in seconds.
	 */
	void Update(double elapsedSecs);

	/**
	 * @brief	Draws the given position.
	 *
	 * @author	Shane
	 * @date	2/3/2016
	 *
	 * @param [in]	position	The position to draw the sprite.
	 */
	void Draw(const SDL2pp::Point&& position);

	/**
	 * @brief	Immediately resets the animation to the first frame.
	 *
	 * @author	Shane
	 * @date	2/3/2016
	 */
	void Reset();

private:
	SDL2pp::Texture _sheet;
	SDL2pp::Point _spriteSize;
	int _currentFrame;
	int _numFrames;
	double _frameTime;
	double _currentFrameElapsed;
	GameManager* _mgr;
};

#endif
