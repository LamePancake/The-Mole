#pragma once
#include <SDL2pp\SDL2pp.hh>
#include "Vector2.h"

/** Represents an abstract tile in in the level. */
class Tile
{
public:

	/** The default width in pixels of the tile. */
	const static size_t DEFAULT_TILE_WIDTH = 64;

	/** The default height in pixels of the tile. */
	const static size_t DEFAULT_TILE_HEIGHT = 64;

	/** Values that represent tile types. */
	enum TileType : char
	{
		blank       = ' ',
		stone       = 'S',
		dirt        = 'D',
		spike       = 'K',
		metal       = 'M',
		enemy       = 'E',
		goal        = 'G',
		collectible = 'C',
		tunnel      = 'T',
		origin      = 'O',
		npc         = 'N',
		checkpoint  = 'F',
		boss        = 'B',
		projectile  = 'P',
		turret		= '!',
	};

	/** Default constuctor. Initilizes everying to default values. */
	Tile(): _xIndex(0),
		    _yIndex(0),
		    _id(blank),
		    _width(DEFAULT_TILE_WIDTH), 
		    _height(DEFAULT_TILE_HEIGHT) {}

	/**
	 * Constructor.
	 *
	 * @param	x	  	The x index. Cached x index into the level array.
	 * @param	y	  	The y index. Cached y index into level array.
	 * @param	id	  	The identifier for the type of tile.
	 * @param	width 	The width of the tile.
	 * @param	height	The height of the tile.
	 */
	Tile(int x, int y, char id, int width = DEFAULT_TILE_WIDTH, int height = DEFAULT_TILE_HEIGHT ) : 
		_xIndex(x),
		_yIndex(y),
		_id(id),
		_width(width),
		_height(height) {}

	/** OVERRIDE. Handles drawing the tile. */
	virtual void Draw();

	/**
	 * OVERRIDE. Handles updating the tile every frame.
	 *
	 * @param	deltaTime	The delta time since the last update.
	 */
	virtual void Update(float deltaTime);

	/**
	 * Returns the width in pixels of the tile.
	 *
	 * @return	The width of the tile.
	 */
	size_t const GetWidth() const;

	/**
	 * Returns the height in pixels of the tile.
	 *
	 * @return	The height of the tile.
	 */
	size_t const GetHeight() const;

	/**
	 * Sets the width in pixels of the tile.
	 *
	 * @param	width	The new width of the tile in pixels.
	 */
	void SetWidth(size_t width);

	/**
	 * Sets the height of the tile in pixels.
	 *
	 * @param	height	The height of the tile in pixels.
	 */
	void SetHeight(size_t height);

	/**
	 * Returns the x, y pair of cached indices of the tile in an array of tiles.
	 *
	 * @return	The x y indicies.
	 */
	SDL2pp::Point GetIndices() const;

	/**
	 * Sets the indices of the tile of where it is placed in the level.
	 *
	 * @param	x	The x index.
	 * @param	y	The y index.
	 */
	void SetIndices(int x, int y);

	/**
	 * Returns the world position (x, y) of the top left corner of the tile. Calculated using x *
	 * width, y * height.
	 *
	 * @return	The world position of the tile.
	 */
    Vector2 GetWorldPosition();

	/**
	 * Gets the identifier for the type of tile this is.
	 *
	 * @return	The identifier.
	 */
	char GetID() const;

	/**
	 * Sets an identifier for the type of tile this is.
	 *
	 * @param	id	The identifier.
	 */
	void SetID(char id);

private:
	/** The width of the tile in pixels. */
	size_t _width;

	/** The height of the tile in pixels. */
	size_t _height;

	/** The x index of the tile in an array of tiles (cached). */
	int _xIndex;

	/** The y index of the tile in an array of tiles (cached). */
	int _yIndex;

	/** The identifier fo the type of tile. */
	char _id;
};