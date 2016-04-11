#include <random>
#include "TargetActorSpawner.h"

TargetActorSpawner::TargetActorSpawner(std::shared_ptr<Level> level,
                                       std::shared_ptr<Actor> prototype,
                                       double period,
                                       int limit,
                                       Vector2 target,
                                       double radius)
    : ActorSpawner(level, prototype, period, limit),
    _target(target), _radius(radius)
{

}

void TargetActorSpawner::Update(double deltaTime)
{
    ActorSpawner::Update(deltaTime);
    if (SpawnedLastUpdate())
    {
        auto last = GetSpawned().back().lock();
        AABB aabb = last->GetAABB();
        Vector2 centre = Vector2(aabb.GetX() + aabb.GetWidth() / 2, aabb.GetY() + aabb.GetHeight() / 2);

        // Stolen from the starscape shopgap :)
        // Determine random starting position within the specified space for the new shimmer
        float x = _radius + ((RAND_MAX - rand()) / (float)RAND_MAX * _radius);
        float y = _radius + ((RAND_MAX - rand()) / (float)RAND_MAX * _radius);

        // Determine the direction (don't change Z since we want it to spawn at least above the player)
        x *= (rand() > RAND_MAX / 2) ? 1 : -1;
        y *= (rand() > RAND_MAX / 2) ? 1 : -1;

        Vector2 spawnLocation(x + centre.GetX(), y + centre.GetY());
        last->SetPosition(spawnLocation);
    }
}

double TargetActorSpawner::GetRadius() const
{
    return _radius;
}

void TargetActorSpawner::SetRadius(double newRadius)
{
    _radius = newRadius;
}

Vector2 TargetActorSpawner::GetTarget() const
{
    return _target;
}

void TargetActorSpawner::SetTarget(Vector2 target)
{
    _target = target;
}
