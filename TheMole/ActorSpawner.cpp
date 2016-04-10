#include "ActorSpawner.h"

std::shared_ptr<Actor> ActorSpawner::GetPrototype()
{
    return _prototype;
}

void ActorSpawner::SetPrototype(std::shared_ptr<Actor> prototype)
{
    _prototype = prototype;
    _prototype->SetDestroysOnInactive(true);
}

void ActorSpawner::Update(double deltaTime)
{
    // Spawners that don't allow multiple should only start counting to the next spawn
    // once their current one has been destroyed
    if (!_multiple && _lastSpawned.expired())
    {
        _timeSinceSpawn += deltaTime;
    }
    if (_timeSinceSpawn > _period)
    {
        _lastSpawned = _level->AddActorCopy(_prototype);
        _timeSinceSpawn = 0;
    }
}
