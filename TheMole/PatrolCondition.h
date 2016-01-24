#pragma once
#include "Condition.h"
class PatrolCondition : public Condition
{
public:
	PatrolCondition();
	~PatrolCondition();
	int Test();
};

