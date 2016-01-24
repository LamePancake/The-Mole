#pragma once
#include "Tile.h"

/**
* Tile_Metal.h
* Represents an metal tile in in the level.
*/
class Tile_Metal : public Tile
{
public:
	/**
	* Default constuctor.
	* Initilizes everying to default values.
	*/
	Tile_Metal() :Tile() {}

	/**
	* Constructor.
	* int x - a cached value of the x index in the array of tiles in a level.
	* int y - a cached value of the y index in the array of tiles in a level.
	*/
	Tile_Metal(int x, int y) : Tile(x, y) {}

	/**
	* Constructor.
	* int x - a cached value of the x index in the array of tiles in a level.
	* int y - a cached value of the y index in the array of tiles in a level.
	* int width - the width of the tile.
	* int height - the height of the tile.
	*/
	Tile_Metal(int x, int y, int width, int height) : Tile(x, y, width, height) {}

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