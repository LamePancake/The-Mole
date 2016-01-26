#ifndef CONDITION_H
#define CONDITION_H
#pragma once
#include <SDL2/SDL.h>
/*
* Condition class is the base class for all condition objects. Test needs to be overwritten by derived classes.
*/
class Condition
{
public:
	Condition();
	~Condition();
	/*
	* Test contains the condition test that each condition will check for.
	*/
	bool Test();
	void Poll_Event();
private:
	/*
	* The below are temporary, and are there as seen in the Input class.
	*/
	void On_Key_Up(SDL_Keysym);
	int buttonsPressed;
	SDL_Event event;
};

#endif