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

/** Loads a level from a text file and returns a vector of tiles that represent the level. */
class LevelLoader
{
public:

	/** Default Constructor. */
	LevelLoader() {}

	/**
	 * Loads in the level and returns an array of tiles. 
	 *
	 * @param	level_path	Full pathname of the level file.
	 *
	 * @return	null if it fails, else the level.
	 */
	std::vector<std::vector<Tile*>> LoadLevel(std::string level_path);

private:

	/**
	 * Returns a new tile based on the character passed to it.
	 *
	 * @param	tile	The type of tile to be created.
	 * @param	x   	The x index into the level.
	 * @param	y   	The y index into the level.
	 *
	 * @return	null if it fails, else the new tile.
	 */
	Tile* CreateTile(char tile, int x, int y);
};