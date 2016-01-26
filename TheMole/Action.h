#ifndef ACTION_H
#define ACTION_H

/*
* Action class is the base class for all action objects. Execute needs to be overwritten by derived classes.
*/
#pragma once
class Action
{
public:
	Action();
	~Action();
	/*
	* This method would contain commands to be executed upon action's state becoming the current state.
	*/
	bool Execute();
};

#endif