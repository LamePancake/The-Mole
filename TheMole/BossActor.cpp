#include <iostream>
#include "GameScreen.h"
#include "ProjectileActor.h"
#include "BossActor.h"

#define HEAT_RATE 20

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
    _aabb.UpdatePosition(*this);
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

BossActor * BossActor::Clone()
{
    return nullptr;
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
    auto checkStage1 = [this](double deltaTime) { return _health > 50 ? Node::Result::Success : Node::Result::Failure; };
    auto checkStage2 = [this](double deltaTime) { return _health <= 50 && _health > 0 ? Node::Result::Success : Node::Result::Failure; };

    auto isPlayerClose = [this](double deltaTime)
    {
        shared_ptr<PlayerActor> player = _gameScreen->GetPlayer();
        Vector2 playerCentre = Vector2(player->GetAABB().GetX() + player->GetAABB().GetWidth() / 2, player->GetAABB().GetY() + player->GetAABB().GetHeight() / 2);
        Vector2 bossCentre = Vector2(_aabb.GetX() + _aabb.GetWidth() / 2, _aabb.GetY() + _aabb.GetHeight() / 2);
        float distToPlayer = playerCentre.Distance(bossCentre);

        return distToPlayer < 30 ? Node::Result::Success : Node::Result::Failure;
    };

    auto notOverheated = [this](double deltaTime)
    {
        return _heat < 100 ? Node::Result::Success : Node::Result::Failure;
    };

    auto prePunch = [this](double deltaTime)
    {
        if (_currentSpriteSheet != "prepunch")
        {
            _sprites[_currentSpriteSheet]->Stop();
            _currentSpriteSheet = "prepunch";
            _sprites[_currentSpriteSheet]->Start();
            _curKinematic.velocity.SetX(0);
            return Node::Result::Running;
        }

        return _sprites[_currentSpriteSheet]->IsFinished() ? Node::Result::Success : Node::Result::Running;
    };

    auto punch = [this](double deltaTime)
    {
        if (_currentSpriteSheet != "punch")
        {
            _sprites[_currentSpriteSheet]->Stop();
            _currentSpriteSheet = "punch";
            _sprites[_currentSpriteSheet]->Start();
            return Node::Result::Running;
        }

        return _sprites[_currentSpriteSheet]->IsFinished() ? Node::Result::Success : Node::Result::Running;
    };
    
    auto preRoll = [this](double deltaTime)
    {
        if (_currentSpriteSheet != "preroll")
        {
            _curKinematic.velocity.SetX(0);
            SetSprite("preroll");
            return Node::Result::Running;
        }
        return _sprites[_currentSpriteSheet]->IsFinished() ? Node::Result::Success : Node::Result::Running;
    };

    auto shouldPreRoll = [this](double deltaTime)
    {
        return _currentSpriteSheet != "roll" && _currentSpriteSheet != "preroll" ? Node::Result::Success : Node::Result::Failure;
    };

    auto roll = [this](double deltaTime)
    {
        float playerX = _gameScreen->GetPlayer()->GetPosition().GetX();
        float bossX = _curKinematic.position.GetX();
        _curKinematic.velocity.SetX(300.f * (playerX - bossX < 0 ? -1 : 1));
        _heat += HEAT_RATE * deltaTime;
        if (_currentSpriteSheet != "roll")
        {
            SetSprite("roll");
        }
        return Node::Result::Running;
    };

    auto overheat = [this](double deltaTime)
    {
        if (_currentSpriteSheet != "overheat")
        {
            _curKinematic.velocity.SetX(0);
            SetSprite("overheat");
            return Node::Result::Running;
        }
        if (_sprites[_currentSpriteSheet]->IsFinished())
        {
            _heat = 0;
            return Node::Result::Success;
        }
        return Node::Result::Running;
    };

    auto shortHop = [this](double deltaTime)
    {
        cout << "hop" << endl;
        return Node::Result::Running;
    };

    auto shockWave = [this](double deltaTime)
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

   auto idle = [this](double deltaTime)
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

   auto eject = [this](double elapsedSecs)
   {
       cout << "eject" << endl;
       return Node::Result::Running;
   };

   // TODO: Draw the entire behaviour tree (here or somewhere within the repo) and point to it
   shared_ptr<Selector> root = shared_ptr<Selector>(new Selector);

   // While health is > 50, we'll go into this sequence (the first "phase" of the battle where the Underwatch
   // only uses rolling and punches)
   shared_ptr<Sequence> firstStageSeq = shared_ptr<Sequence>(new Sequence);
   firstStageSeq->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(checkStage1))));

   shared_ptr<Selector> attackSelector = shared_ptr<Selector>(new Selector);
   shared_ptr<Sequence> closeAttackSequence = shared_ptr<Sequence>(new Sequence);

   // If the player is close, do the pre-punch animation, the punch animation, and then idle for a bit
   closeAttackSequence->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(isPlayerClose))));
   closeAttackSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(prePunch))));
   closeAttackSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(punch))));
   closeAttackSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(idle))));

   // Tf the player is far, do the pre-roll, then continue rolling until overheating or getting close enough to punch the player
   shared_ptr<Selector> rollActionSelector = shared_ptr<Selector>(new Selector);
   shared_ptr<Sequence> preRollSequence = shared_ptr<Sequence>(new Sequence);
   shared_ptr<Sequence> rollSequence = shared_ptr<Sequence>(new Sequence);
   shared_ptr<Sequence> overheatSequence = shared_ptr<Sequence>(new Sequence);

   preRollSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(shouldPreRoll))));
   preRollSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(preRoll))));

   rollSequence->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(notOverheated))));
   rollSequence->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(roll))));

   overheatSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(overheat))));
   overheatSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(idle))));

   rollActionSelector->AddChild(preRollSequence);
   rollActionSelector->AddChild(rollSequence);
   rollActionSelector->AddChild(overheatSequence);

   attackSelector->AddChild(closeAttackSequence);
   attackSelector->AddChild(rollActionSelector);

   firstStageSeq->AddChild(attackSelector);

   root->AddChild(firstStageSeq);

   _bossTree = BossBehavTree(root, 0.1f);
}