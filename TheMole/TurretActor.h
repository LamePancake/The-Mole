#pragma once
#include "Actor.h"

class ProjectileActor;

class TurretActor :
	public Actor
{
public:
    enum Aim
    {
        None,
        XAxis,
        Player
    };

	TurretActor(
		Vector2 position
		, GameManager & manager
		, Vector2 spd
		, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites
		, const std::string&& startSprite
        , std::unordered_map<std::string, std::pair<std::string, bool>> &sounds
		, SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT
		, SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP);

	TurretActor(
		Vector2 position
		, GameManager & manager
		, Vector2 spd
		, Aim aim
        , bool reflectable
        , double period
		, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites
		, const std::string&& startSprite
        , std::unordered_map<std::string, std::pair<std::string, bool>> & sounds
		, SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT
		, SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP
		);

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

    virtual TurretActor* Clone() override;

    virtual bool IsCloneable() const { return true; }

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

	void ChangeAim(Aim newAim);

	Vector2 GetProjectileVel();
private:
	void TurretUpdate(double elapseSecs);
	/** Stores projectiles currently spawned */
	std::vector<std::shared_ptr<ProjectileActor>> _projectiles;
	///Calculates the time between shots
	double _timeInterval; //Don't touch

	Aim _aim;
    bool _reflectable; // Whether projectiles from this turret are reflectable
    double _period; // Time (seconds) between shots

	///This store the direction(s) of the projectile(s) fired by this turret
	std::vector<Vector2> _pattern;
	virtual Type GetType() const override { return Type::turret; }

};
