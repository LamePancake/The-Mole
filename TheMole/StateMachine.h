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

	void AddState(State s);

	template <typename Action>
	std::vector<Action> UpdateStateMachine();

private:
	std::vector<State> _statesList;
	State _initialState;
	State _currentState;
	State _targetState;
	Transition triggeredTransition;
	template<typename Action>
	std::vector<Action> _actions;
};

