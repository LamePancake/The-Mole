#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "Tiles/Tile.h"
#include "Tiles/TileDirt.h"
#include "Tiles/TileMetal.h"
#include "Tiles/TileStone.h"
#include "Tiles/TileBlank.h"
#include "Tiles/TileSpike.h"

/**
* LevelLoader.h
* Loads a level from a text file and returns a vector of tiles that represent the level.
*/
class LevelLoader
{
public:

	/**
	*Default Constructor.
	*/
	LevelLoader() {}

	/**
	* Loads in the level and returns an array of tiles.
	* string level_path - the path to the file that contains the level.
	*/
	std::vector<std::vector<Tile*>> LoadLevel(std::string level_path);

private:

	/**
	* Returns a new tile based on the character passed to it.
	* char tile - the type of tile to be created.
	* int x - the x index of the tile.
	* int y - the y index of the tile.
	*/
	Tile* CreateTile(char tile, int x, int y);
};