#include "SimpleAgent.h"



SimpleAgent::SimpleAgent()
{
	_health = 100;
}


SimpleAgent::~SimpleAgent()
{
}

void SimpleAgent::Update()
{
	// While the AI is alive, do stuff.
	if (_health > 0)
	{
		const Uint8* keys = SDL_GetKeyboardState(nullptr);

		if (keys[SDL_SCANCODE_O])
		{
			std::cout << "Attacking";
		}
		if (keys[SDL_SCANCODE_P])
		{
			std::cout << "Patrolling";
		}
	}
	/*
	switch (keys) {
	case 0:
		_mgr->SetNextScreen("level1");
		return SCREEN_FINISH;
	case 1:
		_mgr->SetNextScreen("settings");
		return SCREEN_OVERLAY;
	case 2:
		// We just tried to quit
		// So for now we're going to do it the dirty way
		exit(0);
	}*/
}
