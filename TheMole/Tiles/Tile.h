#pragma once
#include <tuple>

/**
* Tile.h
* Represents an abstract tile in in the level.
*/
class Tile
{
public:

	/**
	* Default constuctor. 
	* Initilizes everying to default values.
	*/
	Tile(): _width(DEFAULT_TILE_WIDTH), 
		    _height(DEFAULT_TILE_HEIGHT),
			_xIndex(0),
			_yIndex(0) {}

	/**
	* Constructor.
	* int x - a cached value of the x index in the array of tiles in a level.
	* int y - a cached value of the y index in the array of tiles in a level.
	* int width - the width of the tile.
	* int height - the height of the tile.
	*/
	Tile(int x, int y, int width = DEFAULT_TILE_WIDTH, int height = DEFAULT_TILE_HEIGHT ) : 
		_width(width),
		_height(height),
		_xIndex(x),
	    _yIndex(y) {}

	/**
	* OVERRIDE.
	* Handles drawing the tile.
	*/
	virtual void Draw();
	
	/**
	* OVERRIDE.
	* Handles updating the tile every frame.
	* float deltaTime - the time since last update.
	*/
	virtual void Update(float deltaTime);

	/**
	* Returns the width in pixels of the tile.
	*/
	int const GetWidth();
	
	/**
	* Returns the height in pixels of the tile.
	*/
	int const GetHeight();

	/**
	* Sets the width in pixels of the tile.
	* int width - the width of the tiles in pixels.
	*/
	void SetWidth(int width);

	/**
	* Sets the height of the tile in pixels.
	* int height - the height of the tile in pixels.
	*/
	void SetHeight(int height);

	/**
	* Returns the x, y pair of cached indices of the tile in an array of tiles.
	*/
	std::pair<int, int> GetIndices();

	/**
	* Sets the indices of the tile.
	* int x - the x index.
	* int y - the y index.
	*/
	void SetIndices(int x, int y);

	/**
	* Returns the world position (x, y) of the top left corner of the tile.
	* Calculated using x * width, y * height.
	*/
	std::pair<int, int> GetWorldPosition();

private:
	/**
	* The width of the tile in pixels.
	*/
	int _width;

	/**
	* The height of the tile in pixels.
	*/
	int _height;

	/**
	* The x index of the tile in an array of tiles (cached).
	*/
	int _xIndex;

	/**
	* The y index of the tile in an array of tiles (cached).
	*/
	int _yIndex;

	/**
	* The default width in pixels of the tile.
	*/
	const static int DEFAULT_TILE_WIDTH = 64;

	/**
	* The default height in pixels of the tile.
	*/
	const static int DEFAULT_TILE_HEIGHT = 64;
};