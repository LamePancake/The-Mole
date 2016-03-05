#ifndef TURRETACTOR_HEADER
#define TURRETACTOR_HEADER

#include "TurretActor.h"
#include "GameScreen.h"
#include <memory>

TurretActor::TurretActor(
	Vector2 position
	, GameManager & manager
	, Vector2 spd
	, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites
	, const std::string && startSprite
	, SpriteSheet::XAxisDirection startXDirection
	, SpriteSheet::YAxisDirection startYDirection
	): Actor(position, manager, spd, sprites, std::move(startSprite)
		, startXDirection, startYDirection)
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
		//_gameScreen->SpawnProjectileActors(std::make_shared<ProjectileActor>(
		//	_curKinematic.position ///Vec2 position
		//	, *_mgr ///Gamemanager
		//	, Vector2(100.0f, 0.0f) ///Vec2 spd
		//	, _sprites ///sprites
		//	, "turret" ///startsprite
		//	, SpriteSheet::XAxisDirection::LEFT) ///direction
		//	);
}

#endif