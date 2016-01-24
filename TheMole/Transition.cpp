#include "Transition.h"


template <typename Action, typename Condition>
Transition::Transition(State target, std::vector<Action> actList, Condition c)
{
	_targetState = target;
	_actions = actList;
	_condi = c;
}


Transition::~Transition()
{
}

template <typename Condition>
bool Transition::IsTriggered()
{
	return _condi.Test();
}

State Transition::GetTargetState()
{
	return _targetState;
}

template <typename Action>
std::vector<Action> GetAction()
{
	return _actions;
}