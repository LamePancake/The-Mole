#pragma once
#include <SDL2pp\SDL2pp.hh>

/** Represents an abstract tile in in the level. */
class Tile
{
public:

	/** Values that represent tile types. */
	enum TileType: char
	{
		blank       = ' ',
		stone       = 'S',
		dirt        = 'D',
		spike       = 'P',
		metal       = 'M',
		enemy       = 'E',
		goal        = 'G',
		collectible = 'C',
	};

	/** Default constuctor. Initilizes everying to default values. */
	Tile(): _width(DEFAULT_TILE_WIDTH), 
		    _height(DEFAULT_TILE_HEIGHT),
			_xIndex(0),
			_yIndex(0),
			_id(blank) {}

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
		_width(width),
		_height(height),
		_xIndex(x),
	    _yIndex(y),
		_id(id) {}

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
	int const GetWidth() const;

	/**
	 * Returns the height in pixels of the tile.
	 *
	 * @return	The height of the tile.
	 */
	int const GetHeight() const;

	/**
	 * Sets the width in pixels of the tile.
	 *
	 * @param	width	The new width of the tile in pixels.
	 */
	void SetWidth(int width);

	/**
	 * Sets the height of the tile in pixels.
	 *
	 * @param	height	The height of the tile in pixels.
	 */
	void SetHeight(int height);

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
	SDL2pp::Point GetWorldPosition();

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
	int _width;

	/** The height of the tile in pixels. */
	int _height;

	/** The x index of the tile in an array of tiles (cached). */
	int _xIndex;

	/** The y index of the tile in an array of tiles (cached). */
	int _yIndex;

	/** The identifier fo the type of tile. */
	char _id;

	/** The default width in pixels of the tile. */
	const static int DEFAULT_TILE_WIDTH = 64;

	/** The default height in pixels of the tile. */
	const static int DEFAULT_TILE_HEIGHT = 64;
};