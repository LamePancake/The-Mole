#ifndef SIMPLEAGENT_H
#define SIMPLEAGENT_H
class AABB;
#include <SDL2pp\SDL2pp.hh>
#include <iostream>
#include "Vector2.h"
#include "GameManager.h"
#include "AABB.h"

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
	*/
	SimpleAgent(Vector2 position, GameManager & manager);

	/**
	* Clear up memory used for loading the texture
	**/
	void Unload();

	/**
	* Returns in the textures needed to render the AI.
	*/
	SDL2pp::Texture* GetTexture();

	// All the state changing stuff happens in here.
	void Update();

	// Sets position of the agent.
	void SetPosition(Vector2 &newPos);

	// Returns position of the agent.
	Vector2 GetPosition();

	/**
	* Check collision against another SimpleAgent
	*
	* @return true if collided, false if not
	**/
	bool CollisionCheck(SimpleAgent & otherAI);

	/**
	* Return AABB of the agent
	*
	* @retun _aabb
	**/
	AABB GetAABB();
private:
	size_t _health;
	float _speed;
	Vector2 _position;
	AABB _aabb;
	SDL2pp::Texture* _sprite;
	GameManager* _mgr;
};
#endif
