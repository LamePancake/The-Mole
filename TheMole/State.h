#ifndef STATE_H
#define STATE_H
#pragma once
#include <vector>
#include "Transition.h"
#include "Action.h"
class Transition;
/*
* State class is what is transitioned to and from by State Machine via Transitions.
*/
class State
{
public:
	State();
	~State();

	/*
	* Adds action to the state's collection of Actions
	*/
	void AddAction(Action a);

	/*
	* Returns _actions, the collectino of Actions
	*/
	std::vector<Action> GetAction();

	/*
	* Add Action to the collection of entry actions
	*/
	void AddEntryAction(Action a);

	/*
	* Retuns _entActions, collection of entry actions
	*/
	std::vector<Action> GetEntryAction();

	/*
	* Add Action to the collection of exit actions
	*/
	void AddExitAction(Action a);

	/*
	* Retuns _extActions, collection of exit actions
	*/
	std::vector<Action> GetExitAction();

	/*
	* Add Transition to collection of transitions
	*/
	void AddTransition(Transition t);

	/*
	* Retuns _transitions, collection of transitions
	*/
	std::vector<Transition> GetTransitions();

private:
	std::vector<Action> _actions;
	std::vector<Action> _entActions;
	std::vector<Action> _extActions;
	std::vector<Transition> _transitions;
};

#endif