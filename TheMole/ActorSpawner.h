#pragma once
#include <memory>
#include "Actor.h"
#include "Level.h"

class ActorSpawner
{
public:
    ActorSpawner(std::shared_ptr<Level> level, std::shared_ptr<Actor> prototype, double period, int limit)
        : _level(level), _prototype(prototype), _period(period), _limit(limit), _spawned(), _timeSinceSpawn(0),
          _isRunning(true), _spawnedLastFrame(false)
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
    virtual void Update(double deltaTime);

    // Starts spawning stuff
    void Start();

    // Stops spwaning stuff
    void Stop();

    // Determines whether we're spawning stuff
    bool IsRunning();

    // Determines whether we spawned an actor last frame
    bool SpawnedLastUpdate();

    // Gets the last actor that was spawned
    // If limit is -1, this only stores the most recently spawned actor (which may be an expired reference)
    std::vector<std::weak_ptr<Actor>> GetSpawned();

    // Sets the amount of time between spawns
    void SetPeriod(double newPeriod);
    
    // Gets the amount of time between spawns
    double GetPeriod() const;

    // Gets the number of actors that can be spawned at a time
    int GetSpawnLimit() const;

    // Sets the number of actors that can be spawned at the same time (-1 is infinite)
    void SetSpawnLimit(int limit);

    // Gets the number of actors that are currently spawned
    int GetNumSpawned() const;
private:
    std::shared_ptr<Level> _level;

    // The actor from which to instantiate other actors
    std::shared_ptr<Actor> _prototype;

    // Weak reference to the last actor to have been spawned (only used when !_multiple)
    std::vector<std::weak_ptr<Actor>> _spawned;
    double _timeSinceSpawn;

    // Minimum time between spawning actors
    double _period;

    // The number of actors that may be spawned at once (-1 is infinite)
    int _limit;

    bool _isRunning;

    bool _spawnedLastFrame;
};