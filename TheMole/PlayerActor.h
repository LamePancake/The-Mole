#pragma once
#include "Actor.h"
#include "Math.h"

class PlayerActor : public Actor
{
public:

	/**
	 * Constructor that initializes _position to the input parameter.
	 *
	 * @param	position		 	starting position of the agent.
	 * @param [in,out]	manager  	game manager.
	 * @param	spd				 	speed.
	 * @param	texturePath		 	Full pathname of the texture file.
	 * @param	shadowTexturePath	Full pathname of the shadow texture file.
	 */
	PlayerActor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath, std::string shadowTexturePath) : Actor(position, manager, spd, texturePath, shadowTexturePath) {}

	/** Destructor. */
	~PlayerActor();

	/**
	 * Draws.
	 *
	 * @param [in,out]	level	The level.
	 *
	 * 	Draws this object.
	 */
	virtual void Draw(Camera& camera) override;

	// All the state changing stuff happens in here.
	virtual void Update(double elapsedSecs, std::shared_ptr<Level> & level);

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition(double elapsedSecs);

	/**
	 * Digs the given level.
	 *
	 * @param	dir			 	The dir.
	 * @param [in,out]	level	The level.
	 *
	 * @return	true if it succeeds, false if it fails.
	 */
	bool Dig(char dir, std::shared_ptr<Level> & level);

private:

};