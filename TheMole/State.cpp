#include "State.h"



State::State()
{
}


State::~State()
{
}

/*
* Adds action to the state's collection of Actions
*/
void State::AddAction(Action a)
{
	_actions.push_back(a);
}

/*
* Returns _actions, the collectino of Actions
*/
std::vector<Action> State::GetAction()
{
	return _actions;
}

/*
* Add Action to the collection of entry actions
*/
void State::AddEntryAction(Action a)
{
	_entActions.push_back(a);
}

/*
* Retuns _entActions, collection of entry actions
*/
std::vector<Action> State::GetEntryAction()
{
	return _entActions;
}

/*
* Add Action to the collection of exit actions
*/
void State::AddExitAction(Action a)
{
	_extActions.push_back(a);
}

/*
* Retuns _extActions, collection of exit actions
*/
std::vector<Action> State::GetExitAction()
{
	return _extActions;
}

/*
* Add Transition to collection of transitions
*/
void State::AddTransition(Transition t)
{
	_transitions.push_back(t);
}

/*
* Retuns _transitions, collection of transitions
*/
std::vector<Transition> State::GetTransitions()
{
	return _transitions;
}