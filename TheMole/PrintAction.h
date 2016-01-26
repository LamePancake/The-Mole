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
	/*
	* Contains the commands to be executed in this action.
	*/
	bool Execute();
private:
	std::string _output;
};
#endif