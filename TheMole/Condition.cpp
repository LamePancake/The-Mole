#include "Condition.h"



Condition::Condition()
{
}


Condition::~Condition()
{
}

/*
* Test contains the condition test that each condition will check for. Currently this is only a key check, checking for O
*/
bool Condition::Test()
{
	return true;
}

void Condition::Poll_Event()
{
}

void Condition::On_Key_Up(SDL_Keysym keysym)
{
}