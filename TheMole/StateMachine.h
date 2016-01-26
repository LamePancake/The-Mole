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
	/*
	* init is the default state that you begin in and is first in _stateList
	*/
	StateMachine(State &init);

	/*
	* Add State s to _stateList
	*/
	void AddState(State s);

	/*
	* Updates by checking all transitions of current state for triggered conditions. If one is triggered, it's fired and then transition to new state takes place.
	*
	* returns vector of Actions for the current state.
	*/
	std::vector<Action> UpdateStateMachine();

private:
	std::vector<State> _statesList;
	State* _initialState;
	State* _currentState;
	State* _targetState;
	Transition _triggeredTransition;
	std::vector<Action> _actions;
};
#endif
