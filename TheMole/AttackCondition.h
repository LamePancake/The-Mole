#pragma once
#include "Condition.h"
class AttackCondition : public Condition
{ 
public:
	AttackCondition();
	~AttackCondition();
	int Test();
};

