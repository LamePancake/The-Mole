#ifndef ATTACKACTION_H
#define ATTACKACTION_H
#pragma once
#include "Action.h"
#include <iostream>
class AttackAction : public Action
{
public:
	AttackAction();
	~AttackAction();
	bool Execute();
};
#endif
