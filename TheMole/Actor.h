#pragma once
#include <SDL2pp\SDL2pp.hh>
#include <string>
#include <memory>
#include "Vector2.h"
#include "AABB.h"
#include "GameManager.h"
#include "Level.h"
#include "Camera.h"
#include "SpriteSheet.h"

class Actor
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
	Actor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath, std::string shadowTexturePath);

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
	std::shared_ptr<SpriteSheet> GetTexture();

	/**
	* Returns in the textures needed to render the AI shadow.
	*
	* @return	null if it fails, else the texture.
	*/
	std::shared_ptr<SpriteSheet> GetTextureShadow();

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
	virtual void Update(double elapsedSecs, std::shared_ptr<Level> & level);

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition();

	/**
	 * Draws.
	 *
	 * @param [in,out]	camera	ref to the camera.
	 * 
	 * Draws this object.
	 */
	virtual void Draw(Camera& camera);

protected:

	/** The health. */
	size_t _health;

	/** The speed. */
	Vector2 _speed;

	/** The position. */
	Vector2 _position;

	/** The aabb. */
	AABB _aabb;

	/** The sprite shadow. */
	std::shared_ptr<SpriteSheet> _spriteShadow;
	
	/** The sprite. */
	std::shared_ptr<SpriteSheet> _sprite;

	/** The manager. */
	GameManager* _mgr;
};