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
#include "Action.h"

class Agent {
public:
	/**
	* Creates a new Agent object with references to the SDL libraries, a window, and the map of screens to be used in the game.
	*
	*/
	Agent(SDL2pp::SDL& sdl, SDL2pp::SDLImage& sdlImage, SDL2pp::SDLMixer& sdlMixer, SDL2pp::SDLTTF& sdlTtf,
		SDL2pp::Window& window, SDL2pp::Renderer& renderer)
		:_sdl(sdl), _sdlImage(sdlImage), _sdlMixer(sdlMixer), _sdlTtf(sdlTtf),
		_window(window), _renderer(renderer) 
	{
		InitializeAllStates();
	}

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
	SDL2pp::SDL& GetSDL() const { return _sdl; }
	SDL2pp::SDLImage& GetSDLImage() const { return _sdlImage; }
	SDL2pp::SDLMixer& GetSDLMixer() const { return _sdlMixer; }
	SDL2pp::SDLTTF& GetSDLTTF() const { return _sdlTtf; }
	SDL2pp::Window& GetWindow() const { return _window; }
	SDL2pp::Renderer& GetRenderer() const { return _renderer; }

private:
	SDL2pp::SDL& _sdl;
	SDL2pp::SDLImage& _sdlImage;
	SDL2pp::SDLMixer& _sdlMixer;
	SDL2pp::SDLTTF& _sdlTtf;
	SDL2pp::Window& _window;
	SDL2pp::Renderer& _renderer;

	template <typename Action>
	std::vector<Action> _actionList;
	StateMachine _sm_FSM;
	State _s_Patrol;
	State _s_Attack;
	Transition _t_AttackPatrol;
	Transition _t_PatrolAttack;
	template <typename Action>
	std::vector<Action> _attackTransList;
	template <typename Action>
	std::vector<Action> _patrolTransList;
	AttackCondition _c_Attack;
	PatrolCondition _c_Patrol;
};