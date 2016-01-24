#pragma once
#include "Action.h"
#include <iostream>
class PatrolAction : public Action
{
public:
	PatrolAction();
	~PatrolAction();
	int Execute();
};

