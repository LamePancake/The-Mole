#pragma once
#include "ActorSpawner.h"

class TargetActorSpawner : public ActorSpawner
{
public:
    TargetActorSpawner(std::shared_ptr<Level> level,
                       std::shared_ptr<Actor> prototype,
                       double period,
                       bool allowMultiple,
                       Vector2 target,
                       double radius);

    virtual void Update(double deltaTime);

    double GetRadius() const;
    void SetRadius(double newRadius);

    Vector2 GetTarget() const;
    void SetTarget(Vector2 target);

private:
    Vector2 _target;
    double _radius;
};