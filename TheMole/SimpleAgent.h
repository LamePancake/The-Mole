#ifndef SIMPLEAGENT_H
#define SIMPLEAGENT_H
#include <SDL2pp\SDL2pp.hh>
#include <iostream>
#pragma once
/**
* This is the simple version of the state machine. It uses a switch instead of states, transitions, conditions, actions.
**/
class SimpleAgent
{
public:
	SimpleAgent();
	~SimpleAgent();
	// All the state changing stuff happens in here.
	void Update();
private:
	size_t _health;
	float _speed;
};
#endif
