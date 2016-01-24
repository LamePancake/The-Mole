#ifndef PATROLCONDITION_H
#define PATROLCONDITION_H
#pragma once
#include "Condition.h"
class PatrolCondition : public Condition
{
public:
	PatrolCondition();
	~PatrolCondition();
	bool Test();
};

#endif