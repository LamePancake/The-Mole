#include "PlayerActor.h"

PlayerActor::~PlayerActor()
{
}

void PlayerActor::Draw(Camera& camera)
{
	Actor::Draw(camera);
}

void PlayerActor::Update(std::shared_ptr<Level>& level)
{
	Actor::Update(level);
}

void PlayerActor::UpdatePosition()
{
	Actor::UpdatePosition();
}
