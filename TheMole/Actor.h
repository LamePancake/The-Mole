#ifndef ACTOR_H
#define ACTOR_H

#include <SDL2pp\SDL2pp.hh>
#include <string>
#include <memory>
#include <unordered_map>
#include "Vector2.h"
#include "AABB.h"
#include "GameManager.h"
#include "Level.h"
#include "Camera.h"
#include "SpriteSheet.h"


class GameScreen;

class Actor
{
public:

   /**
	* Contains the actor's current kinematic properties (position, velocity, rotation and angular speed in radians).
	*/
	struct KinematicState
	{
		KinematicState()
			: position(0, 0), velocity(0, 0), rotation(0), angular(0) {}

		KinematicState(Vector2 startPos, Vector2 startVel, float startRot = 0.f, float startAng = 0.f)
			: position(startPos), velocity(startVel), rotation(startRot), angular(startAng) {}

        Vector2 position;
		Vector2 velocity;

		float rotation;
		float angular;
	};

	/**
	 * Creates a new Actor, which has a position and the potential to be drawn in the game world.
	 *
	 * @param	position		starting position of the actor.
	 * @param [in,out]	manager A reference to the game manager.
	 * @param	spd				The actor's starting velocity.
	 * @param	sprites			A map of names to sprite sheets.
	 * @param	startSprite		The name of the sprite to show by default.
	 * @param	startXDirection	The direction in the x axis which the actor will face at the start.
	 * @param	startYDirection	The direction in the y axis which the actor will face at the start.
	 */
	Actor(Vector2 position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites, const std::string&& startSprite,
			SpriteSheet::XAxisDirection startXDirection, SpriteSheet::YAxisDirection startYDirection);

	/** Destructor. */
	~Actor();

   /**
	* Return AABB of the agent
	*
	* @return _aabb
	*/
	AABB GetAABB();

	/**
	 * Returns in the textures needed to render the AI.
	 *
	 * @return	null if it fails, else the texture.
	 */
	std::shared_ptr<SpriteSheet> GetTexture();

	/**
	* Returns in the textures needed to render the AI shadow.
	*
	* @return	null if it fails, else the texture.
	*/
	std::shared_ptr<SpriteSheet> GetTextureShadow();

	// Returns position of the agent.
    Vector2 GetPosition();

	/**
	 * Gets the speed.
	 *
	 * @return	The speed.
	 */
	Vector2 GetSpeed();

	/**
	 * Gets the health.
	 *
	 * @return	The health.
	 */
	size_t GetHealth();

	/**
	 * Sets a speed.
	 *
	 * @param	spd	The speed.
	 */
	void SetSpeed(Vector2 spd);

	/**
	 * Sets a health.
	 *
	 * @param	heath	The health.
	 */
	void SetHealth(size_t health);

	/**
	 * Sets a position.
	 *
	 * @param	pos	The position.
	 */
	void SetPosition(Vector2 pos);

	/**
	 * Gets actor direction.
	 *
	 * @return	The actor direction.
	 */
	SpriteSheet::XAxisDirection GetActorXDirection() const;

	/**
	 * @brief	Gets actor y coordinate direction.
	 *
	 * @author	Shane
	 * @date	2/7/2016
	 *
	 * @return	The actor y coordinate direction.
	 */
	SpriteSheet::YAxisDirection GetActorYDirection() const;

	/**
	 * Sets actor direction.
	 *
	 * @param	dir	The direction.
	 */
	void SetActorXDirection(SpriteSheet::XAxisDirection dir);

	/**
	* Sets actor direction.
	*
	* @param	dir	The direction.
	*/
	void SetActorYDirection(SpriteSheet::YAxisDirection dir);

	/**
	 * @brief	Sets whether this actor is visible.
	 *
	 * @author	Shane
	 * @date	2/8/2016
	 *
	 * @param	isVisible	true if this object is visible.
	 */
	void SetVisibility(bool isVisible);

	/**
	 * @brief	Query if this object is visible.
	 *
	 * @author	Shane
	 * @date	2/8/2016
	 *
	 * @return	true if visible, false if not.
	 */
	bool IsVisible() const;

	// All the state changing stuff happens in here. 
	virtual void Update(double elapsedSecs);

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition(double elapsedSecs);

	/**
	 * Draws.
	 *
	 * @param [in,out]	camera	ref to the camera.
	 * 
	 * Draws this object.
	 */
	virtual void Draw(Camera& camera);

	/**
	* Resets the actor.
	*
	* @param [in,out]	Position to reset at.
	*
	*/
	virtual void Reset(Vector2 pos);

protected:

	typedef enum {
		TOP,
		BOTTOM,
		RIGHT,
		LEFT,
		NONE
	} Edge;

	// TEMPORARY struct to hold actor bounds while I figure out something more elegant (it's probably going to stay forever though)
	struct Bounds {
		double rightBound, leftBound, topBound, bottomBound;
		int rightCol, leftCol, topRow, bottomRow;
	};

    struct TileCollisionInfo {
        /**
         * Whether the row of tiles contained in rowIntersect is above(Edge::TOP) or below(Edge::BOTTOM) this actor.
         * Edge::NONE means that the actor hasn't moved in this axis.
         */
        Edge rowEdge;
        /**
         * Whether the column of tiles contained in colIntersect is to the right (Edge::RIGHT) or left (Edge::LEFT) of this actor.
         * Edge::NONE means that the actor hasn't moved in this axis.
         */
        Edge colEdge;

        /** The list of tiles parallel to the x-axis with which one of this actor's edges (specified in rowEdge) is colliding. */
        std::vector<std::shared_ptr<Tile>> rowIntersect;
        /** The list of tiles parallel to the y-axis with which one of this actor's edges (specified in colEdge) is colliding. */
        std::vector<std::shared_ptr<Tile>> colIntersect;

        /** The number of pixels by which this actor has penetrated the row of tiles the tiles in rowIntersect. */
        int rowPenetration;
        /** The number of pixels by which this actor has penetrated the column of tiles in colIntersect. */
        int colPenetration;
    };

	/** The health. */
	size_t _health;

	/** The actor's current kinematic state. */
	KinematicState _curKinematic;

	/** The actor's kinematic state in the last update.*/
	KinematicState _prevKinematic;

    /** Contains the tiles with which the actor intersected.*/
    TileCollisionInfo _collisionInfo;

	/** The aabb. */
	AABB _aabb;

	/** @brief	The name of the current spritesheet and its associated sprite sheet shadow. */
	std::string _currentSpriteSheet;
	
	/** The sprite. */
	std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> _sprites;

	/** The game manager which drives the game loop. */
	GameManager* _mgr;
	
	/** @brief	A pointer to the current game screen. */
	std::shared_ptr<GameScreen> _gameScreen;

	/** @brief	The direction that the sprite is facing along the x axis. */
	SpriteSheet::XAxisDirection _spriteXDir;

	/** @brief	The direction that the sprite is facing along the y axis. */
	SpriteSheet::YAxisDirection _spriteYDir;

	/** @brief	true if this object is visible. */
	bool _isVisible;

	/**
	 * @brief	Determines the tiles with which this actor is colliding.
	 *
	 * @author	Shane
	 * @date	2/5/2016
	 *
	 * @param [in,out]	level		  	The level containing the actor and the tiles.
	 */
	void DetectTileCollisions(TileCollisionInfo& colInfo, std::shared_ptr<Level>& level);

	/**
	 * @brief	Determines the actor's bounding box and which tiles they intersect at the given kinematic state.
	 *
	 * @author	Shane
	 * @date	2/5/2016
	 *
 	 * @param state				A KinematicState object specifying the state of the actor for which to calculate the bounds.
	 * @param [in,out]	bounds	A Bounds struct to hold calculated bounds.
	 */
	void GetBounds(const KinematicState & state, Bounds & bounds);
};

#endif