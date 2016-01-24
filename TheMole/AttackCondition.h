#ifndef ATTACKCONDITION_H
#define ATTACKCONDITION_H
#pragma once
#include "Condition.h"
class AttackCondition : public Condition
{ 
public:
	AttackCondition();
	~AttackCondition();
	bool Test();
};

#endif