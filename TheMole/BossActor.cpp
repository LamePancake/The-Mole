#include "BossActor.h"
#include "GameScreen.h"
#include <iostream>
#include "BossBehavTree.h"

BossActor::BossActor(Vector2 position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites, const std::string&& startSprite,
	SpriteSheet::XAxisDirection startXDirection, SpriteSheet::YAxisDirection startYDirection)
	: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection), _heat(0) {}

BossActor::~BossActor()
{
}

void BossActor::UpdatePosition(double elapsedSecs)
{
	Actor::UpdatePosition(elapsedSecs);
	Vector2 target = _bossTree.GetTarget();
	if (_curKinematic.position.Distance(_bossTree.GetTarget()) > 10)
	{
		if (_curKinematic.position.GetX() > _bossTree.GetTarget().GetX())
		{
			_curKinematic.position.SetX((float)_curKinematic.position.GetX() - ((float)_curKinematic.velocity.GetX() * (float)elapsedSecs));
		}
		else
		{
			_curKinematic.position.SetX((float)_curKinematic.position.GetX() + ((float)_curKinematic.velocity.GetX() * (float)elapsedSecs));
		}
		//cout << "boss target Pos: " << _bossTree.GetTarget().GetX() << endl;
		//cout << "boss Pos: " << _curKinematic.position.GetX() << endl;
	}
}

void BossActor::Draw(Camera & camera)
{
	Actor::Draw(camera);
}

void BossActor::Update(double elapsedSecs)
{
	Actor::Update(elapsedSecs);
	_playerPos = _gameScreen->GetPlayer()->GetPosition();
	_bossPos = GetPosition();
	_bossTree.UpdateVariables(&_playerPos, &_bossPos, _health, _heat, elapsedSecs);
	_bossTree.ExecuteTree();

	UpdatePosition(elapsedSecs);

	if (_idleDur <= 0)
	{
		ResetDurations();
	}
}

void BossActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
}

void BossActor::ResetDurations()
{
	_idleDur      = 2;
	_preRollDur   = 2;
	_rollDur      = 5;
	_prePunchDur  = 2;
	_punchDur     = 1;
	_hopDur       = 1;
	_shockWaveDur = 5;
}