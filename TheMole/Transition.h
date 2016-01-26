#ifndef TRANSITION_H
#define TRANSITION_H
#pragma once
#include <vector>

class Action;
class Condition;
class State;

class Transition
{
public:
	Transition();
	~Transition();
	/*
	* Initializes with target state, actions for this transition, and condition of target list
	*/
	Transition(State &target, std::vector<Action> actList, Condition &c);

	/*
	* Returns true if condition is triggered, false if not.
	*/
	bool IsTriggered();

	/*
	* Returns _target, the target state
	*/
	State* GetTargetState();

	/*
	* Returns _actions, the actions of this transition
	*/
	std::vector<Action> GetAction();

private:
	State* _target;
	std::vector<Action> _actions;
	Condition* _condi;
	int _minVal;
	int _maxVal;
	int _curVal;
	bool _pressed;
	float _curTime;
	float _maxTime;
};
#endif