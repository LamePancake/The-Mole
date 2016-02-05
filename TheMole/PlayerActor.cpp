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
	UpdatePosition();
}

void PlayerActor::UpdatePosition()
{
	Actor::UpdatePosition();
	_position.SetX(_position.GetX() + _speed.GetX());
	_position.SetY(_position.GetY() + _speed.GetY());
}
