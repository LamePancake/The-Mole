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
	 * @brief	Draws the sprite at the given position.
	 *
	 * @author	Shane
	 * @date	2/3/2016
	 *
	 * @param position	The position (upper-left corner) to draw the sprite.
	 */
	void Draw(const SDL2pp::Point&& position);

	/**
	 * @brief	Draws at the given position.
	 *
	 * @author	Shane
	 * @date	2/3/2016
	 *
	 * @param position	The position (upper-left corner) to draw the sprite.
	 */
	void Draw(const SDL2pp::Point& position);

	/**
	 * @brief	Query if this object is animating.
	 *
	 * @author	Shane
	 * @date	2/7/2016
	 *
	 * @return	true if animating, false if not.
	 */
	bool IsAnimating() const;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Query whether the sprite sheet is finished its animation.</summary>
	///
	///<remarks>If the sprite sheet is reversed, it's finished when it's at the first frame; otherwise,
	///         it's finished when it's at the last frame. Note that sprites set to repeating will never
	///         finish.</remarks>
	///
	///<returns> Whether the sprite sheet is finished its animation. false if IsRepeating().</returns>
	///
	///#<author>Shane</author>
	///#<date>3/5/2016</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	bool IsFinished() const;

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

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Sets whether the animation is playing in reverse.</summary>
	///
	///<returns> Whether the animation is playing in reverse.</param>
	///
	///<remarks> Shane, 3/5/2016.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	bool IsReversed() const;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Sets whether the animation should play in reverse.</summary>
	///
	///<param name="direction"> Whether the animation should play in reverse.</param>
	///
	///<remarks> Shane, 3/5/2016.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetReversed(bool reversed);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Sets the current x axis direction for this sprite sheet.</summary>
	///
	///<param name="direction"> The new x axis direction.</param>
	///
	///<remarks> Shane, 3/5/2016.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetXAxisDirection(XAxisDirection direction);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets the x axis direction that the sprite is facing.</summary>
	///
	///<returns> The x axis direction that the sprite is facing</returns>
	///
	///<remarks> Shane, 3/5/2016.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	XAxisDirection GetXAxisDirection() const;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Sets the current y axis direction for this sprite sheet.</summary>
	///
	///<param name="direction"> The new y axis direction.</param>
	///
	///<remarks> Shane, 3/5/2016.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetYAxisDirection(YAxisDirection direction);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets the y axis direction that the sprite is facing.</summary>
	///
	///<returns> The y axis direction that the sprite is facing</returns>
	///
	///<remarks> Shane, 3/5/2016.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	YAxisDirection GetYAxisDirection() const;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Sets the sprite's rotation about the given centre point.</summary>
	///
	///<param name="angle"> The amount by which to rotate the sprite about the centre, in degrees.</param>
	///
	///<remarks> Shane, 3/5/2016.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetRotation(double angle);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets the angle of rotation that will be applied to the sprite.</summary>
	///
	///<returns> The angle of rotation that will be applied to the sprite in degrees.</returns>
	///
	///<remarks> Shane, 3/5/2016.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	double GetRotation() const;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Sets the colour and alpha mod to apply to the sprite.</summary>
	///
	///<param name="colourMod"> The colour mod to apply.</param>
	/// 
	///<remarks> Shane, 3/5/2016.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetColourMod(SDL_Color colourMod);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets the colour and alpha mod to apply to the sprite.</summary>
	///
	///<returns> The colour and alpha mod to apply to the sprite.</returns>
	///
	///<remarks> Shane, 3/5/2016.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	SDL_Color GetColourMod() const;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Scales the sprite by the specified amount about its centre.</summary>
	///
	///<param name="scale"> The scale to apply.</param>
	///
	///<remarks> Shane, 3/5/2016.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetScale(double scale);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets the scale to apply to the sprite.</summary>
	///
	///<returns> The scale to apply to the sprite.</returns>
	///
	///<remarks> Shane, 3/5/2016.</remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	double GetScale() const;

private:
	std::shared_ptr<SDL2pp::Texture> _sheet;
	SDL2pp::Point _spriteSize;

	bool _isRunning;
	bool _isRepeating;
	bool _isReversed;
	int _currentFrame;
	int _numFrames;
	double _frameTime;
	double _currentFrameElapsed;

	GameManager* _mgr;
	
	XAxisDirection _defaultXDir;
	YAxisDirection _defaultYDir;
	XAxisDirection _currentXDir;
	YAxisDirection _currentYDir;

	double _scale;
	double _rotation;
	SDL_Color _colourMod;
};

#endif
