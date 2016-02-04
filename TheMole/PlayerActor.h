#pragma once
#include "Actor.h"

class PlayerActor : public Actor
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
	PlayerActor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath) : Actor(position, manager, spd, texturePath) {}

	/** Destructor. */
	~PlayerActor();

	/** Draws this object. */
	virtual void Draw(std::shared_ptr<Level> & level);

	// All the state changing stuff happens in here.
	virtual void Update(std::shared_ptr<Level> & level);

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition();

private:

};