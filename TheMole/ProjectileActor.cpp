#include "ProjectileActor.h"
#include "PlayerActor.h"
#include "GameScreen.h"

ProjectileActor::ProjectileActor(Vector2 position
	, GameManager & manager
	, Vector2 spd
	, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites
	, const std::string && startSprite
	, SpriteSheet::XAxisDirection startXDirection, SpriteSheet::YAxisDirection startYDirection)
	: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection)
{
	_sprites[_currentSpriteSheet];
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
	_aabb.UpdatePosition(*this);
	UpdatePosition(elapsedSecs);
	ProjectileUpdate(elapsedSecs);
}

void ProjectileActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
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
	_collisionInfo.colIntersect.clear();
	_collisionInfo.rowIntersect.clear();
	DetectTileCollisions(_collisionInfo, _gameScreen->GetLevel());
	if (_collisionInfo.colPenetration > 5) {
		return true;
	}
	return _aabb.CheckCollision(otherAI.GetAABB());
}


void ProjectileActor::ProjectileUpdate(double elapseSecs)
{
	const std::shared_ptr<GameScreen> screen = std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen());
	if (CollisionCheck(*(screen->GetPlayer())))
	{
		_sprites[_currentSpriteSheet]->Stop();
		SetVisibility(false);
	}
}
