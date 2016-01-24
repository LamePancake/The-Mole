#ifndef PATROLACTION_H
#define PATROLACTION_H
#pragma once
#include "Action.h"
#include <iostream>
class PatrolAction : public Action
{
public:
	PatrolAction();
	~PatrolAction();
	bool Execute();
};

#endif