#ifndef TRANSITION_H
#define TRANSITION_H
#pragma once
#include "Action.h"
#include "Condition.h"
#include "State.h"
#include <vector>
class Transition
{
public:
	template <typename Action, typename Condition>
	Transition(State target, std::vector<Action> actList, Condition c);
	~Transition();
	template <typename Condition>
	bool IsTriggered();
	State GetTargetState();
	template <typename Action>
	std::vector<Action> GetAction();

private:
	State _targetState;
	template <typename Action>
	std::vector<Action> _actions;
	template <typename Condition>
	Condition _condi;
	int _minVal;
	int _maxVal;
	int _curVal;
	bool _pressed;
	float _curTime;
	float _maxTime;
};
#endif