#pragma once
#include "Actor.h"

class NPCActor : public Actor
{
public:

	/**
	 * Constructor that initializes _position to the input parameter.
	 *
	 * @param	position	   	starting position of the agent.
	 * @param [in,out]	manager	game manager.
	 * @param	spd			   	speed.
	 * @param	texturePath	   	Full pathname of the texture file.
	 *
	 */
	NPCActor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath)
		: Actor(position, manager, spd, texturePath, 6) {}

	/** Destructor. */
	~NPCActor();

	/**
	* Draws.
	*
	* @param [in,out]	level	The level.
	*
	* 	Draws this object.
	*/
	virtual void Draw(Camera& camera) override;

	// All the state changing stuff happens in here.
	virtual void Update(double elapsedSecs) override;

private:
	//Will probably store dialog here.
};