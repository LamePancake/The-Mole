#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#pragma once
#include "State.h"
#include <vector>
#include "Action.h"
#include "Transition.h"
class StateMachine
{
public:
	StateMachine();
	~StateMachine();
	StateMachine(State init);

	void AddState(State s);

	std::vector<Action> UpdateStateMachine();

private:
	std::vector<State> _statesList;
	State _initialState;
	State _currentState;
	State _targetState;
	Transition _triggeredTransition;
	std::vector<Action> _actions;
};
#endif
