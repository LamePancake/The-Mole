#include "LevelLoader.h"

// Loads the level
std::vector<std::vector<Tile*>> LevelLoader::LoadLevel(std::string level_path)
{
	int level_width  = 0; // Keeps track of the width of the level
	int level_height = 0; // Keeps track of the height of the level

	std::vector<std::vector<Tile*>> level; // Stores the level
	std::string line; // Stores a line to be read

	std::ifstream inFile(level_path.c_str());

	if (!inFile.is_open())
	{
		std::cerr << "Unable to load level: " << level_path << std::endl;
		return std::vector<std::vector<Tile*>>();
	}

	while (std::getline(inFile, line))
	{
		level.push_back(std::vector<Tile*>());
		for (auto it = line.begin(); it != line.end(); ++it)
		{
			level[level_height].push_back(CreateTile((*it), level_width, level_height));
			level_width++;
		}

		level_width = 0;
		level_height++;
	}
	
	return level;
}

// Creates a tile based on the character passed into to it
Tile* LevelLoader::CreateTile(char tile, int x, int y)
{
	switch (toupper(tile))
	{
		case 'D':
		{
			return new Tile_Dirt(x, y);
		}
		case 'M':
		{
			return new Tile_Metal(x, y);
		}
		case 'S':
		{
			return  new Tile_Stone(x, y);
		}
		case ' ':
		{
			return new Tile_Blank(x, y);
		}
		default:
		{
			return new Tile_Blank(x, y);
		}
	}
}
