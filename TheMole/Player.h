#pragma once
#include <SDL2pp\SDL2pp.hh>
#include "Vector2.h"
#include "AABB.h"
#include "GameManager.h"
#include "Level.h"

class Player
{
public:
	Player() {}

	~Player();

	/** Draws this object. */
	void Draw(std::shared_ptr<Level> & level);

	/**
	* Return AABB of the agent
	*
	* @return _aabb
	**/
	AABB GetAABB();

	/**
	* Constructor that initializes _position to the input parameter
	*
	* @param the starting position of the agent
	* @param the game manager
	* @param starting speed
	*/
	Player(Vector2 position, GameManager & manager, Vector2 spd);

	/**
	* Returns in the textures needed to render the AI.
	*/
	SDL2pp::Texture* GetTexture();

	// All the state changing stuff happens in here.
	void Update(std::shared_ptr<Level> & level);

	// Updates position of the agent by adding _speed to it.
	void UpdatePosition();

	// Returns position of the agent.
	Vector2 GetPosition();

private:
	
	/** The health. */
	size_t _health;

	/** The speed. */
	Vector2 _speed;

	/** The position. */
	Vector2 _position;

	/** The aabb. */
	AABB _aabb;

	/** The sprite. */
	SDL2pp::Texture* _sprite;

	/** The manager. */
	GameManager* _mgr;
};