#pragma once
#include "Tile.h"

/** Represents an metal tile in in the level. */
class TileMetal : public Tile
{
public:

	/** Default constuctor. Initilizes everying to default values. */
	TileMetal() :Tile() {}

	/**
	* Constructor.
	*
	* @param	x	The x index. Cached x index into level array.
	* @param	y	The y index. Cached y index into level array.
	*/
	TileMetal(int x, int y) : Tile(x, y) {}

	/**
	* Constructor.
	*
	* @param	x	  	The x index. Cached x index into the level array.
	* @param	y	  	The y index. Cached y index into the level array.
	* @param	width 	The width of the tile.
	* @param	height	The height of the tile
	*/
	TileMetal(int x, int y, int width, int height) : Tile(x, y, width, height) {}


	/** OVERRIDE. Handles drawing the tile. */
	virtual void Draw();

	/**
	* OVERRIDE. Handles updating the tile every frame.
	*
	* @param	deltaTime	The delta time since last update.
	*/
	virtual void Update(float deltaTime);


private:

};