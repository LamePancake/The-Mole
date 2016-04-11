#include <iostream>
#include "GameScreen.h"
#include "BombAIActor.h"
#include "ProjectileActor.h"
#include "BossActor.h"
#include "ToggleActor.h"

#define HEAT_RATE 10
#define SLOW_RATE 200.f

using std::shared_ptr;

BossActor::BossActor(Vector2 position,
          GameManager & manager,
          Vector2 spd,
          std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
          const std::string&& startSprite,
          shared_ptr<Actor> alien,
          shared_ptr<TargetedActorSpawner> spawner,
          SpriteSheet::XAxisDirection startXDirection,
          SpriteSheet::YAxisDirection startYDirection)
	: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection),
    _heat(0),
    _alien(alien),
    _tookDamage(false),
    _rollDur(10),
    _idleDur(2),
    _explosionSpawner(spawner)
{
    CreateBehaviourTree();
    _curKinematic.velocity.SetY(341.3f);
    SetHealth(10);
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
    _aabb.UpdatePosition(*this);

    // Detect tile collisions
    _collisionInfo.colIntersect.clear();
    _collisionInfo.rowIntersect.clear();
    DetectTileCollisions(_collisionInfo, _gameScreen->GetLevel());
    if (_collisionInfo.shouldCorrectX)
    {
        float corrected = _collisionInfo.colPenetration * (_collisionInfo.colEdge == Edge::RIGHT ? -1 : 1);
        _curKinematic.position.SetX(_curKinematic.position.GetX() + corrected);
    }
    if (_collisionInfo.shouldCorrectY)
    {
        float corrected = _collisionInfo.rowPenetration * (_collisionInfo.rowEdge == Edge::BOTTOM ? -1 : 1);
        _curKinematic.position.SetY(_curKinematic.position.GetY() + corrected);
    }

    // Detect actor collisions
    for (auto & actor : _gameScreen->GetLevel()->GetActors())
    {
        if (actor.get() == this) continue;
        switch (actor->GetType())
        {
        case Actor::Type::bombenemy:
        {
            shared_ptr<BombAIActor> bomber = dynamic_pointer_cast<BombAIActor>(actor);
            AABB bomberAABB = bomber->GetAABB();

            // The boss sprite's borders are a bit far from its actual dimensions
            // So now we have this
            Vector2 bomberCentre = Vector2(bomberAABB.GetX() + bomberAABB.GetWidth() / 2, bomberAABB.GetY() + bomberAABB.GetHeight() / 2);
            Vector2 bossCentre = Vector2(_aabb.GetX() + _aabb.GetWidth() / 2, _aabb.GetY() + _aabb.GetHeight() / 2);

            float dist = bomberCentre.Distance(bossCentre);

            if (!bomber->IsBlowingUp() && bomber->GetAABB().CheckCollision(_aabb) && dist <= 100)
            {
                // Blow up any stray bombers, but only actually take damage when we're overheating
                if (_currentSpriteSheet == "overheat" && bomber->IsUnderMindControl())
                {
                    bomber->BlowUp();
                    _tookDamage = true;
                    _health -= 10;
                }
                else if (_currentSpriteSheet == "roll")
                {
                    bomber->BlowUp();
                }
            }
        }
        }
    }

    _prevKinematic = _curKinematic;
}

void BossActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);

    if (_currentSpriteSheet != "dead")
    {
        _bossTree.Reset();
        _heat = 0;
        SetHealth(50);
        ResetDurations();
    }
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
    auto checkStage1 = [this](double deltaTime) { return _health > 0 ? Node::Result::Success : Node::Result::Failure; };
    auto checkStage2 = [this](double deltaTime) { return _currentSpriteSheet != "dead" ? Node::Result::Success : Node::Result::Failure; };
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
        bool over = _heat >= 100;
        return !over ? Node::Result::Success : Node::Result::Failure;
    };

    auto passedPlayer = [this](double deltaTime)
    {
        float playerX = _gameScreen->GetPlayer()->GetPosition().GetX();
        float bossX = _curKinematic.position.GetX();

        int curRollDir = playerX < bossX ? -1 : 1;
        bool passed = curRollDir != _rollDir;
        return passed ? Node::Result::Success : Node::Result::Failure;
    };

    auto tookDamage = [this](double deltaTime)
    {
        if (_tookDamage)
        {
            _tookDamage = false;
            return  Node::Result::Success;
        }
        return Node::Result::Failure;
    };
    
    auto preRoll = [this](double deltaTime)
    {
        if (_currentSpriteSheet != "preroll")
        {
            float playerX = _gameScreen->GetPlayer()->GetPosition().GetX();
            float bossX = _curKinematic.position.GetX();

            _rollDir = playerX < bossX ? -1 : 1;
            _spriteXDir = _rollDir == -1 ? SpriteSheet::XAxisDirection::LEFT : SpriteSheet::XAxisDirection::RIGHT;

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

        _curKinematic.velocity.SetX(300.f * (playerX < bossX ? -1 : 1));
        _heat += HEAT_RATE * deltaTime;

        _spriteXDir = _curKinematic.velocity.GetX() < 0 ? SpriteSheet::XAxisDirection::LEFT : SpriteSheet::XAxisDirection::RIGHT;

        if (_currentSpriteSheet != "roll")
        {
            SetSprite("roll");
        }
        return Node::Result::Running;
    };

    auto slow = [this](double deltaTime)
    {
        if (_currentSpriteSheet != "idle")
        {
            SetSprite("idle");
        }

        float deltaV = deltaTime * SLOW_RATE * (-_rollDir);
        float newX = _curKinematic.velocity.GetX() + deltaV;
        _curKinematic.velocity.SetX(newX);

        _heat += HEAT_RATE * deltaTime;

        // Check whether _rollDir and newX have different signs
        if (((int)newX & 0x80000000) != (_rollDir & 0x80000000))
        {
            _curKinematic.velocity.SetX(0);
            return Node::Result::Success;
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
        else if (_sprites[_currentSpriteSheet]->IsFinished())
        {
            _sprites[_currentSpriteSheet]->Reset();
            return Node::Result::Success;
        }
        else if (!_sprites[_currentSpriteSheet]->IsAnimating())
        {
            _sprites[_currentSpriteSheet]->Start();
        }
        return Node::Result::Running;
    };

    auto resetHeat = [this](double deltaTime)
    {
        _heat = 0;
        for (auto & actor : _gameScreen->GetLevel()->GetActors())
        {
            if (actor->GetType() == Type::toggle)
            {
                shared_ptr<ToggleActor> toggle = dynamic_pointer_cast<ToggleActor>(actor);
                toggle->SetOn(false);
            }
        }
        return Node::Result::Success;
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

   // Starts the xplosion sequence
   auto explode = [this](double elapsedSecs)
   {
       Vector2 centre(_aabb.GetX() + _aabb.GetWidth() / 2, _aabb.GetY() + _aabb.GetHeight() / 2);
       _explosionSpawner->SetTarget(centre);
       _explosionSpawner->Start();
       _gameScreen->GetLevel()->AddSpawner(_explosionSpawner);
       return Node::Success;
   };

   // Checks whether an explosion spawned last frame
   auto spawnedExplosion = [this](double deltaTime)
   {
       Node::Result res = _explosionSpawner->GetNumSpawned() == 13 ? Node::Result::Success : Node::Result::Failure;
       return res;
   };

   auto spawnAlien = [this](double deltaTime)
   {
       // Put the alien in at current position
       _alien->SetPosition(Vector2(_curKinematic.position.GetX() - 64, _curKinematic.position.GetY() + 128));
       
       SetSprite("dead");
       // Update health bar?

       // Start firing those dank projectiles
       _gameScreen->GetLevel()->AddActor(_alien);
       SetActive(false);
       return Node::Success;
   };

   // TODO: Draw the entire behaviour tree (here or somewhere within the repo) and point to it
   shared_ptr<Selector> root = shared_ptr<Selector>(new Selector);

   // While health is > 50, we'll go into this sequence (the first "phase" of the battle where the Underwatch
   // only uses rolling and punches)
   shared_ptr<Sequence> firstStageSeq = shared_ptr<Sequence>(new Sequence);
   firstStageSeq->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(checkStage1))));

   // Roll until we either pass the player (slow down and turn around), or overheat (slow down and overheat)
   shared_ptr<Selector> rollActionSelector = shared_ptr<Selector>(new Selector);
   shared_ptr<Sequence> preRollSequence = shared_ptr<Sequence>(new Sequence);
   shared_ptr<Sequence> rollSequence = shared_ptr<Sequence>(new Sequence(false));
   shared_ptr<Selector> continueRollSelector = shared_ptr<Selector>(new Selector);
   shared_ptr<Sequence> passSequence = shared_ptr<Sequence>(new Sequence);
   shared_ptr<Sequence> overheatSequence = shared_ptr<Sequence>(new Sequence);
   shared_ptr<Selector> overheatEndSelector = shared_ptr<Selector>(new Selector);

   preRollSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(shouldPreRoll))));
   preRollSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(preRoll))));

   passSequence->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(passedPlayer))));
   passSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(slow))));

   continueRollSelector->AddChild(passSequence);
   continueRollSelector->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(roll))));

   rollSequence->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(notOverheated))));
   rollSequence->AddChild(continueRollSelector);

   // Play the overheat animation 15 times or get hit by an enemy
   shared_ptr<Node> overheatTask = shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(overheat)));
   overheatEndSelector->AddChild(shared_ptr<Node>(new RunNTimes(overheatTask, 15, true)));
   overheatEndSelector->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(tookDamage))));
   
   // Slow down, get hit or overheat 15 times, reset heat, then chill out
   overheatSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(slow))));
   overheatSequence->AddChild(shared_ptr<Node>(new UntilSuccess(overheatEndSelector)));
   overheatSequence->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(resetHeat))));
   overheatSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(idle))));

   rollActionSelector->AddChild(preRollSequence);
   rollActionSelector->AddChild(rollSequence);
   rollActionSelector->AddChild(overheatSequence);

   firstStageSeq->AddChild(rollActionSelector);

   root->AddChild(firstStageSeq);

   // Blow up and spawn the alien
   shared_ptr<Sequence> ejectSeq = shared_ptr<Sequence>(new Sequence);
   shared_ptr<Node> checkStage2Task = shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(checkStage2)));
   shared_ptr<Node> startExploding = shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(explode)));
   shared_ptr<Node> explodeTask = shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(spawnedExplosion)));
   shared_ptr<Node> untilExplode = shared_ptr<Node>(new UntilSuccess(explodeTask));
   shared_ptr<Node> spawnAlienTask = shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(spawnAlien)));

   ejectSeq->AddChild(checkStage2Task);
   ejectSeq->AddChild(startExploding);
   ejectSeq->AddChild(untilExplode);
   ejectSeq->AddChild(spawnAlienTask);
   
   root->AddChild(ejectSeq);

   _bossTree = BossBehavTree(root, 0.1f);
}