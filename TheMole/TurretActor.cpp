#ifndef TURRETACTOR_HEADER
#define TURRETACTOR_HEADER

#include "TurretActor.h"
#include "GameScreen.h"
#include "BombAIActor.h"
#include "ProjectileActor.h"
#include "ToggleActor.h"
#include <memory>
#include <math.h>

TurretActor::TurretActor(
	Vector2 position
	, GameManager & manager
	, Vector2 spd
	, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites
	, const std::string && startSprite
	, SpriteSheet::XAxisDirection startXDirection
	, SpriteSheet::YAxisDirection startYDirection
	) : Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection), _aim(Aim::None), _reflectable(false)
{
	_timeInterval = 0;
    _period = 3;
}

TurretActor::TurretActor(Vector2 position
	, GameManager & manager
	, Vector2 spd
	, Aim aim
    , bool reflectable
    , double period
	, std::unordered_map<std::string
	, std::shared_ptr<SpriteSheet>>& sprites
	, const std::string && startSprite
	, SpriteSheet::XAxisDirection startXDirection
	, SpriteSheet::YAxisDirection startYDirection
	) : Actor(position, manager, spd, sprites, std::move(startSprite)
		, startXDirection, startYDirection), _aim(aim), _reflectable(reflectable), _period(period)
{
	_timeInterval = 0;
    _sprites["turret"]->Stop();
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
    TurretUpdate(elapsedSecs);
    _aabb.UpdatePosition(*this);
	if (_isDestroyed || !_isActive) return;

	for (auto & actor : _gameScreen->GetLevel()->GetActors())
	{
        if (actor->GetAABB().CheckCollision(_aabb))
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
            else if (actor->GetType() == Type::projectile)
            {
                auto projectile = dynamic_pointer_cast<ProjectileActor>(actor);
                if (projectile->IsReflectable() && projectile->WasReflected())
                {
                    // Major hacks in-bound
                    // The only timte this will ever happen is in the boss battle...
                    // So we're going to do some logic here that probably shouldn't be a thing
                    SetActive(false);
                    auto switches = _gameScreen->GetLevel()->GetActorsOfType<ToggleActor>(Type::toggle);
                    switches[2]->SetVisibility(true);
                    switches[2]->SetActive(true);

                    _sprites[_currentSpriteSheet]->Stop();
                    _currentSpriteSheet = "dead";
                    _sprites[_currentSpriteSheet]->Start();

                    projectile->Destroy();
                }
            }
        }
	}
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

void TurretActor::ChangeAim(Aim newAim)
{
    _aim = newAim;
}

Vector2 TurretActor::GetProjectileVel()
{
    float playerX = _gameScreen->GetPlayer()->GetPosition().GetX();
    float myX = _curKinematic.position.GetX();

    int aimMultiplier = playerX < myX ? -1 : 1;
    if (_aim == Aim::XAxis)
    {
        Vector2 aim;
        aim.SetX(200 * aimMultiplier);
        aim.SetY(0);
        return aim;
    }
    else
    {
        Vector2 aim = _gameScreen->GetPlayer()->GetPosition() - _curKinematic.position;
        double length = sqrt(pow(aim.GetX(), 2) + pow(aim.GetY(), 2));
        aim = Vector2((aim.GetX() / length) * 100, (aim.GetY() / length) * 100);
        return aim;
    }
}

void TurretActor::TurretUpdate(double elapseSecs)
{
	_timeInterval += elapseSecs;

	Vector2 projectilePosition = Vector2();
	if (this->_spriteXDir == SpriteSheet::XAxisDirection::LEFT)
		projectilePosition = Vector2(_curKinematic.position.GetX() - (_sprites["shoot"]->GetFrameWidth()), _curKinematic.position.GetY() + (_sprites[_currentSpriteSheet]->GetFrameHeight() / 2.0f) - (_sprites["shoot"]->GetFrameHeight() / 2.0f));
	else
		projectilePosition = Vector2(_curKinematic.position.GetX() + _sprites[_currentSpriteSheet]->GetFrameWidth() + (_sprites["shoot"]->GetFrameWidth()), _curKinematic.position.GetY() + (_sprites[_currentSpriteSheet]->GetFrameHeight() / 2.0f) - (_sprites["shoot"]->GetFrameHeight() / 2.0f));

    if (_aim != Aim::None)
    {
        SpriteSheet::XAxisDirection xDir = _gameScreen->GetPlayer()->GetPosition().GetX() < _curKinematic.position.GetX()
                                            ? SpriteSheet::XAxisDirection::LEFT : SpriteSheet::XAxisDirection::RIGHT;
        _spriteXDir = xDir;
    }

    if (_timeInterval > _period)
    {
        _sprites["turret"]->Reset();
        _sprites["turret"]->Start();
        // Play a sound
		if (_aim != Aim::None)
		{
            auto projectile = std::make_shared<ProjectileActor>(
                projectilePosition //- Vector2(0, -50) ///Vec2 position
                , *_mgr ///Gamemanager
                , GetProjectileVel()
                , _sprites ///sprites
                , "shoot" ///startsprite
                , this->_spriteXDir); ///direction

            projectile->SetReflectable(_reflectable);
            _gameScreen->GetLevel()->AddActor(projectile);
		}
		else if (_pattern.size() == 0)
		{
            auto projectile = std::make_shared<ProjectileActor>(
                projectilePosition //- Vector2(0, -50) ///Vec2 position
                , *_mgr ///Gamemanager
                , this->_startXDir == SpriteSheet::XAxisDirection::LEFT ? Vector2(-200.0f, 0.0f) : Vector2(200.0f, 0.0f) ///Vec2 spd
                , _sprites ///sprites
                , "shoot" ///startsprite
                , this->_startXDir); ///direction
                
            projectile->SetReflectable(_reflectable);
            _gameScreen->GetLevel()->AddActor(projectile);
		}
		else
		{
			for (size_t i = 0; i < _pattern.size(); i++)
			{
                auto projectile = std::make_shared<ProjectileActor>(
                    projectilePosition //- Vector2(0, -50) ///Vec2 position
                    , *_mgr ///Gamemanager
                    , this->_startXDir == SpriteSheet::XAxisDirection::LEFT ? Vector2(-200.0f, 0.0f) : Vector2(200.0f, 0.0f) ///Vec2 spd
                    , _sprites ///sprites
                    , "shoot" ///startsprite
                    , this->_startXDir); ///direction

                projectile->SetReflectable(_reflectable);
                _gameScreen->GetLevel()->AddActor(projectile);
			}
		}
        _timeInterval = 0;
	}
}

#endif