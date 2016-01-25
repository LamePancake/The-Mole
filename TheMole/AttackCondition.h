#ifndef ATTACKCONDITION_H
#define ATTACKCONDITION_H
#pragma once
#include "Condition.h"
#include <SDL2/SDL.h>
class AttackCondition : public Condition
{ 
public:
	AttackCondition();
	~AttackCondition();
	bool Test();
	bool Poll_Event();
private:
	bool On_Key_Up(SDL_Keysym);
	int buttonsPressed;
	SDL_Event event;
};

#endif