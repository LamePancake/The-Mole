#include "Input.h"
#include <stdio.h>
#include <memory>

using namespace std;

Input::Input()
	: _prevKeyboardState((Uint8*)std::malloc(sizeof(Uint8) * SDL_NUM_SCANCODES)), _curKeyboardState((Uint8*)std::malloc(sizeof(Uint8) * SDL_NUM_SCANCODES))
{}

Input::~Input()
{
	free(_prevKeyboardState);
	free(_curKeyboardState);
}


void Input::UpdateKeyboardState() {
	memcpy(_prevKeyboardState, _curKeyboardState, sizeof(Uint8) * SDL_NUM_SCANCODES);
	memcpy(_curKeyboardState, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

bool Input::ActionOccured(string actionName, ActionType actionType) {
	switch (actionType) {
	case Pressed:
		return KeyPressed(_actionMap[actionName]->actionKey);
		break;
	case Held:
		return KeyHeld(_actionMap[actionName]->actionKey);
		break;
	}
	return false;
}

void Input::PollEvent()
{
	while (SDL_PollEvent(&_generalEvent))
	{
		switch (_generalEvent.type)
		{
		case SDL_KEYDOWN:
			//Doesn't do anything at this point
		case SDL_KEYUP:
			On_Key_Up(_generalEvent.key.keysym);
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
	if (_prevKeyboardState[key] && !_curKeyboardState[key])
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
	if (_prevKeyboardState[key] && _curKeyboardState[key])
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

void Input::AddKeyToMap(string key, InputAction &inputAct)
{	
	_actionMap.insert({ {key, make_shared<InputAction>(&inputAct)} });
}
