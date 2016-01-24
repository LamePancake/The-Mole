#include "AttackAction.h"



AttackAction::AttackAction()
{
}


AttackAction::~AttackAction()
{
}

bool AttackAction::Execute()
{
	std::cout << "Attacking";
	return true;
}
