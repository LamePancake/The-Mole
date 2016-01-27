#pragma once
#include "Tile.h"

/** Represents an stone tile in in the level. */
class TileStone : public Tile
{
public:

	/** Default constuctor. Initilizes everying to default values. */
	TileStone() :Tile() {}

	/**
	 * Constructor. 
	 *
	 * @param	x	The x index. Cached value for the x index in the array for the level.
	 * @param	y	The y index. Cached value for the y index in the array for the level.
	 */
	TileStone(int x, int y) : Tile(x, y) {}

	/**
	 * Constructor. 
	 *
	 * @param	x	  	The x index. Cached value for the x index in the array for the level.
	 * @param	y	  	The y index. Cached value for the y index in the array for the level.
	 * @param	width 	The width of the tile.
	 * @param	height	The height of the tile.
	 */
	TileStone(int x, int y, int width, int height) : Tile(x, y, width, height) {}

	/** OVERRIDE. Handles drawing the tile. */
	virtual void Draw();

	/**
	 * OVERRIDE. Handles updating the tile every frame.
	 *
	 * @param	deltaTime	The delta time since the last update.
	 */
	virtual void Update(float deltaTime);

private:

};