#include "ObjectActor.h"
#include "PlayerActor.h"
#include "GameScreen.h"

ObjectActor::ObjectActor(Vector2 position, GameManager & manager, Vector2 spd, int id, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites, const std::string&& startSprite,
	SpriteSheet::XAxisDirection startXDirection, SpriteSheet::YAxisDirection startYDirection)
	: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection), _id(id), _collided(false)
{
	if (id == flag)
		_sprites[_currentSpriteSheet]->Pause();
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

void ObjectActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
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
	if (!_collided && CollisionCheck(*(_gameScreen->GetPlayer())))
	{
		_sprites[_currentSpriteSheet]->Start();
		_sprites[_currentSpriteSheet]->SetRepeating(false);

		_gameScreen->GetLevel()->SetSpawnPoint(GetPosition());
		_collided = true;
	}
}
 
void ObjectActor::PancakeUpdate(double elapsedSecs)
{
	if (CollisionCheck(*(_gameScreen->GetPlayer())))
	{
		_sprites[_currentSpriteSheet]->Stop();
		SetVisibility(false);
	}
}

void ObjectActor::ProjectileThrowerUpdate(double elapseSecs)
{
	const std::shared_ptr<GameScreen> screen = std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen());
	if (CollisionCheck(*(screen->GetPlayer())))
	{
		_sprites[_currentSpriteSheet]->Stop();
		SetVisibility(false);
	}
}

