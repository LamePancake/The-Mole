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

/*
* Contain commands to be executed for this action. Currently just printing.
*
* returns true if successfully executed, false if something goes wrong. Currently just returning true.
*/
bool PrintAction::Execute()
{
	std::cout << _output;
	return true;
}
