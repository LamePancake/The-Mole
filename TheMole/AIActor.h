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
	AIActor(SDL2pp::Point position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites, const std::string&& startSprite,
		SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT, SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP)
		: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection) {}

	/** Destructor. */
	~AIActor();

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
};

#endif
