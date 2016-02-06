#include "ObjectActor.h"
#include "PlayerActor.h"
#include "GameScreen.h"

ObjectActor::ObjectActor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath, int id, int framesPerSecond)
	: Actor(position, manager, spd, texturePath, framesPerSecond), _id(id)
{
	_sprite->Pause();
	_spriteShadow->Pause();
}

ObjectActor::~ObjectActor()
{
}

void ObjectActor::Draw(Camera & camera)
{
	Actor::Draw(camera);
}

void ObjectActor::Update(double elapsedSecs)
{
	Actor::Update(elapsedSecs);
	_aabb.UpdatePosition(*this);

	switch (_id)
	{
		case flag:
		{
			FlagUpdate(elapsedSecs);
			break;
		}
		case pancake:
		{
			PancakeUpdate(elapsedSecs);
			break;
		}
	}
}

bool ObjectActor::CollisionCheck(Actor & otherAI)
{
	return _aabb.CheckCollision(otherAI.GetAABB());
}

int ObjectActor::GetID()
{
	return _id;
}

void ObjectActor::SetID(int id)
{
	_id = id;
}

void ObjectActor::FlagUpdate(double elapsedSecs)
{
	const std::shared_ptr<GameScreen> screen = std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen());
	if (CollisionCheck(*(screen->GetPlayer())))
	{
		_sprite->Start();
		_spriteShadow->Start();
		_sprite->SetRepeating(false);
		_spriteShadow->SetRepeating(false);
	}
}

void ObjectActor::PancakeUpdate(double elapsedSecs)
{
	const std::shared_ptr<GameScreen> screen = std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen());
	if (CollisionCheck(*(screen->GetPlayer())))
	{
		_sprite->Stop();
		_spriteShadow->Stop();
		_sprite->SetDraw(false);
		_spriteShadow->SetDraw(false);
	}
}

