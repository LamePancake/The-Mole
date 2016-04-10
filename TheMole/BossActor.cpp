#include <iostream>
#include "GameScreen.h"
#include "ProjectileActor.h"
#include "BossActor.h"

#define HEAT_RATE 10
#define SLOW_RATE 200.f

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
    _curKinematic.velocity.SetY(341.3f);
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

    _prevKinematic = _curKinematic;
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
            float playerX = _gameScreen->GetPlayer()->GetPosition().GetX();
            float bossX = _curKinematic.position.GetX();

            _rollDir = playerX < bossX ? -1 : 1;

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
        if (_currentSpriteSheet != "roll")
        {
            SetSprite("roll");
        }
        return Node::Result::Running;
    };

    auto passedPlayer = [this](double deltaTime)
    {
        float playerX = _gameScreen->GetPlayer()->GetPosition().GetX();
        float bossX = _curKinematic.position.GetX();

        int curRollDir = playerX < bossX ? -1 : 1;
        return curRollDir != _rollDir ? Node::Result::Success : Node::Result::Failure;
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

   // Roll until we either pass the player (slow down and turn around), or overheat (slow down and overheat)
   shared_ptr<Selector> rollActionSelector = shared_ptr<Selector>(new Selector);
   shared_ptr<Sequence> preRollSequence = shared_ptr<Sequence>(new Sequence);
   shared_ptr<Sequence> rollSequence = shared_ptr<Sequence>(new Sequence);
   shared_ptr<Selector> continueRollSelector = shared_ptr<Selector>(new Selector);
   shared_ptr<Sequence> passSequence = shared_ptr<Sequence>(new Sequence);
   shared_ptr<Sequence> overheatSequence = shared_ptr<Sequence>(new Sequence);

   preRollSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(shouldPreRoll))));
   preRollSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(preRoll))));

   passSequence->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(passedPlayer))));
   passSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(slow))));

   continueRollSelector->AddChild(passSequence);
   continueRollSelector->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(roll))));

   rollSequence->AddChild(shared_ptr<Node>(new Task(true, std::function<Node::Result(double)>(notOverheated))));
   rollSequence->AddChild(continueRollSelector);

   overheatSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(overheat))));
   overheatSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(slow))));
   overheatSequence->AddChild(shared_ptr<Node>(new Task(false, std::function<Node::Result(double)>(idle))));

   rollActionSelector->AddChild(preRollSequence);
   rollActionSelector->AddChild(rollSequence);
   rollActionSelector->AddChild(overheatSequence);

   firstStageSeq->AddChild(rollActionSelector);

   root->AddChild(firstStageSeq);

   _bossTree = BossBehavTree(root, 0.1f);
}