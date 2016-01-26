#ifndef AGENT_H
#define AGENT_H
#pragma once
#include <stack>
#include <string>
#include <unordered_map>
#include <memory>
#include <SDL2pp\SDL2pp.hh>
#include <vector>
#include "State.h"
#include "StateMachine.h"
#include "Transition.h"
#include "AttackCondition.h"
#include "PatrolCondition.h"
#include "PrintAction.h"
#include "AttackAction.h"
#include "PatrolAction.h"
#include "Action.h"

/*
* This is the class that is the AI object. 
*/
class Agent {
public:

	Agent();
	// THE FOLLOWING IS COMMENTED OUT BECAUSE I'M NOT SURE IF THE AI WILL NEED THOSE THINGS LATER
	/**
	* Creates a new Agent object with references to the SDL libraries, a window, and the map of screens to be used in the game.
	*
	*/
	/*Agent(SDL2pp::SDL& sdl, SDL2pp::SDLImage& sdlImage, SDL2pp::SDLMixer& sdlMixer, SDL2pp::SDLTTF& sdlTtf,
		SDL2pp::Window& window, SDL2pp::Renderer& renderer)
		:_sdl(sdl), _sdlImage(sdlImage), _sdlMixer(sdlMixer), _sdlTtf(sdlTtf),
		_window(window), _renderer(renderer) 
	{
		InitializeAllStates();
	}*/

	// Don't want the GameManager object to be copied or moved
	Agent operator=(Agent&) = delete;
	Agent(Agent&) = delete;

	/**
	* Initialize state machine, all states, transitions, and conditions
	*
	* @param 
	*/
	void InitializeAllStates();

	/**
	* Update state machine, called once per frame
	*
	*
	*/
	void Update();

	// SDL accessor methods
	/*SDL2pp::SDL& GetSDL() const { return _sdl; }
	SDL2pp::SDLImage& GetSDLImage() const { return _sdlImage; }
	SDL2pp::SDLMixer& GetSDLMixer() const { return _sdlMixer; }
	SDL2pp::SDLTTF& GetSDLTTF() const { return _sdlTtf; }
	SDL2pp::Window& GetWindow() const { return _window; }
	SDL2pp::Renderer& GetRenderer() const { return _renderer; }*/

private:
	/*SDL2pp::SDL& _sdl;
	SDL2pp::SDLImage& _sdlImage;
	SDL2pp::SDLMixer& _sdlMixer;
	SDL2pp::SDLTTF& _sdlTtf;
	SDL2pp::Window& _window;
	SDL2pp::Renderer& _renderer;*/

	/*
	* This is the collection of actions to be executed by the AI
	*/
	std::vector<Action> _actionList;

	/*
	* This is the Finite State Machine that will execute all the necessary calls
	*/
	StateMachine _sm_FSM;

	/*
	* The patrol state.
	*/
	State _s_Patrol;

	/*
	* The attack state.
	*/
	State _s_Attack;

	/*
	* The transition from attack to patrol. Naming convention is [current][target]
	*/
	Transition _t_AttackPatrol;

	/*
	* The transition from patrol to attack. Naming convention is [current][target]
	*/
	Transition _t_PatrolAttack;

	/*
	* The collection of attack state's transitions.
	*/ 
	std::vector<Action> _attackTransList;

	/*
	* The collection of patrol state's transitions.
	*/
	std::vector<Action> _patrolTransList;

	/*
	* Attack state's trigger condition.
	*/
	AttackCondition _c_Attack;

	/*
	* Patrol state's trigger condition.
	*/
	PatrolCondition _c_Patrol;
};

#endif