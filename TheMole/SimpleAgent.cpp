#include "SimpleAgent.h"



SimpleAgent::SimpleAgent()
{
}


SimpleAgent::~SimpleAgent()
{
}

SimpleAgent::SimpleAgent(Vector2 position, GameManager & manager, Vector2 spd)
{
	_speed = spd;
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

	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	if (_health <= 0)
	{
		std::cout << "Dead\n";
	}
	if (_health > 0)
	{
		std::cout << "Patrolling\n";
	}

	_aabb.UpdatePosition(*this);
	UpdatePosition();
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

void SimpleAgent::UpdatePosition()
{
	_position.SetX(_position.GetX() + _speed.GetX());
	_position.SetY(_position.GetY() + _speed.GetY());
}

Vector2 SimpleAgent::GetPosition()
{
	return _position;
}

void SimpleAgent::SetSpeed(Vector2 spd)
{
	_speed = spd;
}

bool SimpleAgent::CollisionCheck(SimpleAgent &otherAI)
{
	return _aabb.CheckCollision(otherAI.GetAABB());
}

AABB SimpleAgent::GetAABB()
{
	return _aabb;
}