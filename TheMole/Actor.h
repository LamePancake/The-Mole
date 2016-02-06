#ifndef ACTOR_H
#define ACTOR_H

#include <SDL2pp\SDL2pp.hh>
#include <string>
#include <memory>
#include "Vector2.h"
#include "AABB.h"
#include "GameManager.h"
#include "Level.h"
#include "Camera.h"
#include "SpriteSheet.h"

class GameScreen;

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
	 * @param	framesPerSecond  	The frames per second.
	 */
	Actor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath, int framesPerSecond);

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

	/**
	 * Gets actor direction.
	 *
	 * @return	The actor direction.
	 */
	SpriteSheet::XAxisDirection GetActorDirection();

	/**
	 * Sets actor direction.
	 *
	 * @param	dir	The direction.
	 */
	void SetActorDirection(SpriteSheet::XAxisDirection dir);

	// All the state changing stuff happens in here. 
	virtual void Update(double elapsedSecs);

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition(double elapsedSecs);

	/**
	 * Draws.
	 *
	 * @param [in,out]	camera	ref to the camera.
	 * 
	 * Draws this object.
	 */
	virtual void Draw(Camera& camera);

protected:

	typedef enum {
		TOP,
		BOTTOM,
		RIGHT,
		LEFT,
		NONE
	} Edge;

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


	/** @brief	A pointer to the current game screen. */
	std::shared_ptr<GameScreen> _gameScreen;

	SpriteSheet::XAxisDirection _actorDir;

	/**
	 * @brief	Gets tile collision information.
	 *
	 * @author	Shane
	 * @date	2/5/2016
	 *
	 * @param [in,out]	rowEdge		  	The row edge.
	 * @param [in,out]	colEdge		  	The col edge.
	 * @param [in,out]	rowPenetration	The row penetration.
	 * @param [in,out]	colPenetration	The col penetration.
	 * @param [in,out]	rowIntersect  	The row intersect.
	 * @param [in,out]	colIntersect  	The col intersect.
	 * @param [in,out]	level		  	The level.
	 */
	void GetTileCollisionInfo(Edge & rowEdge, Edge & colEdge, int & rowPenetration, int & colPenetration,
		std::vector<std::shared_ptr<Tile>>& rowIntersect, std::vector<std::shared_ptr<Tile>>& colIntersect, std::shared_ptr<Level>& level);
};

#endif