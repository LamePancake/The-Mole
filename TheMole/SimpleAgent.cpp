#include "SimpleAgent.h"



SimpleAgent::SimpleAgent()
{
	_health = 100;
	_position = Vector2(0.0f, 0.0f);
}


SimpleAgent::~SimpleAgent()
{
}

SimpleAgent::SimpleAgent(Vector2 position, GameManager & manager)
{
	_health = 100;
	_position = position;
	_mgr = &manager;
	// This is temporary so we can test whether AABB can be created from texture dimensions
	_sprite = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_dirt.png");
	_aabb = AABB(*_sprite, *this);
}

void SimpleAgent::Unload()
{
	delete _sprite;
}

SDL2pp::Texture* SimpleAgent::GetTexture()
{
	return _sprite;
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

	_aabb.UpdatePosition(*this);
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

void SimpleAgent::SetPosition(Vector2 &newPos)
{
	_position = newPos;
}

Vector2 SimpleAgent::GetPosition()
{
	return _position;
}

bool SimpleAgent::CollisionCheck(SimpleAgent &otherAI)
{
	return _aabb.CheckCollision(otherAI.GetAABB());
}

AABB SimpleAgent::GetAABB()
{
	return _aabb;
}