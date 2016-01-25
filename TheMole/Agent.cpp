#include "Agent.h"

using std::string;
using std::shared_ptr;

Agent::Agent()
{
	InitializeAllStates();
}

/**
* Initialize state machine, all states, transitions, and conditions
*
* @param
*/
void Agent::InitializeAllStates()
{
	PrintAction startAtt = PrintAction("start attacking");
	_attackTransList.push_back(startAtt);
	PrintAction patrolAtt = PrintAction("start patrolling");
	_patrolTransList.push_back(patrolAtt);

	_t_AttackPatrol = Transition(_s_Patrol, _patrolTransList, _c_Patrol);
	_t_PatrolAttack = Transition(_s_Attack, _attackTransList, _c_Attack);

	_s_Attack.AddAction(AttackAction());
	_s_Attack.AddEntryAction(PrintAction("begin attack"));
	_s_Attack.AddExitAction(PrintAction("end attack"));
	_s_Attack.AddTransition(_t_AttackPatrol);

	_s_Patrol.AddAction(PatrolAction());
	_s_Patrol.AddEntryAction(PrintAction("begin patrol"));
	_s_Patrol.AddExitAction(PrintAction("end patrol"));
	_s_Patrol.AddTransition(_t_PatrolAttack);

	_sm_FSM = StateMachine(_s_Patrol);
	_sm_FSM.AddState(_s_Attack);
}


/**
* Update state machine, called once per frame
*
*
*/
void Agent::Update()
{
	_actionList = _sm_FSM.UpdateStateMachine();

	for (size_t i = 0; i < _actionList.size(); i++)
	{
		_actionList[i].Execute();
	}
}