#ifndef PRINTACTION_H
#define PRINTACTION_H
#pragma once
#include "Action.h"
#include <string>
#include <iostream>
class PrintAction : public Action
{
public:
	PrintAction();
	~PrintAction();
	PrintAction(std::string output);
	bool Execute();
private:
	std::string _output;
};
#endif