#include "Input.h"
#include <stdio.h>

Input::Input()
{
	curKeyboardState = SDL_GetKeyboardState(NULL);
	prevKeyboardState = curKeyboardState;
}


Input::~Input()
{
}

void Input::UpdateKeyboardState() {
	prevKeyboardState = curKeyboardState;
	curKeyboardState = SDL_GetKeyboardState(NULL);
}

void Input::PollEvent()
{
	while (SDL_PollEvent(&generalEvent))
	{
		switch (generalEvent.type)
		{
		case SDL_KEYDOWN:
			//Doesn't do anything at this point
		case SDL_KEYUP:
			On_Key_Up(generalEvent.key.keysym);
		default:
			break;
		}
	}
}

//The parameter value should be an enumerated value that matches
//SDL_Scancode values
bool Input::KeyPressed(SDL_Scancode key)
{
	if (key == NULL)
		return false;
	if (prevKeyboardState[key] && !curKeyboardState[key])
		return true;
	else
		return false;
}

//The parameter value should be an enumerated value that matches
//SDL_Scancode values
bool Input::KeyHeld(SDL_Scancode key)
{
	if (key == NULL)
		return false;
	if (prevKeyboardState[key] && curKeyboardState[key])
		return true;
	else
		return false;
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
