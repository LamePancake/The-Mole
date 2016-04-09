#pragma once
#include "Actor.h"

class ProjectileActor :
	public Actor
{
public:

	/**
	* Constructor that initializes _position to the input parameter.
	*
	* @param	position		starting position of the actor.
	* @param [in,out]	manager A reference to the game manager.
	* @param	spd				The actor's starting velocity.
	* @param	id				The type of object (pancake or flag) that this object represents. Should probably be broken into sub-classes.
	* @param	sprites			A map of names to sprite sheets.
	* @param	startSprite		The name of the sprite to show by default.
	* @param	startXDirection	The direction in the x axis which the actor will face at the start.
	* @param	startYDirection	The direction in the y axis which the actor will face at the start.
	*/
	ProjectileActor(Vector2 position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites, const std::string&& startSprite,
		SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT
		, SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP);

	~ProjectileActor();

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

    virtual ProjectileActor* Clone() override;

    virtual bool IsCloneable() const { return true; }

	virtual Type GetType() const override { return Type::projectile; }

	void UpdatePosition(double elapsedSecs);

	/**
	* Collision check.
	*
	* @param [in,out]	otherAI	The other an i.
	*
	* @return	true if it succeeds, false if it fails.
	*/
	bool CollisionCheck(Actor & otherAI);


private:
	void ProjectileUpdate(double elapseSecs);
};

