#include "StateMachine.h"



StateMachine::StateMachine()
{
}


StateMachine::~StateMachine()
{
}

StateMachine::StateMachine(State init)
{
	_initialState = init;
	_currentState = _initialState;
}

void StateMachine::AddState(State s)
{
	_statesList.push_back(s);
}

std::vector<Action> StateMachine::UpdateStateMachine()
{
	_triggeredTransition;

	// Check through each transition and store the first one that triggers
	for (int i = 0; i < _currentState.GetTransitions().size(); i++)
	{
		if (_currentState.GetTransitions()[i].IsTriggered())
		{
			_triggeredTransition = _currentState.GetTransitions()[i];
			break;
		}
	}

	// Check if we have a transition to fire
	if (&_triggeredTransition != NULL)
	{
		// Find the target state
		_targetState = _triggeredTransition.GetTargetState();
		// Add the exit action of the old state, the transition action and the entry for the new state
		for (int i = 0; i < _triggeredTransition.GetAction().size(); i++)
		{
			_actions.push_back(_triggeredTransition.GetAction()[i]);
		}
		// Complete the transition and return action list
		_currentState = _targetState;
		return _actions;
	}
	// Otherwise just return the current state's actions
	else
	{
		return _currentState.GetAction();
	}
}
