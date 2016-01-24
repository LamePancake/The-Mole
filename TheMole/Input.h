#pragma once
#include <SDL2/SDL.h>
class Input
{
public:
	Input();
	~Input();
	void Poll_Event();
private:
	void On_Key_Up(SDL_Keysym);
	int buttonsPressed;
	SDL_Event event;
};