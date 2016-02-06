#pragma once
#include "Actor.h"
#include "Math.h"

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
	PlayerActor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath) 
		: Actor(position, manager, spd, texturePath, 8), _maxJumpVel(0), _jumpVelocity(0), _atGoal(false) {}

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

	/**
	 * Collision check.
	 *
	 * @param [in,out]	otherAI	The other an i.
	 *
	 * @return	true if it succeeds, false if it fails.
	 */
	bool CollisionCheck(Actor &otherAI);

	/**
	 * Query if this object is dead.
	 *
	 * @return	true if dead, false if not.
	 */
	bool IsDead();

	/**
	 * Determines if player is at the goal.
	 *
	 * @return	true if it succeeds, false if it fails.
	 */
	bool AtGoal();

	/**
	* Set jump velocity of the player.
	*
	* @param initial velocity in the Y axis.
	**/
	void SetJumpVelocity(float initVel);

	/**
	* Get jump velocity of the player.
	*
	* @return float that is the velocity of the jump in the Y axis.
	**/
	float GetJumpVelocity();

	/**
	* Set max jump velocity of the player.
	*
	* @param initial velocity in the Y axis.
	**/
	void SetMaximumJumpVelocity(float initVel);

	/**
	* Get max jump velocity.
	*
	* @return float that is the velocity of the jump in the Y axis.
	**/
	float GetMaximumJumpVelocity();
private:
	bool _atGoal;
	float _jumpVelocity;
	float _maxJumpVel;
};