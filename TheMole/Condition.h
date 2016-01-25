#ifndef CONDITION_H
#define CONDITION_H
#pragma once
#include <SDL2/SDL.h>
class Condition
{
public:
	Condition();
	~Condition();
	bool Test();
	void Poll_Event();
private:
	void On_Key_Up(SDL_Keysym);
	int buttonsPressed;
	SDL_Event event;
};

#endif