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
	Transition(State &target, std::vector<Action> actList, Condition &c);
	bool IsTriggered();
	State* GetTargetState();
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