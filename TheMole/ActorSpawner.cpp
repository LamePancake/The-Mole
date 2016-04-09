#include "ActorSpawner.h"

std::shared_ptr<Actor> ActorSpawner::GetPrototype()
{
    return _prototype;
}

void ActorSpawner::SetPrototype(std::shared_ptr<Actor> prototype)
{
    _prototype = prototype;
    _prototype->SetDestroyOnReset(true);
}

void ActorSpawner::Update(double deltaTime)
{
    _timeSinceSpawn += deltaTime;
    if (_timeSinceSpawn < _period || (!_multiple && !_lastSpawned.expired())) return;

    _lastSpawned = _level->AddActorCopy(_prototype);
    _timeSinceSpawn = 0;
}
