#include "LevelLoader.h"

// Loads the level
 std::shared_ptr<Level> LevelLoader::LoadLevel(std::string levelPath, std::shared_ptr<PlayerActor> & player)
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
				std::shared_ptr<AIActor> e = std::make_shared<AIActor>(tile->GetWorldPosition(), *GameManager::GetInstance(), Vector2(100.0f, 200.0f), ".\\Assets\\Textures\\Baddie_walk_56x56.png");
				level->AddEnemy(e);
				tile->SetID(Tile::blank);
			}
			else if (toupper((*it)) == Tile::origin)
			{
				player = std::make_shared<PlayerActor>(tile->GetWorldPosition(), *GameManager::GetInstance(), Vector2(0.0f, 0.0f), ".\\Assets\\Textures\\Borin_idle_56x56.png");
				tile->SetID(Tile::blank);
			}
			else if (toupper((*it)) == Tile::npc)
			{
				std::shared_ptr<NPCActor> n = std::make_shared<NPCActor>(tile->GetWorldPosition(), *GameManager::GetInstance(), Vector2(0, 0), ".\\Assets\\Textures\\Toad_idle.png");
				level->AddNPC(n);
				tile->SetID(Tile::blank);
			}
			else if(toupper((*it)) == Tile::checkpoint)
			{
				std::shared_ptr<ObjectActor> f = std::make_shared<ObjectActor>(tile->GetWorldPosition(), *GameManager::GetInstance(), Vector2(0, 0), ".\\Assets\\Textures\\Flag_raise.png", ObjectActor::flag, 6);
				level->AddActorObject(f);
				tile->SetID(Tile::blank);
			}
			else if (toupper((*it)) == Tile::collectible)
			{
				std::shared_ptr<ObjectActor> c = std::make_shared<ObjectActor>(tile->GetWorldPosition(), *GameManager::GetInstance(), Vector2(0, 0), ".\\Assets\\Textures\\Pancake.png", ObjectActor::pancake, 1);
				level->AddActorObject(c);
				tile->SetID(Tile::blank);
			}

			level->AddTileToLevel(tile, levelHeight);
			level->AddPositions(toupper((*it)), SDL2pp::Point(levelWidth, levelHeight));

			levelWidth++;
		}

		levelWidth = 0;
		levelHeight++;
	}

	level->SetTileWidth(tileWidth);
	level->SetTileHeight(tileHeight);

	inFile.close();

	return level;
}
