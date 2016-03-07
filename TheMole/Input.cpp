#include "Input.h"
#include <stdio.h>
#include <memory>

using namespace std;

Input::Input()
	: _prevKeyboardState((Uint8*)std::calloc(sizeof(Uint8) * SDL_NUM_SCANCODES, 1)), _curKeyboardState((Uint8*)std::calloc(sizeof(Uint8) * SDL_NUM_SCANCODES, 1))
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

void Input::ClearKeyboardState()
{
	free(_prevKeyboardState);
	free(_curKeyboardState);
	_prevKeyboardState = (Uint8*)std::calloc(sizeof(Uint8) * SDL_NUM_SCANCODES, 1);
	_curKeyboardState = (Uint8*)std::calloc(sizeof(Uint8) * SDL_NUM_SCANCODES, 1);
}

bool Input::ActionOccurred(string actionName, ActionType actionType) {
	switch (actionType) {
	case Pressed:
		return KeyPressed(_actionMap[actionName]->actionKey);
		break;
	case Released:
		return KeyReleased(_actionMap[actionName]->actionKey);
		break;
	case Held:
		return KeyHeld(_actionMap[actionName]->actionKey);
		break;
	case StillUp:
		return KeyStillUp(_actionMap[actionName]->actionKey);
		break;
	case Down:
		return KeyDown(_actionMap[actionName]->actionKey);
		break;
	case Up:
		return KeyUp(_actionMap[actionName]->actionKey);
		break;
	}
	return false;
}

void Input::PollEvent()
{
	//while (SDL_PollEvent(&_generalEvent))
	//{
	//	switch (_generalEvent.type)
	//	{
	//	case SDL_KEYDOWN:
	//		//Doesn't do anything at this point
	//	case SDL_KEYUP:
	//		On_Key_Up(_generalEvent.key.keysym);
	//	default:
	//		break;
	//	}
	//}
}

//The parameter value should be an enumerated value that matches
//SDL_Scancode values
bool Input::KeyPressed(SDL_Scancode key)
{
	if (key == NULL)
		return false;
	if (!_prevKeyboardState[key] && _curKeyboardState[key])
		return true;
	else
		return false;
}

//The parameter value should be an enumerated value that matches
//SDL_Scancode values
bool Input::KeyReleased(SDL_Scancode key)
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

bool Input::KeyStillUp(SDL_Scancode key)
{
	if (key == NULL)
		return false;
	if (!_prevKeyboardState[key] && !_curKeyboardState[key])
		return true;
	else
		return false;
}

bool Input::KeyUp(SDL_Scancode key)
{
	if (key == NULL)
		return false;
	if (!_curKeyboardState[key])
		return true;
	else
		return false;
}

bool Input::KeyDown(SDL_Scancode key)
{
	if (key == NULL)
		return false;
	if (_curKeyboardState[key])
		return true;
	else
		return false;
}

void Input::AddKeyToMap(string key, InputAction &inputAct)
{	
	_actionMap.insert({ {key, std::shared_ptr<InputAction>(new InputAction(inputAct))} });
}
