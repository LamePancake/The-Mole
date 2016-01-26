#include "AttackCondition.h"
#include <stdio.h>


AttackCondition::AttackCondition()
{
}


AttackCondition::~AttackCondition()
{
}

/*
* Test contains the condition test that each condition will check for. Currently this is only a key check, checking for O
*
* returns true if fulfilled and false if not.
*/
bool AttackCondition::Test()
{
	//return Poll_Event();
	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	if (keys[SDL_SCANCODE_O])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AttackCondition::Poll_Event()
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

bool AttackCondition::On_Key_Up(SDL_Keysym keysym)
{
	switch (keysym.sym)
	{
	case SDLK_o:
		return true;
	default:
		return false;
	}
}