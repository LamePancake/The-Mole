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
		LEFT,
	} XAxisDirection;

	typedef enum {
		UP,
		DOWN,
	} YAxisDirection;

	/**
	 * Constructor.
	 *
	 * @author	Shane
	 * @date	2/3/2016
	 *
	 * @param 	spritesheetFile			Name of the file containing the sprite sheet. Must be in a format supported by SDL_Image.
	 * @param	numFrames				The number of frames in the animation.
	 * @param	duration				The duration (in seconds) for the animation.
	 * @param	isRepeating				Whether the animation automatically repeats. Defaults to true.
	 * @param	defaultXAxisDirection	The default direction that the sprite is facing in the x-axis.
	 * @param	defaultYAxisDirection	The default direction that the sprite is facing in the y-axis.
	 */
	SpriteSheet(std::string&& spritesheetFile, int numFrames, double duration, bool isRepeating = true,
				XAxisDirection defaultXAxisDirection = XAxisDirection::RIGHT, YAxisDirection defaultYAxisDirection = YAxisDirection::UP);

	/**
	* Constructor.
	*
	* @author	Shane
	* @date	2/3/2016
	*
	* @param 	texture					Pointer to a texture containing the sprite sheet's animation frames.
	* @param	numFrames				The number of frames in the animation.
	* @param	duration				The duration (in seconds) for the animation.
	* @param	isRepeating				Whether the animation automatically repeats. Defaults to true.
	* @param	defaultXAxisDirection	The default direction that the sprite is facing in the x-axis.
	* @param	defaultYAxisDirection	The default direction that the sprite is facing in the y-axis.
	*/
	SpriteSheet(std::shared_ptr<SDL2pp::Texture>& texture, int numFrames, double duration, bool isRepeating = true,
		XAxisDirection defaultXAxisDirection = XAxisDirection::RIGHT, YAxisDirection defaultYAxisDirection = YAxisDirection::UP);

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
	 * @param position	The position (upper-left corner) to draw the sprite.
	 * @param xAxisDir	The direction that the sprite should face in the x axis (XAxisDirection::RIGHT or XAxisDirection::LEFT).
	 * @param yAxisDir	The direction that the sprite should face in the y axis (YAxisDirection::UP or YAxisDirection::DOWN).
	 */
	void Draw(const SDL2pp::Point&& position, XAxisDirection xAxisDir, YAxisDirection yAxisDir);

	/**
	 * @brief	Query if this object is animating.
	 *
	 * @author	Shane
	 * @date	2/7/2016
	 *
	 * @return	true if animating, false if not.
	 */
	bool IsAnimating() const;

	/**
	 * @brief	Starts or resumes the sprite sheet's animation. If the sheet is already animating, this does nothing.
	 *
	 * @author	Shane
	 * @date	2/5/2016
	 */
	void Start();

	/**
	 * @brief	Pauses the sheet's animation. Has no effect if the sheet is already paused or stopped.
	 *
	 * @author	Shane
	 * @date	2/5/2016
	 */
	void Pause();

	/**
	 * @brief	Stops and Reset()s the sprite sheet's animation.
	 *
	 * @author	Shane
	 * @date	2/5/2016
	 */
	void Stop();

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
	 * Gets the texture associated with the sprite.
	 *
	 * @return	The texture.
	 */
	SDL2pp::Texture& GetTexture();

	/**
	 * @brief	Determine if this sprite sheet's animation automatically loops.
	 * 
	 * @return	true if repeating, false if not.
	 */
	bool IsRepeating();

	/**
	 * @brief	Sets whether this sprite sheet's animation should automatically loop.
	 *
	 * @param	repeating	true to repeating.
	 */
	void SetRepeating(bool repeating);

private:
	std::shared_ptr<SDL2pp::Texture> _sheet;
	SDL2pp::Point _spriteSize;
	bool _isRunning;
	int _currentFrame;
	int _numFrames;
	double _frameTime;
	double _currentFrameElapsed;
	GameManager* _mgr;
	XAxisDirection _defaultXDir;
	YAxisDirection _defaultYDir;
	bool _isRepeating;
};

#endif
