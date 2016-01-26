#include "Transition.h"
#include "State.h"
#include "Condition.h"

Transition::Transition()
{

}

Transition::~Transition()
{
}

/*
* Initializes with target state, actions for this transition, and condition of target list
*/
Transition::Transition(State &target, std::vector<Action> actList, Condition &c)
{
	_target = &target;
	_actions = actList;
	_condi = &c;
}

/*
* Returns true if condition is triggered, false if not.
*/
bool Transition::IsTriggered()
{
	return _condi->Test();
}

/*
* Returns _target, the target state
*/
State* Transition::GetTargetState()
{
	return _target;
}

/*
* Returns _actions, the actions of this transition
*/
std::vector<Action> Transition::GetAction()
{
	return _actions;
}