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
	_timeInterval = 0;
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

void TurretActor::SetPattern(Vector2 prjDir)
{
	_pattern.push_back(prjDir);
}

void TurretActor::SetPattern(std::vector<Vector2> prjDirSet)
{
	_pattern = prjDirSet;
}

void TurretActor::TurretUpdate(double elapseSecs)
{
	_timeInterval += elapseSecs;
	if (_timeInterval > 5)
	{
		if (_pattern.size() == 0)
		{
			_gameScreen->GetLevel()->AddActor(std::make_shared<ProjectileActor>(
				_curKinematic.position //- Vector2(0, -50) ///Vec2 position
				, *_mgr ///Gamemanager
				, Vector2(-200.0f, 0.0f) ///Vec2 spd
				, _sprites ///sprites
				, "shoot" ///startsprite
				, SpriteSheet::XAxisDirection::LEFT) ///direction
				);
		}
		else
		{
			for (size_t i = 0; i < _pattern.size(); i++)
			{
				_gameScreen->GetLevel()->AddActor(std::make_shared<ProjectileActor>(
					_curKinematic.position //- Vector2(0, -50) ///Vec2 position
					, *_mgr ///Gamemanager
					, _pattern[i] ///Vec2 spd
					, _sprites ///sprites
					, "shoot" ///startsprite
					, SpriteSheet::XAxisDirection::LEFT) ///direction
					);
			}
		}
		if (_timeInterval > 5) {
			_gameScreen->GetLevel()->AddActor(std::make_shared<ProjectileActor>(
				_curKinematic.position //- Vector2(0, -50) ///Vec2 position
				, *_mgr ///Gamemanager
				, Vector2(-200.0f, 0.0f) ///Vec2 spd
				, _sprites ///sprites
				, "shoot" ///startsprite
				, SpriteSheet::XAxisDirection::LEFT) ///direction
				);
			_timeInterval = elapseSecs;
		}
	}
}

#endif