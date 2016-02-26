#ifndef BOSSACTOR_H
#define BOSSACTOR_H
#include "Actor.h"
#include "PlayerActor.h"
#include "BossBehavTree.h"
#pragma once
using namespace std;
class BossActor : public Actor
{
public:
	BossActor(shared_ptr<PlayerActor> player, Vector2 position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites, const std::string&& startSprite,
		SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT, SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP)
		: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection) 
	{
		_player = player;
		_bossTree = BossBehavTree();
		_health = 100;
		_heat = 0;
	}
	~BossActor();
	void Update();

private:
	shared_ptr<PlayerActor> _player;
	BossBehavTree _bossTree;
	int _health;
	int _heat;
};
#endif 