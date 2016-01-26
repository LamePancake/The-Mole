#include "AttackAction.h"



AttackAction::AttackAction()
{
}


AttackAction::~AttackAction()
{
}

/*
* Contain commands to be executed for this action. Currently just printing.
*
* returns true if successfully executed, false if something goes wrong. Currently just returning true.
*/
bool AttackAction::Execute()
{
	std::cout << "Attacking";
	return true;
}
