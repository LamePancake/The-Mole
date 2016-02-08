#pragma once
#include "Actor.h"
#include "Math.h"

class PlayerActor : public Actor
{
public:

	/**
	 * @brief	Creates a new player actor with the given parameters.
	 *
	 * @author	Shane
	 * @date	2/8/2016
	 *
	 * @param	position	    The player's start position.
	 * @param [in,out]	manager	A reference to the game manager.
	 * @param	spd			   	The player's starting speed.
	 * @param	sprites			The list of sprite sheets for the player.
	 * @param	startSprite	   	The default sprite to draw.
	 * @param	startXDirection	The actor's default facing direction along the x axis.
	 * @param	startYDirection	The actor's default facing direction along the y axis.
	 */
	PlayerActor(Vector2 position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites, const std::string&& startSprite,
		SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT, SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP);

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
	 * 
	 * @editor  Tim (added parameter)
	 * @date    2/6/2016
	 */
	void UpdateCollisions(double elapsedSecs);

	/**
	 * @brief	Handles collisions with tiles.
	 *
	 * @author	Shane
	 * @date	2/7/2016
	 */
	void DefaultTileCollisionHandler(std::vector<std::shared_ptr<Tile>>& tiles, Edge edge, float correctedPos);

	bool _atGoal;
	float _jumpVelocity;
	float _maxJumpVel;
	char _digDir[2];
	bool _jumped;

	SpriteSheet::XAxisDirection _prevDirection;

	bool _isDigging;
};