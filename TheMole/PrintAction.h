#pragma once
#include "Action.h"
#include <iostream>
class PrintAction : public Action
{
public:
	PrintAction();
	~PrintAction();
	int Execute();
};
