#include <iostream>
#include "GameScreen.h"
#include "ProjectileActor.h"
#include "BossActor.h"

using std::shared_ptr;

BossActor::BossActor(Vector2 position,
          GameManager & manager,
          Vector2 spd,
          std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
          const std::string&& startSprite,
          shared_ptr<Actor> projectile,
          SpriteSheet::XAxisDirection startXDirection,
          SpriteSheet::YAxisDirection startYDirection)
	: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection), _heat(0), _projPrototype(projectile)
{
    CreateBehaviourTree();
}

BossActor::~BossActor()
{
}

void BossActor::UpdatePosition(double elapsedSecs)
{
	Actor::UpdatePosition(elapsedSecs);
    _curKinematic.position.SetX(_curKinematic.position.GetX() + elapsedSecs * _curKinematic.velocity.GetX());
    _curKinematic.position.SetY(_curKinematic.position.GetY() + elapsedSecs * _curKinematic.velocity.GetY());
}

void BossActor::Draw(Camera & camera)
{
	Actor::Draw(camera);
}

void BossActor::Update(double elapsedSecs)
{
	Actor::Update(elapsedSecs);
    if (_isDestroyed || !_isActive) return;

    _bossTree.Update(elapsedSecs);
	UpdatePosition(elapsedSecs);
}

void BossActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
}

void BossActor::ResetDurations()
{
	_idleDur      = 2;
	_preRollDur   = 2;
	_rollDur      = 10;
	_prePunchDur  = 2;
	_punchDur     = 1;
	_hopDur       = 2;
	_shockWaveDur = 5;
}

void BossActor::SetSprite(string name)
{
	_sprites[_currentSpriteSheet]->Stop();
	_currentSpriteSheet = name;
	SetActorYDirection(SpriteSheet::YAxisDirection::UP);
	_sprites[_currentSpriteSheet]->Start();
}

void BossActor::CreateBehaviourTree()
{
    // Simple state checks
    auto checkHeatFunc = [this](double deltaTime) { return _heat < 100 ? Node::Result::Success : Node::Result::Failure; };
    auto checkOverheatedFunc = [this](double deltaTime) { return _heat >= 100 ? Node::Result::Success : Node::Result::Failure; };
    auto checkAliveFunc = [this](double deltaTime) { return _health > 0 ? Node::Result::Success : Node::Result::Failure; };
    auto checkDeadFunc = [this](double deltaTime) { return _health <= 0 ? Node::Result::Success : Node::Result::Failure; };

    // Prepare to punch the player; switch to a new sprite
    auto prePunchFunc = [this](double deltaTime)
    {
        float distToPlayer = _curKinematic.position.Distance(_gameScreen->GetPlayer()->GetPosition());
        if (distToPlayer < 10)
        {
            SetSprite("prepunch");
            return Node::Result::Running;
        }
        return Node::Result::Success;
    };

    // 
    auto punchFunc = [this](double deltaTime)
    {
        if (_rollDur > 0)
        {
            // TODO: Figure out what the actual target position is supposed to do
            cout << "punch" << endl;
            //*_targetPos = _gameScreen->GetPlayer()->GetPosition();
            _rollDur -= deltaTime;
            SetSprite("punch");
            return Node::Result::Running;
        }
        return Node::Result::Success;
    };

    auto preRollFunc = [this](double deltaTime)
    {
        float distToPlayer = _curKinematic.position.Distance(_gameScreen->GetPlayer()->GetPosition());
        return distToPlayer > 10;
    };

    auto rollFunc = [this](double deltaTime)
    {
        if (_rollDur > 0)
        {
            cout << "roll" << endl;
            float playerX = _gameScreen->GetPlayer()->GetPosition().GetX();
            float bossX = _curKinematic.position.GetX();
            _curKinematic.velocity.SetX(300.f * (playerX - bossX < 0 ? -1 : 1));
            //*_targetPos = _gameScreen->GetPlayer()->GetPosition();
            //cout << "bPos: " << targetPos->GetX() << endl;
            //cout << "pPos: " << _gameScreen->GetPlayer()->GetPosition().GetX() << endl;
            //_rollDur -= deltaTime;
            SetSprite("roll");
            return Node::Result::Running;
        }
        else
        {
            ResetDurations();
            return Node::Result::Success;
        }
    };

    auto shortHopFunc = [this](double deltaTime)
    {
        cout << "hop" << endl;
        return Node::Result::Running;
    };

    auto shockWaveFunc = [this](double deltaTime)
    {
        if (_shockWaveDur > 0)
        {
            // Use prototype actor to create new one and add to level
            //_gameScreen->GetLevel()->AddActor(projectile);
            return Node::Result::Running;
        }
        else
        {
            return Node::Result::Success;
        }
    };

   auto idleFunc = [this](double deltaTime)
    {
        if (_idleDur > 0)
        {
            cout << "cooldown" << endl;
            _idleDur -= deltaTime;
            SetSprite("idle");
            return Node::Result::Running;
        }
        else
        {
            ResetDurations();
            return Node::Result::Success;
        }
   };

   auto ejectFunc = [this](double elapsedSecs)
   {
       cout << "eject" << endl;
       return Node::Result::Running;
   };

   shared_ptr<Node> root = shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(rollFunc)));
   _bossTree = BossBehavTree(root, 0.3f);
}