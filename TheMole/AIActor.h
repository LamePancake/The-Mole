#ifndef AIACTOR_H
#define AIACTOR_H
#include "Actor.h"
#include "Tile.h"

#pragma once

/**
* This is the simple version of the state machine. It uses a switch instead of states, transitions, conditions, actions.
**/
class AIActor : public Actor
{
public:

	/**
	* @brief	Creates a new AI actor with the given parameters.
	*
	* @author	Shane
	* @date	2/8/2016
	*
	* @param	position	    The AI's start position.
	* @param [in,out]	manager	A reference to the game manager.
	* @param	spd			   	The AI's starting speed.
	* @param [in,out]	sprites	A named list of sprite sheets that can be used to draw the AI.
	* @param	startSprite	   	The default sprite sheet to draw.
	* @param	startXDirection	The actor's default facing direction along the x axis.
	* @param	startYDirection	The actor's default facing direction along the y axis.
	*/
	AIActor(GameManager & manager,
        Vector2 position,
		Vector2 spd,
		std::unordered_map<std::string,
		std::shared_ptr<SpriteSheet>>& sprites,
        const std::string&& startSprite,
        std::unordered_map<std::string, std::pair<std::string, bool>> & sounds,
        std::shared_ptr<SDL2pp::Texture> mindControlIndicator,
        Vector2 spawn,
		SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT, SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP)
		: Actor(position, manager, spd, sprites, std::move(startSprite), sounds, startXDirection, startYDirection),
		_underControl{ false }, _controlTimeLeft{ 0.0f }, _isSelected{ false }, _isCandidate{ false }, _ctrlIndicator{ mindControlIndicator }, _spawn(spawn)
	{
	}

    /**
     * @brief Creates an AIActor from a serialised string.
     * @param serialised The string specifying the actor's properties.
     *
     * Format
     * All of Actor serialisation info (see Actor.h)
     * x y // spawn position, doubles
     * texture_name // Mind control indicator texture name
     */
    AIActor(std::string & serialised);

	/** Destructor. */
	~AIActor();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Sets whether this AI is currently selected for mind control.</summary>
	///
	///<param name="selected"> Whether the AI is selected for mind control.</param>
	///
	///### <author> Shane.</author>
	///### <date> 2/26/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetSelectedForControl(bool selected);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Sets whether this AI a candidate for mind control.</summary>
	///
	///<param name="selected"> Whether the AI is a candidate for mind control.</param>
	///
	///### <author> Shane.</author>
	///### <date> 2/26/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetIsMindControlCandidate(bool isCandidate);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Makes this AI come out of mind control.</summary>
	///
	///### <author> Shane.</author>
	///### <date> 2/26/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void StopMindControl();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Sets the direction that this AI should move while under mind control (and causes
	///          the AI to fall under mind control).</summary>
	///
	///<param name="direction"> The direction it should move.</param>
	///
	///### <author> Shane.</author>
	///### <date> 2/26/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetMindControlDirection(SpriteSheet::XAxisDirection direction);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Queries whether this AI is under mind control.</summary>
	///
	///<returns> Whether this AI is under mind control.</returns>
	///
	///### <author> Shane.</author>
	///### <date> 2/26/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	bool IsUnderMindControl() const;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Queries the direction in which this AI will move while under mind control.</summary>
	///
	///<returns> The direction in which this AI will move while under mind control.</returns>
	///
	///### <author> Shane.</author>
	///### <date> 2/26/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	SpriteSheet::XAxisDirection GetMindControlDirection() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Queries the spawn point of this AI actor.</summary>
    ///
    ///<returns> The spawn point of this AI actor.</returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    Vector2 GetSpawnPoint() const;

	// All the state changing stuff happens in here.
	virtual void Update(double elapsedSecs) override;

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition(double elapsedSecs);

	virtual Type GetType() const override { return Type::enemy; }

	/**
	 * Draws.
	 *
	 * @param [in,out]	camera	ref to the camera.
	 * 
	 * Draws this object.
	 */
	virtual void Draw(Camera& camera) override;

	/**
	* Check collision against another SimpleAgent
	*
	* @return true if collided, false if not
	**/
	bool CollisionCheck(Actor & otherAI);

	/**
	* Scans tiles left and right of the agent and negate _speed if left or right neighbour is not blank tile
	*/
	void ScanNeighbouringTiles(std::shared_ptr<Level> & level);

	void Reset(Vector2 position);

    virtual AIActor* Clone() override;
    virtual bool IsCloneable() const { return true; }

    void CancelMindControl();

private:
	
	void GetPulseColour(const Uint8* startColour, const Uint8* endColour, Uint8* result);

    Vector2 _spawn; // Where this actor spawned

	// Mind control properties
	std::shared_ptr<SDL2pp::Texture> _ctrlIndicator; // A texture drawn to indicate whether this actor is under mind control
	bool _underControl;
	double _controlTimeLeft;	// Time left until mind control ends
	double _pulseTimeTotal;		// Time that should elapse before the next pulse
	double _currentPulseTime;	// Time that has elapsed during this pulse

	// Mind control selection properties (when the player is selecting whom to control)
	bool _isSelected;
	bool _isCandidate;
	SpriteSheet::XAxisDirection _controlDir;
};

#endif
