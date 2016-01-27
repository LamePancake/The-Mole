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
			std::shared_ptr<Tile> tile = nullptr;

			// Creates the new tile and addes it to the type list in level, for rendering
			switch (toupper((*it)))
			{
				case 'D':
				{
					tile = std::make_shared<TileDirt>(levelWidth, levelHeight);
					level->AddDirt(tile);
					break;
				}
				case 'M':
				{
					tile = std::make_shared<TileMetal>(levelWidth, levelHeight);
					level->AddMetal(tile);
					break;
				}
				case 'S':
				{
					tile = std::make_shared<TileStone>(levelWidth, levelHeight);
					level->AddStone(tile);
					break;
				}
				case 'P':
				{
					tile = std::make_shared<TileSpike>(levelWidth, levelHeight);
					level->AddSpike(tile);
					break;
				}
				case ' ':
				{
					tile = std::make_shared<TileBlank>(levelWidth, levelHeight);
					level->AddBlank(tile);
					break;
				}
				default:
				{
					tile = std::make_shared<TileBlank>(levelWidth, levelHeight);
					level->AddBlank(tile);
					break;
				}
			}

			// Stores the tile in the level as well
			level->AddTileToLevel(tile, levelHeight); 
			levelWidth++;
		}

		levelWidth = 0;
		levelHeight++;
	}
	
	inFile.close();

	return level;
}
