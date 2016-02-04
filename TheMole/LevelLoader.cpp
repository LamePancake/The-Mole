#include "LevelLoader.h"

// Loads the level
 std::shared_ptr<Level> LevelLoader::LoadLevel(std::string levelPath)
{
	int    levelWidth   = 0; // Keeps track of the width of the level
	int    levelHeight  = 0; // Keeps track of the height of the level
	size_t tileWidth    = 0;
	size_t tileHeight   = 0;

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
					
			tileWidth  = tile->GetWidth();
			tileHeight = tile->GetHeight();

			if (toupper((*it)) == Tile::enemy)
			{
				std::shared_ptr<SimpleAgent> e = std::make_shared<SimpleAgent>(tile->GetWorldPosition(), *GameManager::GetInstance(), Vector2(0.9f, 0.0f), ".\\Assets\\Textures\\baddie.png");
				level->AddEnemy(e);
				tile->SetID(Tile::blank);
			}
			else if (toupper((*it)) == Tile::origin)
			{
				tile->SetID(Tile::blank);
			}

			level->AddTileToLevel(tile, levelHeight);
			level->AddPositions(toupper((*it)), SDL2pp::Point(levelWidth, levelHeight));

			levelWidth++;
		}

		level->SetTileWidth(tileWidth);
		level->SetTileHeight(tileHeight);

		levelWidth = 0;
		levelHeight++;
	}
	
	inFile.close();

	return level;
}
