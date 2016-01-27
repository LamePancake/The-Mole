#pragma once
#include "Tile.h"

/**
* Tile_Stone.h
* Represents an stone tile in in the level.
*/
class TileStone : public Tile
{
public:

	/**
	* Default constuctor.
	* Initilizes everying to default values.
	*/
	TileStone() :Tile() {}

	/**
	* Constructor.
	* int x - a cached value of the x index in the array of tiles in a level.
	* int y - a cached value of the y index in the array of tiles in a level.
	*/
	TileStone(int x, int y) : Tile(x, y) {}

	/**
	* Constructor.
	* int x - a cached value of the x index in the array of tiles in a level.
	* int y - a cached value of the y index in the array of tiles in a level.
	* int width - the width of the tile.
	* int height - the height of the tile.
	*/
	TileStone(int x, int y, int width, int height) : Tile(x, y, width, height) {}

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

private:

};