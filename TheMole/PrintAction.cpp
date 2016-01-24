#include "PrintAction.h"



PrintAction::PrintAction()
{
}


PrintAction::~PrintAction()
{
}

PrintAction::PrintAction(std::string output)
{
	_output = output;
}

bool PrintAction::Execute()
{
	std::cout << _output;
	return true;
}
