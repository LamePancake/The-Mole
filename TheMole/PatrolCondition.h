#ifndef PATROLCONDITION_H
#define PATROLCONDITION_H
#pragma once
#include "Condition.h"
#include <SDL2/SDL.h>
class PatrolCondition : public Condition
{
public:
	PatrolCondition();
	~PatrolCondition();
	/*
	* Test contains the condition test that each condition will check for.
	*/
	bool Test();
	bool Poll_Event();
private:
	/*
	* The below are temporary, and are there as seen in the Input class.
	*/
	bool On_Key_Up(SDL_Keysym);
	int buttonsPressed;
	SDL_Event event;
};

#endif