#ifndef SIMPLEAGENT_H
#define SIMPLEAGENT_H
#include <SDL2pp\SDL2pp.hh>
#include <iostream>
#include "Vector2.h"
#pragma once

/**
* This is the simple version of the state machine. It uses a switch instead of states, transitions, conditions, actions.
**/
class SimpleAgent
{
public:
	SimpleAgent();
	~SimpleAgent();
	SimpleAgent(Vector2 position);
	// All the state changing stuff happens in here.
	void Update();

	// Sets position of the agent.
	void SetPosition(Vector2 &newPos);

	// Returns position of the agent.
	Vector2 GetPosition();
private:
	size_t _health;
	float _speed;
	Vector2 _position;
};
#endif
