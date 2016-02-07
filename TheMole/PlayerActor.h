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
	PlayerActor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath);

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
	virtual void Update(double elapsedSecs) override;

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition(double elapsedSecs);

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

	/**
	 * @brief	Updates all player properties influenced by the user's input.
	 *
	 * @author	Shane
	 * @date	2/6/2016
	 */
	void UpdateInput();

	/**
	 * @brief	Updates all collisions (with tiles and enemies).
	 *
	 * @author	Shane	 
	 * @date	2/6/2016
	 * @editor  Tim (added parameter)
	 * @date    2/6/2016
	 */
	void UpdateCollisions(double elapsedSecs);

	bool _atGoal;
	float _jumpVelocity;
	float _maxJumpVel;
	char _digDir[2];
	bool _jumped;

	bool _isDigging;

	/** The sprite shadow digging. */
	std::shared_ptr<SpriteSheet> _spriteSideDigShadow;

	/** The spritesheet for digging. */
	std::shared_ptr<SpriteSheet> _spriteSideDig;

	/** The sprite shadow digging. */
	std::shared_ptr<SpriteSheet> _spriteVerticalDigShadow;

	/** The spritesheet for digging. */
	std::shared_ptr<SpriteSheet> _spriteVerticalDig;

	/** The sprite shadow digging. */
	std::shared_ptr<SpriteSheet> _spriteWalkShadow;

	/** The spritesheet for digging. */
	std::shared_ptr<SpriteSheet> _spriteWalk;

	/** The current sprite animating. */
	std::shared_ptr<SpriteSheet> _currentSpriteSheet;

	/** The current shadow animating */
	std::shared_ptr<SpriteSheet> _currentSpriteSheetShadow;
};