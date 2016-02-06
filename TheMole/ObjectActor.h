#pragma once
#include "Actor.h"
class PlayerActor;
class GameScreen;

class ObjectActor : public Actor
{
public:

	enum ObjectType
	{
		flag,
		pancake,
	};

	/**
	 * Constructor that initializes _position to the input parameter.
	 *
	 * @param	position	   	starting position of the agent.
	 * @param [in,out]	manager	game manager.
	 * @param	spd			   	speed.
	 * @param	texturePath	   	Full pathname of the texture file.
	 * @param	id			   	The identifier.
	 * @param	framesPerSecond	The frames per second.
	 */
	ObjectActor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath, int id, int framesPerSecond)
		: Actor(position, manager, spd, texturePath, framesPerSecond), _id(id), _isVisible(true) {
		_sprite->Stop();
		_spriteShadow->Stop();
	}

	/** Destructor. */
	~ObjectActor();

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
	
	/**
	* Collision check.
	*
	* @param [in,out]	otherAI	The other an i.
	*
	* @return	true if it succeeds, false if it fails.
	*/
	bool CollisionCheck(Actor & otherAI);

	/**
	 * Gets the identifier.
	 *
	 * @return	The identifier.
	 */
	int GetID();

	/**
	 * Sets an identifier.
	 *
	 * @param	id	The identifier.
	 */
	void SetID(int id);

	bool IsVisisble();

private:

	int _id;

	bool _isVisible;

	void FlagUpdate(double elapsedSecs);

	void PancakeUpdate(double elapsedSecs);
};

