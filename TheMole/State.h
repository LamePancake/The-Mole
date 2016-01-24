#pragma once
#include <vector>
#include "Action.h"
#include "Transition.h"
class State
{
public:
	State();
	~State();

	template<typename Action>
	void AddAction(Action a);

	template<typename Action>
	std::vector<Action> GetAction();

	template<typename Action>
	void AddEntryAction(Action a);

	template<typename Action>
	std::vector<Action> GetEntryAction();

	template<typename Action>
	void AddExitAction(Action a);

	template<typename Action>
	std::vector<Action> GetExitAction();

	void AddTransition(Transition t);

	std::vector<Transition> GetTransitions();

private:
	template<typename Action>
	std::vector<Action> _actions;
	template<typename Action>
	std::vector<Action> _entActions;
	template<typename Action>
	std::vector<Action> _extActions;
	std::vector<Transition> _transitions;
};

