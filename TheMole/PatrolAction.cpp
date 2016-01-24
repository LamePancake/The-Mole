#include "PatrolAction.h"



PatrolAction::PatrolAction()
{
}


PatrolAction::~PatrolAction()
{
}

bool PatrolAction::Execute()
{
	std::cout << "Patrolling";
	return true;
}
