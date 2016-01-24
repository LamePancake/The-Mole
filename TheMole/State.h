#ifndef STATE_H
#define STATE_H
#pragma once
#include <vector>
#include "Transition.h"
#include "Action.h"

class State
{
public:
	State();
	~State();

	void AddAction(Action a);

	std::vector<Action> GetAction();

	void AddEntryAction(Action a);

	std::vector<Action> GetEntryAction();

	void AddExitAction(Action a);

	std::vector<Action> GetExitAction();

	void AddTransition(Transition t);

	std::vector<Transition> GetTransitions();

private:
	std::vector<Action> _actions;
	std::vector<Action> _entActions;
	std::vector<Action> _extActions;
	std::vector<Transition> _transitions;
};

#endif