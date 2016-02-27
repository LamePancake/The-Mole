#ifndef AIACTOR_H
#define AIACTOR_H
#include "Actor.h"
#include "Tile.h"

#pragma once

/**
* This is the simple version of the state machine. It uses a switch instead of states, transitions, conditions, actions.
**/
class AIActor : public Actor
{
public:

	/**
	* @brief	Creates a new AI actor with the given parameters.
	*
	* @author	Shane
	* @date	2/8/2016
	*
	* @param	position	    The AI's start position.
	* @param [in,out]	manager	A reference to the game manager.
	* @param	spd			   	The AI's starting speed.
	* @param [in,out]	sprites	A named list of sprite sheets that can be used to draw the AI.
	* @param	startSprite	   	The default sprite sheet to draw.
	* @param	startXDirection	The actor's default facing direction along the x axis.
	* @param	startYDirection	The actor's default facing direction along the y axis.
	*/
	AIActor(Vector2 position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites, const std::string&& startSprite,
		SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT, SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP)
		: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection), _underControl{ false }, _controlTimeLeft{ 0.0f }, _isSelected{ false }, _isCandidate{ false } {}

	/** Destructor. */
	~AIActor();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Sets whether this AI is currently selected for mind control.</summary>
	///
	///<param name="selected"> Whether the AI is selected for mind control.</param>
	///
	///### <author> Shane.</author>
	///### <date> 2/26/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetSelectedForControl(bool selected);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Sets whether this AI a candidate for mind control.</summary>
	///
	///<param name="selected"> Whether the AI is a candidate for mind control.</param>
	///
	///### <author> Shane.</author>
	///### <date> 2/26/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetIsMindControlCandidate(bool isCandidate);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Sets whether this AI is under mind control.</summary>
	///
	///<param name="controlled"> Whether the AI is under mind control.</param>
	///<param name="direction"> If the AI is under control, the direction it should move. Ignored if controlled is false.</param>
	///
	///### <author> Shane.</author>
	///### <date> 2/26/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetMindControlProperties(bool controlled, SpriteSheet::XAxisDirection direction);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets whether this AI is under mind control and in which direction if so.</summary>
	///
	///<param name="controlled"> [out] Whether the AI is under mind control.</param>
	///<param name="direction">  [out] If the AI is under control, the direction it should move.</param>
	///
	///### <author> Shane.</author>
	///### <date> 2/26/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void GetMindControlProperties(bool& controlled, SpriteSheet::XAxisDirection& direction) const;

	// All the state changing stuff happens in here.
	virtual void Update(double elapsedSecs) override;

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition(double elapsedSecs);

	/**
	 * Draws.
	 *
	 * @param [in,out]	camera	ref to the camera.
	 * 
	 * Draws this object.
	 */
	virtual void Draw(Camera& camera) override;

	/**
	* Check collision against another SimpleAgent
	*
	* @return true if collided, false if not
	**/
	bool CollisionCheck(Actor & otherAI);

	/**
	* Scans tiles left and right of the agent and negate _speed if left or right neighbour is not blank tile
	*/
	void ScanNeighbouringTiles(std::shared_ptr<Level> & level);

private:
	
	const Uint8 _normalShadow[3] = { 127, 127, 127 };
	const Uint8 _normalColour[3] = { 255, 255, 255 };

	const Uint8 _mindCtrlShadow[3] = { 95, 127, 95 };
	const Uint8 _mindCtrlColour[3] = { 190, 255, 190 };

	// Mind control properties
	bool _underControl;
	float _controlTimeLeft;
	SpriteSheet::XAxisDirection _controlDir;

	// Mind control selection properties (when the player is selecting whom to control)
	bool _isSelected;
	bool _isCandidate;
};

#endif
