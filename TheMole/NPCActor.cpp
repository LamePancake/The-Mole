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
	Actor::Update(elapsedSecs);
	_aabb.UpdatePosition(*this);
}

