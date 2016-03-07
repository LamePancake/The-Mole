#pragma once
#include "Actor.h"

class TurretActor :
	public Actor
{
public:
	TurretActor(
		Vector2 position
		, GameManager & manager
		, Vector2 spd
		, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites
		, const std::string&& startSprite
		,SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT
		, SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP);

	~TurretActor();

	/**
	* Draws.
	*
	* @param [in,out]	level	The level.
	*
	* 	Draws this object.
	*/
	virtual void Draw(Camera& camera);

	// All the state changing stuff happens in here.
	virtual void Update(double elapsedSecs) override;

	// Resets the actor
	virtual void Reset(Vector2 pos);

	/**
	* Collision check.
	*
	* @param [in,out]	otherAI	The other an i.
	*
	* @return	true if it succeeds, false if it fails.
	*/
	bool CollisionCheck(Actor & otherAI);

	void SetPattern(Vector2 prjDir);

	void SetPattern(std::vector<Vector2> prjDirSet);
private:
	void TurretUpdate(double elapseSecs);
	/** Stores projectiles currently spawned */
	std::vector<std::shared_ptr<ProjectileActor>> _projectiles;
	///Calculates the time between shots
	double _timeInterval; //Don't touch
	///This store the direction(s) of the projectile(s) fired by this turret
	std::vector<Vector2> _pattern;
	virtual Type GetType() const override { return Type::turret; }

private:
	void TurretUpdate(double elapseSecs);
	/** Stores projectiles currently spawned */
	//std::vector<std::shared_ptr<ProjectileActor>> _projectiles;
	double _timeInterval;
};
