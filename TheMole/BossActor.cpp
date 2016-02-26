#include "BossActor.h"
#include "GameScreen.h"
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
	_bossTree.UpdateVariables(_gameScreen->GetPlayer()->GetPosition(), _curKinematic.position, _health, _heat);
	_bossTree.ExecuteTree();
}