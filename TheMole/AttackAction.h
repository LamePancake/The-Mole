#ifndef ATTACKACTION_H
#define ATTACKACTION_H
#pragma once
#include "Action.h"
#include <iostream>
/*
* Action for the attack state. Inherits Action base class.
*/
class AttackAction : public Action
{
public:
	AttackAction();
	~AttackAction();
	/*
	* Contains the commands to be executed in this action.
	*/
	bool Execute();
};
#endif
