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
    if (!_isRunning) return;
    _spawnedLastFrame = false;

    _timeSinceSpawn += deltaTime;
    if (_timeSinceSpawn > _period)
    {
        if (_limit == -1)
        {
            _spawned[0] = _level->AddActorCopy(_prototype);
            _spawnedLastFrame = true;
        }
        else if (_spawned.size() < _limit)
        {
            _spawned.push_back(_level->AddActorCopy(_prototype));
            _timeSinceSpawn = 0;
            _spawnedLastFrame = true;
        }
        else
        {
            // Erase all of the expired pointers and try again
            auto expiredFinder = [](std::weak_ptr<Actor> & ptr) {return ptr.expired(); };
            _spawned.erase(std::remove_if(_spawned.begin(), _spawned.end(), expiredFinder), _spawned.end());
            if (_spawned.size() < _limit)
            {
                _spawned.push_back(_level->AddActorCopy(_prototype));
                _timeSinceSpawn = 0;
                _spawnedLastFrame = true;
            }
        }
    }
}

void ActorSpawner::Start()
{
    _isRunning = true;
}

void ActorSpawner::Stop()
{
    _isRunning = false;
}

bool ActorSpawner::IsRunning()
{
    return _isRunning;
}

bool ActorSpawner::SpawnedLastUpdate()
{
    return _spawnedLastFrame;
}

std::vector<std::weak_ptr<Actor>> ActorSpawner::GetSpawned()
{
    return _spawned;
}

void ActorSpawner::SetPeriod(double newPeriod)
{
    _period = newPeriod;
}

double ActorSpawner::GetPeriod() const
{
    return _period;
}

int ActorSpawner::GetSpawnLimit() const
{
    return _limit;
}

void ActorSpawner::SetSpawnLimit(int limit)
{
    _limit = limit;
}

int ActorSpawner::GetNumSpawned() const
{
    return _spawned.size();
}
