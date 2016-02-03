#include "Player.h"

Player::~Player()
{
}

void Player::Draw(std::shared_ptr<Level>& level)
{
}

AABB Player::GetAABB()
{
	return AABB();
}

Player::Player(Vector2 position, GameManager & manager, Vector2 spd)
{
}

void Player::Update(std::shared_ptr<Level>& level)
{
}

void Player::UpdatePosition()
{
}

Vector2 Player::GetPosition()
{
	return Vector2();
}
