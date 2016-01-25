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
	bool Test();
	bool Poll_Event();
private:
	bool On_Key_Up(SDL_Keysym);
	int buttonsPressed;
	SDL_Event event;
};

#endif