#pragma once

#include "Actor.h"
#include "BossBehavTree.h"
#include "TargetedActorSpawner.h"

class BossActor : public Actor
{
public:
	BossActor(Vector2 position, GameManager & manager,
              Vector2 spd,
              std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
              const std::string&& startSprite,
              std::unordered_map<std::string, std::pair<std::string, bool>> & sounds,
              std::shared_ptr<Actor> alien,
              std::shared_ptr<TargetedActorSpawner> spawner,
		      SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT,
              SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP);

	~BossActor();

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition(double elapsedSecs);

	/**
	* Draws.
	*
	* @param [in,out]	camera	ref to the camera.
	*
	* Draws this object.
	*/
	virtual void Draw(Camera& camera) override;

	virtual void Update(double elapsedSecs);

	// Resets the actor
	virtual void Reset(Vector2 pos);

    virtual BossActor* Clone();
    virtual bool IsCloneable() const { return false; }

	virtual Type GetType() const override { return Type::boss; }

	void ResetDurations();

	double _idleDur;
	double _preRollDur;
	double _rollDur;
	double _prePunchDur;
	double _punchDur;
	double _hopDur;
	double _shockWaveDur;

private:
	BossBehavTree _bossTree;
	float _heat;
    std::shared_ptr<Actor> _alien;
    std::shared_ptr<TargetedActorSpawner> _explosionSpawner;

	Vector2 _spawnPos;
    bool _isFirstUpdate;

    // The direction in which the boss was rolling on pre-roll
    int _rollDir;

    // Whether we just took damage
    bool _tookDamage;

    void CreateBehaviourTree();
};
 