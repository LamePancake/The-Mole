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
	_aabb.UpdatePosition(*this);
}

void NPCActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
}

