#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <SDL2pp\SDL2pp.hh>
#include <string>
#include <cstdio>
#include "GameManager.h"

class SpriteSheet {

public:

	typedef enum {
		RIGHT,
		LEFT
	} XAxisDirection;

	/**
	 * @brief	Constructor.
	 *
	 * @author	Shane
	 * @date	2/3/2016
	 *
	 * @param [in,out]	filename	  	Filename of the spritesheet file.
	 * @param	numFrames			  	The number of frames in the animation.
	 * @param	duration			  	The duration (in seconds) for the animation.
	 * @param	defaultFacingDirection	The default direction that the sprite is facing.
	 */
	SpriteSheet(std::string& filename, int numFrames, double duration, XAxisDirection defaultFacingDirection);

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
	void Draw(const SDL2pp::Point&& position, XAxisDirection facingDir);

	/**
	 * @brief	Immediately resets the animation to the first frame.
	 *
	 * @author	Shane
	 * @date	2/3/2016
	 */
	void Reset();

	/**
	 * @brief	Gets the width of one animation frame.
	 *
	 * @author	Shane
	 * @date	2/5/2016
	 *
	 * @return	The frame width.
	 */
	int GetFrameWidth();

	/**
	 * @brief	Gets the height of one animation frame.
	 *
	 * @author	Shane
	 * @date	2/5/2016
	 *
	 * @return	The frame height.
	 */
	int GetFrameHeight();

	/**
	 * Gets the texture.
	 *
	 * @return	The texture.
	 */
	SDL2pp::Texture& GetTexture();

private:
	SDL2pp::Texture _sheet;
	SDL2pp::Point _spriteSize;
	int _currentFrame;
	int _numFrames;
	double _frameTime;
	double _currentFrameElapsed;
	GameManager* _mgr;
	XAxisDirection _defaultFacing;
};

#endif
