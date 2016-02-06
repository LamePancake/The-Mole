#include "ObjectActor.h"
#include "PlayerActor.h"
#include "GameScreen.h"

ObjectActor::~ObjectActor()
{
}

void ObjectActor::Draw(Camera & camera)
{
	if (!_isVisible) return;

	Actor::Draw(camera);
}

void ObjectActor::Update(double elapsedSecs)
{
	if (!_isVisible) return;

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

bool ObjectActor::IsVisisble()
{
	return _isVisible;
}

void ObjectActor::FlagUpdate(double elapsedSecs)
{
	const std::shared_ptr<GameScreen> screen = std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen());
	if (CollisionCheck(*(screen->GetPlayer())))
	{
		_sprite->Start();
		_spriteShadow->Start();
	}
}

void ObjectActor::PancakeUpdate(double elapsedSecs)
{
	const std::shared_ptr<GameScreen> screen = std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen());
	if (CollisionCheck(*(screen->GetPlayer())))
	{
		_isVisible = false;
	}
}

