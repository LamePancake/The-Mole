#include <random>
#include "TargetedActorSpawner.h"

TargetedActorSpawner::TargetedActorSpawner(std::shared_ptr<Level> level,
                                       std::shared_ptr<Actor> prototype,
                                       double period,
                                       int limit,
                                       Vector2 target,
                                       double radius)
    : ActorSpawner(level, prototype, period, limit),
    _target(target), _radius(radius)
{

}

void TargetedActorSpawner::Update(double deltaTime)
{
    ActorSpawner::Update(deltaTime);

    // Grab the most recently spawned thing (if it's spawned last frame) and put it in the correct area
    // Some code stolen from Starscape
    if (SpawnedLastUpdate())
    {
        auto last = GetSpawned().back().lock();
        AABB aabb = last->GetAABB();

        // Stolen from the starscape shopgap :)
        // Determine the direction (don't change Z since we want it to spawn at least above the player)
        float xMul = (rand() > RAND_MAX / 2) ? 1 : -1;
        float yMul = (rand() > RAND_MAX / 2) ? 1 : -1;

        // Get the offset from the target
        float xRad = ((RAND_MAX - rand()) / (float)RAND_MAX * _radius) * xMul;
        float yRad = ((RAND_MAX - rand()) / (float)RAND_MAX * _radius) * yMul;

        // Determine random starting position within the specified space for the new shimmer
        float x = xRad - (aabb.GetWidth() / 2) + _target.GetX();
        float y = yRad - (aabb.GetHeight() / 2) +_target.GetY();

        Vector2 spawnLocation(x, y);
        last->SetPosition(spawnLocation);
    }
}

double TargetedActorSpawner::GetRadius() const
{
    return _radius;
}

void TargetedActorSpawner::SetRadius(double newRadius)
{
    _radius = newRadius;
}

Vector2 TargetedActorSpawner::GetTarget() const
{
    return _target;
}

void TargetedActorSpawner::SetTarget(Vector2 target)
{
    _target = target;
}
