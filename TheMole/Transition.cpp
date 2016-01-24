#include "Transition.h"
#include "State.h"
#include "Condition.h"

Transition::Transition()
{

}

Transition::~Transition()
{
}

Transition::Transition(State &target, std::vector<Action> actList, Condition &c)
{
	_target = &target;
	_actions = actList;
	_condi = &c;
}

bool Transition::IsTriggered()
{
	return _condi->Test();
}

State* Transition::GetTargetState()
{
	return _target;
}

std::vector<Action> Transition::GetAction()
{
	return _actions;
}