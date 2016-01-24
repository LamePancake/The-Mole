#pragma once
#include "Action.h"
#include <iostream>
class AttackAction : public Action
{
public:
	AttackAction();
	~AttackAction();
	int Execute();
};

