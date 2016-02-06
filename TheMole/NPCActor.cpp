#include "NPCActor.h"

NPCActor::~NPCActor()
{
}

void NPCActor::Draw(Camera & camera)
{
	Actor::Draw(camera);
}

void NPCActor::Update(double elapsedSecs, std::shared_ptr<Level>& level)
{
	Actor::Update(elapsedSecs, level);
	_aabb.UpdatePosition(*this);
}

