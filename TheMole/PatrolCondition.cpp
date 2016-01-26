#include "PatrolCondition.h"
#include <stdio.h>


PatrolCondition::PatrolCondition()
{
}


PatrolCondition::~PatrolCondition()
{
}

/*
* Test contains the condition test that each condition will check for. Currently this is only a key check, checking for P
* 
* returns true if fulfilled and false if not.
*/
bool PatrolCondition::Test()
{
	//return Poll_Event();
	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	if (keys[SDL_SCANCODE_P])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PatrolCondition::Poll_Event()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			//Doesn't do anything at this point
		case SDL_KEYUP:
			return On_Key_Up(event.key.keysym);
		default:
			break;
		}
	}
}

bool PatrolCondition::On_Key_Up(SDL_Keysym keysym)
{
	switch (keysym.sym)
	{
	case SDLK_p:
		return true;
	default:
		return false;
	}
}