#include "ProjectileActor.h"
#include "GameScreen.h"

ProjectileActor::ProjectileActor(Vector2 position,
                                 GameManager & manager,
                                 Vector2 spd,
                                 std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
                                 const std::string && startSprite,
                                 std::unordered_map<std::string, std::pair<std::string, bool>> &sounds,
                                 bool reflectable,
	                             SpriteSheet::XAxisDirection startXDirection, SpriteSheet::YAxisDirection startYDirection)
	: Actor(position, manager, spd, sprites, std::move(startSprite), sounds, startXDirection, startYDirection), _reflectable(reflectable), _wasReflected(false)
{
}

ProjectileActor::~ProjectileActor()
{
}

void ProjectileActor::Draw(Camera & camera)
{
	Actor::Draw(camera);
}

void ProjectileActor::Update(double elapsedSecs)
{
	Actor::Update(elapsedSecs);
	if (_isDestroyed || !_isActive) return;

	_collisionInfo.colIntersect.clear();
	_collisionInfo.rowIntersect.clear();

    _spriteXDir = _curKinematic.velocity.GetX() < 0 ? SpriteSheet::XAxisDirection::LEFT : SpriteSheet::XAxisDirection::RIGHT;

	DetectTileCollisions(_collisionInfo, _gameScreen->GetLevel());
	if (_collisionInfo.shouldCorrectX || _collisionInfo.shouldCorrectY) {

		_sprites[_currentSpriteSheet]->Stop();
		Destroy();
	}

	UpdatePosition(elapsedSecs);
    _aabb.UpdatePosition(*this);
	ProjectileUpdate(elapsedSecs);
}

void ProjectileActor::Reset(Vector2 pos)
{
	Destroy();
}

bool ProjectileActor::IsReflectable()
{
    return _reflectable;
}

void ProjectileActor::SetReflectable(bool reflectable)
{
    _reflectable = reflectable;
}

bool ProjectileActor::WasReflected() const
{
    return _wasReflected;
}

ProjectileActor * ProjectileActor::Clone()
{
	return new ProjectileActor(*this);
}

void ProjectileActor::UpdatePosition(double elapsedSecs) {
	// Copy everything over so that we can use this in collision detection stuff later
	_prevKinematic = _curKinematic;

	Actor::UpdatePosition(elapsedSecs);

	_curKinematic.position.SetX(_curKinematic.position.GetX() + _curKinematic.velocity.GetX() * elapsedSecs * 2);
	_curKinematic.position.SetY(_curKinematic.position.GetY() + _curKinematic.velocity.GetY() * elapsedSecs * 2);
}

bool ProjectileActor::CollisionCheck(Actor & otherAI)
{
	return _aabb.CheckCollision(otherAI.GetAABB());
}

void ProjectileActor::ProjectileUpdate(double elapseSecs)
{
	const std::shared_ptr<GameScreen> screen = std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen());
    for (auto actor : _gameScreen->GetLevel()->GetActors())
    {
        if (_aabb.CheckCollision(actor->GetAABB()))
        {
            switch (actor->GetType())
            {
            case Type::player:
            {
                Vector2 vel = _curKinematic.velocity;
                screen->GetPlayer()->ProjectileHit(this);

                // Hacky check for having been reflected
                if (_curKinematic.velocity.GetX() == vel.GetX())
                {
                    _sprites[_currentSpriteSheet]->Stop();
                    Destroy();
                }
                else
                {
                    // Set the projectile back to whatever its position was last frame so that we don't continuously hit the player
                    _curKinematic.position = _prevKinematic.position;
                    _wasReflected = true;
                }
            }
            break;
            // Fall through
            case Type::door:
            case Type::enemy:
            case Type::bombenemy:
                _sprites[_currentSpriteSheet]->Stop();
                Destroy();
            }
        }
    }
    _prevKinematic = _curKinematic;
}
