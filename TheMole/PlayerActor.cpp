#include "PlayerActor.h"

PlayerActor::~PlayerActor()
{
}

void PlayerActor::Draw(std::shared_ptr<Level>& level)
{
	Actor::Draw(level);
}

void PlayerActor::Update(std::shared_ptr<Level>& level)
{
	Actor::Update(level);
}

void PlayerActor::UpdatePosition()
{
	Actor::UpdatePosition();
}
