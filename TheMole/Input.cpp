#include "Input.h"
#include <stdio.h>

Input::Input()
{
}


Input::~Input()
{
}

void Input::Poll_Event()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			//Doesn't do anything at this point
		case SDL_KEYUP:
			On_Key_Up(event.key.keysym);
		default:
			break;
		}
	}
}

void Input::On_Key_Up(SDL_Keysym keysym)
{
	switch (keysym.sym)
	{
	case SDLK_a:
		printf("\nLeft");
		break;
	case SDLK_w:
		printf("\nUp");
		break;
	case SDLK_s:
		printf("\nDown");
		break;
	case SDLK_f:
		printf("\nRight");
		break;
	default:
		break;
	}
}


