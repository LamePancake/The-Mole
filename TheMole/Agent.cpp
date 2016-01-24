#include "Agent.h"

using std::string;
using std::shared_ptr;

/**
* Initialize state machine, all states, transitions, and conditions
*
* @param
*/
void Agent::InitializeAllStates()
{
	_attackTransList.push_back();
}


/**
* Update state machine, called once per frame
*
*
*/
void Agent::Update()
{
	_actionList = _sm_FSM.UpdateStateMachine();

	for (int i = 0; i < _actionList.size(); i++)
	{
		_actionList[i].Execute();
	}
}