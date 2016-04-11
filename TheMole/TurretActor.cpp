#ifndef TURRETACTOR_HEADER
#define TURRETACTOR_HEADER

#include "TurretActor.h"
#include "GameScreen.h"
#include "BombAIActor.h"
#include "ProjectileActor.h"
#include <memory>

TurretActor::TurretActor(
	Vector2 position
	, GameManager & manager
	, Vector2 spd
	, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites
	, const std::string && startSprite
	, SpriteSheet::XAxisDirection startXDirection
	, SpriteSheet::YAxisDirection startYDirection
	) : Actor(position, manager, spd, sprites, std::move(startSprite)
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
	if (_isDestroyed || !_isActive) return;

	for (auto & actor : _gameScreen->GetLevel()->GetActors())
	{
		if (actor->GetType() == Type::bombenemy)
		{
			auto bomber = dynamic_pointer_cast<BombAIActor>(actor);
			if (CollisionCheck(*bomber) && bomber->IsUnderMindControl())
			{
				bomber->BlowUp();
				SetActive(false);
				SetVisibility(false);
				return;
			}
		}
	}

	TurretUpdate(elapsedSecs);
}

void TurretActor::Reset(Vector2 pos)
{
	_timeInterval = 0;
	Actor::Reset(pos);
	SetActive(true);
	SetVisibility(true);
}

TurretActor * TurretActor::Clone()
{
	return new TurretActor(*this);
}

bool TurretActor::CollisionCheck(Actor & otherAI)
{
	return _aabb.CheckCollision(otherAI.GetAABB());
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

	Vector2 projectilePosition = Vector2();
	if (this->_startXDir == SpriteSheet::XAxisDirection::LEFT)
		projectilePosition = Vector2(_curKinematic.position.GetX() - (_sprites["shoot"]->GetFrameWidth()), _curKinematic.position.GetY() + (_sprites[_currentSpriteSheet]->GetFrameHeight() / 2.0f) - (_sprites["shoot"]->GetFrameHeight() / 2.0f));
	else
		projectilePosition = Vector2(_curKinematic.position.GetX() + _sprites[_currentSpriteSheet]->GetFrameWidth() + (_sprites["shoot"]->GetFrameWidth()), _curKinematic.position.GetY() + (_sprites[_currentSpriteSheet]->GetFrameHeight() / 2.0f) - (_sprites["shoot"]->GetFrameHeight() / 2.0f));

	if (_timeInterval > 0.25)
	{
		if (_pattern.size() == 0)
		{
			_gameScreen->GetLevel()->AddActor(std::make_shared<ProjectileActor>(
				projectilePosition //- Vector2(0, -50) ///Vec2 position
				, *_mgr ///Gamemanager
				, this->_startXDir == SpriteSheet::XAxisDirection::LEFT ? Vector2(-200.0f, 0.0f) : Vector2(200.0f, 0.0f) ///Vec2 spd
				, _sprites ///sprites
				, "shoot" ///startsprite
				, this->_startXDir) ///direction
				);
		}
		else
		{
			for (size_t i = 0; i < _pattern.size(); i++)
			{
				_gameScreen->GetLevel()->AddActor(std::make_shared<ProjectileActor>(
					projectilePosition //- Vector2(0, -50) ///Vec2 position
					, *_mgr ///Gamemanager
					, this->_startXDir == SpriteSheet::XAxisDirection::LEFT ? _pattern[i] : Vector2(_pattern[i].GetX() * -1, _pattern[i].GetY())  ///Vec2 spd
					, _sprites ///sprites
					, "shoot" ///startsprite
					, this->_startXDir) ///direction
					);
			}
		}
		if (_timeInterval > 3) {
			_gameScreen->GetLevel()->AddActor(std::make_shared<ProjectileActor>(
				projectilePosition //- Vector2(0, -50) ///Vec2 position
				, *_mgr ///Gamemanager
				, this->_startXDir == SpriteSheet::XAxisDirection::LEFT ? Vector2(-200.0f, 0.0f) : Vector2(200.0f, 0.0f)///Vec2 spd
				, _sprites ///sprites
				, "shoot" ///startsprite
				, this->_startXDir) ///direction
				);
			_timeInterval = elapseSecs;
		}
	}
}

#endif