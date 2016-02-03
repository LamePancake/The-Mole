#ifndef SIMPLEAGENT_H
#define SIMPLEAGENT_H
class AABB;
#include <SDL2pp\SDL2pp.hh>
#include <iostream>
#include "Vector2.h"
#include "GameManager.h"
#include "AABB.h"
#include "Level.h"

#pragma once

/**
* This is the simple version of the state machine. It uses a switch instead of states, transitions, conditions, actions.
**/
class SimpleAgent
{
public:
	SimpleAgent();
	~SimpleAgent();
	/**
	* Constructor that initializes _position to the input parameter
	*
	* @param the starting position of the agent
	* @param the game manager
	* @param starting speed
	*/
	SimpleAgent(Vector2 position, GameManager & manager, Vector2 spd);

	/**
	* Clear up memory used for loading the texture
	**/
	void Unload();

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

	/**
	* Set the speed that the position gets updated by
	**/
	void SetSpeed(Vector2 spd);

	/**
	* Check collision against another SimpleAgent
	*
	* @return true if collided, false if not
	**/
	bool CollisionCheck(SimpleAgent & otherAI);

	/**
	* Return AABB of the agent
	*
	* @return _aabb
	**/
	AABB GetAABB();

	/**
	* Scans tiles left and right of the agent and negate _speed if left or right neighbour is not blank tile
	*/
	void ScanNeighbouringTiles(std::shared_ptr<Level> & level);
private:
	size_t _health;
	Vector2 _speed;
	Vector2 _position;
	AABB _aabb;
	SDL2pp::Texture* _sprite;
	GameManager* _mgr;
};
#endif
