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
	 * Constructor that initializes _position to the input parameter.
	 *
	 * @param	position	   	starting position of the agent.
	 * @param [in,out]	manager	game manager.
	 * @param	spd			   	speed.
	 * @param	texturePath	   	Full pathname of the texture file.
	 */
	AIActor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath) 
		: Actor(position, manager, spd, texturePath, 8) {}

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
