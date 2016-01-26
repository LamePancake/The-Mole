#include "PatrolAction.h"



PatrolAction::PatrolAction()
{
}


PatrolAction::~PatrolAction()
{
}

/*
* Contain commands to be executed for this action. Currently just printing.
*
* returns true if successfully executed, false if something goes wrong. Currently just returning true.
*/
bool PatrolAction::Execute()
{
	std::cout << "Patrolling";
	return true;
}
