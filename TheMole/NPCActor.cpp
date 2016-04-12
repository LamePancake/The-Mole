#include "NPCActor.h"
#include "GameScreen.h"

NPCActor::~NPCActor()
{
}

void NPCActor::Draw(Camera & camera)
{
	Actor::Draw(camera);
}

void NPCActor::Update(double elapsedSecs)
{
    if (_isDestroyed || !_isActive) return;
	Actor::Update(elapsedSecs);

    UpdatePosition(elapsedSecs);
	_aabb.UpdatePosition(*this);

    // If we're moving, reverse when hitting walls
    if (_curKinematic.velocity.GetX() != 0 || _curKinematic.velocity.GetY() != 0)
    {
        _collisionInfo.colIntersect.clear();
        _collisionInfo.rowIntersect.clear();
        DetectTileCollisions(_collisionInfo, _gameScreen->GetLevel());
        if (_collisionInfo.shouldCorrectX)
        {
            float correction = _collisionInfo.colEdge == Edge::RIGHT ? _collisionInfo.colPenetration * -1 : _collisionInfo.colPenetration;
            _curKinematic.position.SetX(_curKinematic.position.GetX() + correction);
            _curKinematic.velocity.SetX(_curKinematic.velocity.GetX() * -1);
            _spriteXDir = _spriteXDir == SpriteSheet::XAxisDirection::LEFT ? SpriteSheet::XAxisDirection::RIGHT : SpriteSheet::XAxisDirection::LEFT;
        }
        if (_collisionInfo.shouldCorrectY)
        {
            float correction = _collisionInfo.rowEdge == Edge::BOTTOM ? _collisionInfo.rowPenetration * -1 : _collisionInfo.rowPenetration;
            _curKinematic.position.SetY(_curKinematic.position.GetY() + correction);
        }

        _prevKinematic = _curKinematic;
    }
}

void NPCActor::UpdatePosition(double elapsedSecs)
{
    float deltaX = _curKinematic.velocity.GetX() * elapsedSecs;
    float deltaY = _curKinematic.velocity.GetY() * elapsedSecs;

    _curKinematic.position.SetX(_curKinematic.position.GetX() + deltaX);
    _curKinematic.position.SetY(_curKinematic.position.GetY() + deltaY);
}

void NPCActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
}

NPCActor * NPCActor::Clone()
{
    return new NPCActor(*this);
}

