#ifndef ACTOR_H
#define ACTOR_H

#include <SDL2pp\SDL2pp.hh>
#include <string>
#include <memory>
#include <unordered_map>
#include "Vector2.h"
#include "AABB.h"
#include "GameManager.h"
#include "Camera.h"
#include "SpriteSheet.h"
#include "Tile.h"
#include "Util.h"

class GameScreen;
class Level;

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

	enum Edge {
		TOP,
		BOTTOM,
		RIGHT,
		LEFT,
		NONE
	};

	enum Type {
		npc,
		player,
		boss,
		door,
		object,
		projectile,
		turret,
		toggle,
        bombenemy,
		enemy
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
			SpriteSheet::XAxisDirection startXDirection, SpriteSheet::YAxisDirection startYDirection, bool active = true);

    /**
     * @brief Constructs an actor from a serialised string containing all of the actor's construction parameters (one per line, excluding sprites).
     * @param serialsied The string containing construction parameters.
     *
     * Format:
     * x y // position, doubles
     * x y // speed, doubles
     * sprite_name (string) texture_file (string) num_frames (int) duration (double) LEFT or RIGHT (XAXisDirection) UP or DOWN (YAxisDirection) // Sprite sheets (1 per line)
     * start_sprite // string
     * LEFT or RIGHT // start XAxisDirection
     * UP or DOWN // start YAxisDirection
     * 0 or 1  // isActive
     */
    Actor(std::string & serialised);

    Actor(Actor& other)
        : _health(other._health),
        _curKinematic(other._curKinematic),
        _prevKinematic(other._prevKinematic),
        _collisionInfo(other._collisionInfo),
        _aabb(other._aabb),
        _currentSpriteSheet(other._currentSpriteSheet),
        _mgr(other._mgr),
        _gameScreen(other._gameScreen),
        _spriteXDir(other._spriteXDir),
        _spriteYDir(other._spriteYDir),
        _startXDir(other._startXDir),
        _startYDir(other._startYDir),
        _isVisible(other._isVisible),
        _isDestroyed(other._isDestroyed),
        _destroyOnReset(other._destroyOnReset),
        _isActive(other._isActive)
    {
        for (auto & sheet : other._sprites)
        {
            std::shared_ptr<SpriteSheet> sheetCopy = std::shared_ptr<SpriteSheet>(new SpriteSheet(*sheet.second));
            _sprites[sheet.first] = sheetCopy;
        }
    }

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

   /**
	* @brief	Query if this object is destroyed.
	*
 	* @return	true if destroyed, false if not.
	*/
	bool IsDestroyed() const;

    /**
     * @brief Query if this object is active (should be considered in updates).
     *        Actors that are inactive when a checkpoint is hit are destroyed.
     *
     * @return Whether this object is active.
     */
    bool IsActive() const;

    /**
     * @brief Sets whether this object is active (should be considered in updates).
     *
     * @param active Whether this object should be active.
     */
    void SetActive(bool active);

	/**
	 * @brief	Destroy this object.
	 */
	void Destroy();

    /**
     * @brief Whethis actor will be destroyed upon being reset.
     */
    bool DestroysOnReset() const;

    /**
     * @brief Sets whethis actor will be destroyed upon being reset.
     * @param dsstroyOnReset Whether this actor should be destroyed upon being reset.
     */
    void SetDestroyOnReset(bool destroyOnReset);

	// All the state changing stuff happens in here. 
	virtual void Update(double elapsedSecs);

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition(double elapsedSecs);

	virtual Type GetType() const = 0;

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

    /**
     * @brief Clones the actor, using the underlying type to create the instance. If the actor isn't cloneable, returns nullptr
     *
     * @return The cloned actor.
     */
    virtual Actor* Clone() = 0;

    /**
     * @brief Queries whether this actor is cloneable.
     *
     * @return Whether this actor is cloneable.
     */
    virtual bool IsCloneable() const = 0;

protected:

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

        // Whether a position correction should be applied in the given axis
        bool shouldCorrectX;
        bool shouldCorrectY;

        // The corner tile (set only when shouldCorrectX && shouldCorrectY)
        std::shared_ptr<Tile> corner;

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

	/** @brief	The direction that the sprite is facing along the x axis. */
	SpriteSheet::XAxisDirection _startXDir;

	/** @brief	The direction that the sprite is facing along the y axis. */
	SpriteSheet::YAxisDirection _startYDir;

	/** @brief	true if this object is visible. */
	bool _isVisible;

	bool _isDestroyed;

    bool _destroyOnReset;

    /** @brief true if this Actor should be updated, checked against for collision, etc. */
    bool _isActive;

	/**
	 * @brief	Determines the tiles with which this actor is colliding.
	 *
	 * @author	Shane
	 * @date	2/5/2016
	 *
	 * @param [in,out]	level		  	The level containing the actor and the tiles.
	 */
	void DetectTileCollisions(TileCollisionInfo& colInfo, std::shared_ptr<Level>& level);

    void GetTilesAlongEdge(Edge edge, const Bounds& bounds, std::vector<std::shared_ptr<Tile>>& tiles);

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