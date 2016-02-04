#pragma once
#include <SDL2pp\SDL2pp.hh>
#include <string>
#include <memory>
#include "Vector2.h"
#include "AABB.h"
#include "GameManager.h"
#include "Level.h"
#include "Camera.h"

class Actor
{
public:
	/**
	* Constructor that initializes _position to the input parameter
	*
	* @param the starting position of the agent
	* @param the game manager
	* @param starting speed
	*/
	Actor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath);

	/** Destructor. */
	~Actor();

	/**
	* Return AABB of the agent
	*
	* @return _aabb
	**/
	AABB GetAABB();

	/**
	 * Returns in the textures needed to render the AI.
	 *
	 * @return	null if it fails, else the texture.
	 */
	std::shared_ptr<SDL2pp::Texture> GetTexture();

	// Returns position of the agent.
	Vector2 GetPosition();

	/**
	 * Gets the speed.
	 *
	 * @return	The speed.
	 */
	Vector2 GetSpeed();

	/**
	 * Gets the health.
	 *
	 * @return	The health.
	 */
	size_t GetHealth();

	/**
	 * Sets a speed.
	 *
	 * @param	spd	The speed.
	 */
	void SetSpeed(Vector2 spd);

	/**
	 * Sets a health.
	 *
	 * @param	heath	The health.
	 */
	void SetHealth(size_t health);

	/**
	 * Sets a position.
	 *
	 * @param	pos	The position.
	 */
	void SetPosition(Vector2 pos);

	// All the state changing stuff happens in here. 
	virtual void Update(std::shared_ptr<Level> & level);

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition();

	/**
	 * Draws.
	 *
	 * @param [in,out]	level	The level.
	 *
	 * 	Draws this object.
	 */
	virtual void Draw(std::shared_ptr<Level> & level, Camera& camera);

protected:

	/** The health. */
	size_t _health;

	/** The speed. */
	Vector2 _speed;

	/** The position. */
	Vector2 _position;

	/** The aabb. */
	AABB _aabb;

	/** The sprite. */
	std::shared_ptr<SDL2pp::Texture> _sprite;

	/** The manager. */
	GameManager* _mgr;
};