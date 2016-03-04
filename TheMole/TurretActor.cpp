#include "TurretActor.h"

TurretActor::TurretActor(Vector2 position
	, GameManager & manager
	, Vector2 spd
	, std::unordered_map<std::string
	, std::shared_ptr<SpriteSheet>>& sprites
	, const std::string && startSprite
	, SpriteSheet::XAxisDirection startXDirection
	, SpriteSheet::YAxisDirection startYDirection)
	: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection)
{
	_sprites[_currentSpriteSheet];
}

TurretActor::~TurretActor()
{
}

void TurretActor::Draw(Camera & camera)
{
	Actor::Draw(camera);
}

void TurretActor::Update(double elapsedSecs)
{
	Actor::Update(elapsedSecs);
	_aabb.UpdatePosition(*this);
	TurretUpdate(elapsedSecs);
}

void TurretActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
}

bool TurretActor::CollisionCheck(Actor & otherAI)
{
	return false;
}

void TurretActor::TurretUpdate(double elapseSecs)
{
}
