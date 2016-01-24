#include "Transition.h"


Transition::Transition()
{

}

Transition::~Transition()
{
}

Transition::Transition(State target, std::vector<Action> actList, Condition c)
{
	_targetState = target;
	_actions = actList;
	_condi = c;
}

bool Transition::IsTriggered()
{
	return _condi.Test();
}

State Transition::GetTargetState()
{
	return _targetState;
}

std::vector<Action> Transition::GetAction()
{
	return _actions;
}