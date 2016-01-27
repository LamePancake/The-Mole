#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#include "Tiles/Tile.h"
#include "Tiles/TileDirt.h"
#include "Tiles/TileMetal.h"
#include "Tiles/TileStone.h"
#include "Tiles/TileBlank.h"
#include "Tiles/TileSpike.h"
#include "Level.h"

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
	std::shared_ptr<Level> LoadLevel(std::string levelPath);

private:

};