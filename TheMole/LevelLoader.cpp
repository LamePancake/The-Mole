#include "LevelLoader.h"

// Loads the level
 std::shared_ptr<Level> LevelLoader::LoadLevel(std::string levelPath)
{
	int levelWidth  = 0; // Keeps track of the width of the level
	int levelHeight = 0; // Keeps track of the height of the level

	std::shared_ptr<Level> level = std::make_shared<Level>(); // Stores the level
	std::string line; // Stores a line to be read

	std::ifstream inFile(levelPath.c_str());

	if (!inFile.is_open())
	{
		std::cerr << "Unable to load level: " << levelPath << std::endl;
		return nullptr;
	}

	while (std::getline(inFile, line))
	{
		for (auto it = line.begin(); it != line.end(); ++it)
		{
			std::shared_ptr<Tile> tile = std::make_shared<Tile>(levelWidth, levelHeight, toupper((*it)));
			level->AddTileToLevel(tile, levelHeight); 
			levelWidth++;
		}

		levelWidth = 0;
		levelHeight++;
	}
	
	inFile.close();

	return level;
}
