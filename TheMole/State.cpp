#include "State.h"



State::State()
{
}


State::~State()
{
}

void State::AddAction(Action a)
{
	_actions.push_back(a);
}

std::vector<Action> State::GetAction()
{
	return _actions;
}

void State::AddEntryAction(Action a)
{
	_entActions.push_back(a);
}

std::vector<Action> State::GetEntryAction()
{
	return _entActions;
}

void State::AddExitAction(Action a)
{
	_extActions.push_back(a);
}

std::vector<Action> State::GetExitAction()
{
	return _extActions;
}

void State::AddTransition(Transition t)
{
	_transitions.push_back(t);
}

std::vector<Transition> State::GetTransitions()
{
	return _transitions;
}