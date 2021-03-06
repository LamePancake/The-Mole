#pragma once
#include "Actor.h"

class NPCActor : public Actor
{
public:

	/**
	* @brief	Creates a new NPC actor with the given parameters.
	*
	* @author	Shane
	* @date	2/8/2016
	*
	* @param	position	    The NPC's start position.
	* @param [in,out]	manager	A reference to the game manager.
	* @param	spd			   	The NPC's starting speed.
	* @param	sprites			The list of sprite sheets available for drawing the NPC.
	* @param	startSprite	   	The default sprite sheet to draw.
	* @param	startXDirection	The actor's default facing direction along the x axis.
	* @param	startYDirection	The actor's default facing direction along the y axis.
	*/
	NPCActor(Vector2 position,
             GameManager & manager,
             Vector2 spd,
             std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
             const std::string&& startSprite,
             std::unordered_map<std::string, std::pair<std::string, bool>> & sounds,
		     SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT, 
             SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP)
		: Actor(position, manager, spd, sprites, std::move(startSprite), sounds, startXDirection, startYDirection) {}

    NPCActor(std::string serialised)
        : Actor(serialised) {}

	/** Destructor. */
	~NPCActor();

	/**
	* Draws.
	*
	* @param [in,out]	level	The level.
	*
	* 	Draws this object.
	*/
	virtual void Draw(Camera& camera) override;

	// All the state changing stuff happens in here.
	virtual void Update(double elapsedSecs) override;

    virtual void UpdatePosition(double elapsedSecs) override;

	// Resets the actor
	virtual void Reset(Vector2 pos);

    virtual NPCActor* Clone() override;

    virtual bool IsCloneable() const { return true; }

	virtual Type GetType() const override { return Type::npc; }

private:
	//Will probably store dialog here.
};