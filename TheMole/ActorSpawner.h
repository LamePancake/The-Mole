#pragma once
#include <memory>
#include "Actor.h"
#include "Level.h"

class ActorSpawner
{
public:
    ActorSpawner(std::shared_ptr<Level> level, std::shared_ptr<Actor> prototype, double period, bool allowMultiple)
        : _level(level), _prototype(prototype), _period(period), _multiple(allowMultiple), _lastSpawned(), _timeSinceSpawn(0)
    {
        _prototype->SetDestroysOnInactive(true);
    }

    /**
     * @brief Gets the prototype actor used to spawn new actors.
     * This can be useful if you need to change some property of the prototype (e.g. position) without replacing it entirely.
     *
     * @return The prtotype actor used to spawn new actors.
     */
    std::shared_ptr<Actor> GetPrototype();

    /**
     * @brief Sets a new prototype for spawning actors.
     *
     * @param prototype The new prototype to use. Shouldn't be empty.
     */
    void SetPrototype(std::shared_ptr<Actor> prototype);

    /**
     * @brief Updates the actor spawner, spawning new actors as appropriate.
     *
     */
    void Update(double deltaTime);

    std::weak_ptr<Actor> GetLastSpawned();

    void SetPeriod(double newPeriod);

    double GetPeriod() const;

    bool CanSpawnMultiple() const;

    void SetCanSpawnMultiple(bool allowMultiple);

private:
    std::shared_ptr<Level> _level;

    // The actor from which to instantiate other actors
    std::shared_ptr<Actor> _prototype;

    // Weak reference to the last actor to have been spawned (only used when !_multiple)
    std::weak_ptr<Actor> _lastSpawned;
    double _timeSinceSpawn;

    // Minimum time between spawning actors
    double _period;

    // Whether to allow multiple actors to spawn at the same time
    bool _multiple;
};