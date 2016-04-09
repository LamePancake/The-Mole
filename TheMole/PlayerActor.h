#pragma once
#include "Actor.h"
#include "Math.h"

class PlayerActor : public Actor
{
public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Creates a new player actor with the given parameters.</summary>
    ///
    ///<param name="position">        The player's start position.</param>
    ///<param name="manager">         [in,out] A reference to the game manager.</param>
    ///<param name="spd">             The player's starting speed.</param>
    ///<param name="sprites">         [in,out] The list of sprite sheets for the player.</param>
    ///<param name="startSprite">     The default sprite to draw.</param>
    ///<param name="startXDirection"> The actor's default facing direction along the x axis.</param>
    ///<param name="startYDirection"> The actor's default facing direction along the y axis.</param>
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	PlayerActor(Vector2 position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites, const std::string&& startSprite,
		SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT, SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP);

	/** Destructor. */
	~PlayerActor();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Draws this object with the given camera.</summary>
    ///
    ///<param name="camera"> [in,out] The camera to use for drawing.</param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void Draw(Camera& camera) override;

	// All the state changing stuff happens in here.
	virtual void Update(double elapsedSecs) override;

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition(double elapsedSecs);

	// Gets whether the player stopped time
	bool StoppedTime() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Collision check.</summary>
    ///
    ///<remarks> Shane, 2/22/2016.</remarks>
    ///
    ///<param name="otherAI"> [in,out] The other actor to check against.</param>
    ///
    ///<returns> true if the actors are colliding.</returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	bool CollisionCheck(Actor &otherAI);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Query if this object is dead.</summary>
    ///
    ///<returns> true if dead, false if not.</returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	bool IsDead();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Determines if the player is at the goal.</summary>
    ///
    ///<returns> Whether the player is at the goal.</returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	bool AtGoal();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Determines if the player triggered intro cutscene.</summary>
	///
	///<returns> Whether the player triggered intro cutscene.</returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	bool TriggeredIntro();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Sets jump velocity.</summary>
    ///
    ///<param name="initVel"> velocity in the Y axis.</param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetJumpVelocity(float initVel);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Gets player's jump velocity.</summary>
    ///
    ///<returns> float that is the velocity of the jump in the Y axis.</returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

	float GetJumpVelocity();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Sets maximum jump velocity.</summary>
    ///
    ///<param name="initVel"> initial velocity in the Y axis.</param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetMaximumJumpVelocity(float initVel);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Gets maximum jump velocity.</summary>
    ///
    ///<returns> float that is the velocity of the jump in the Y axis.</returns>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	float GetMaximumJumpVelocity();

	///Used for projectiles
	void ProjectileHit(Actor *prj);

	// Resets the player
	virtual void Reset(Vector2 pos);

    virtual Actor* Clone();

    virtual bool IsCloneable() const { return false; }

	virtual Type GetType() const override { return Type::player; }

private:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Updates all player properties influenced by the user's input.</summary>
    ///
    ///<remarks> Shane, 2/22/2016.</remarks>
    ///
    ///### <author> Shane.</author>
    ///### <date> 2/6/2016.</date>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	void UpdateInput(double elapsedSecs);

	void UpdateShieldStatus(double deltaTime);

	///Decrements shield
	void ShieldHit();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Updates controllable AI selection.</summary>
	///
	///<param name="released"> Whether we just released the mind control key.</param>
	///
	///### <author> Shane.</author>
	///### <date> 2/26/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void UpdateMindControlSelection(bool released);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Digs applicable tiles (if any) in the current dig direction.</summary>
    /// 
    ///<remarks> Shane, 2/22/2016.</remarks>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void DigDiggableTiles();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Updates all collisions (with tiles and enemies).</summary>
    ///
    ///<param name="elapsedSecs"> The elapsed time in seconds.</param>
    ///
    ///### <author> Shane</author>
    ///### <date> 2/6/2016</date>
    ///           
    ///<editor>Tim (added parameter)</editor>
    ///<date>2/6/2016.</date>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	void UpdateCollisions(double elapsedSecs);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Stops any jump in progress.</summary>
    ///
    ///<remarks> Shane, 2/22/2016.</remarks>
    ///
    ///### <author> Shane.</author>
    ///### <date> 2/8/2016.</date>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	void StopJumping();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Handles collisions with tiles.</summary>
    ///
    ///<remarks> Shane, 2/22/2016.</remarks>
    ///
    ///<param name="tiles">        [in,out] The tiles.</param>
    ///<param name="edge">         The edge.</param>
    ///<param name="correctedPos"> The corrected position.</param>
    ///
    ///### <author> Shane.</author>
    ///### <date> 2/7/2016.</date>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	void DefaultTileCollisionHandler(std::vector<std::shared_ptr<Tile>>& tiles, Edge edge, float correctedPos);

	bool _atGoal;
	bool _triggeredIntro;
	float _jumpVelocity;
	float _maxJumpVel;
	float _jumpBoost;
	bool _jumped;
	bool _gliding;
	bool _jumpBoosted;
	double _jumpDuration;
	double _jumpTimeElapsed;
	bool _wasOnGround; // Tracks whether the player was on the ground in the previous frame
	SpriteSheet::XAxisDirection _prevDirection;

	///Shield stuff
	bool _shieldActive;
	bool _shieldReleased;
	bool _shieldHit;
	int _shieldStr;
	double _shieldTimer;
	Actor *_lastPrj;

    Edge _digDir;
	bool _godMode;

	// Mind control variables
	bool _stoppedTime;
	const float _mindControlRadius = 384.f;
	int _selected;
};