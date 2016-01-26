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
	/*
	* Contains the commands to be executed in this action.
	*/
	bool Execute();
};

#endif