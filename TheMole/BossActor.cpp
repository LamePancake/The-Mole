#include "BossActor.h"
#include "GameScreen.h"
#include <iostream>
BossActor::BossActor(Vector2 position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites, const std::string&& startSprite,
	SpriteSheet::XAxisDirection startXDirection, SpriteSheet::YAxisDirection startYDirection)
	: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection), _heat(0) {}

BossActor::~BossActor()
{
}

void BossActor::UpdatePosition(double elapsedSecs)
{
	Actor::UpdatePosition(elapsedSecs);
}

void BossActor::Draw(Camera & camera)
{
	Actor::Draw(camera);
}

void BossActor::Update(double elapsedSecs)
{
	Actor::Update(elapsedSecs);
	Vector2 playerPos = _gameScreen->GetPlayer()->GetPosition();
	Vector2 bossPos = GetPosition();
	_bossTree.UpdateVariables(&playerPos, &bossPos, _health, _heat);
	_bossTree.ExecuteTree();
}

void BossActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
}
