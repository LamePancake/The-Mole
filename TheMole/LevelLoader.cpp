#include "LevelLoader.h"

// Loads the level
std::shared_ptr<Level> LevelLoader::LoadLevel(std::string levelPath, std::shared_ptr<PlayerActor> & player)
{
	int    levelWidth = 0; // Keeps track of the width of the level
	int    levelHeight = 0; // Keeps track of the height of the level
	size_t tileWidth = 0;
	size_t tileHeight = 0;

	std::shared_ptr<Level> level = std::make_shared<Level>(); // Stores the level
	std::string line; // Stores a line to be read

	std::ifstream inFile(levelPath.c_str());

	if (!inFile.is_open())
	{
		std::cerr << "Unable to load level: " << levelPath << std::endl;
		return nullptr;
	}

	GameManager& gameManager = *GameManager::GetInstance();

	// Welcome to not wanting to properly make a texture cache
	// So long as we don't have *too* many repeated textures, I'm sure that this list will be totally manageable :):):):):):)
	std::shared_ptr<SDL2pp::Texture> baddieWalkSheet = std::make_shared<SDL2pp::Texture>(gameManager.GetRenderer(), ".\\Assets\\Textures\\Baddie_walk_56x56.png");
	std::shared_ptr<SDL2pp::Texture> flagSheet = std::make_shared<SDL2pp::Texture>(gameManager.GetRenderer(), ".\\Assets\\Textures\\Flag_raise.png");
	std::shared_ptr<SDL2pp::Texture> pancakeSheet = std::make_shared<SDL2pp::Texture>(gameManager.GetRenderer(), ".\\Assets\\Textures\\Pancake.png");
	std::shared_ptr<SDL2pp::Texture> projectileSheet = std::make_shared<SDL2pp::Texture>(gameManager.GetRenderer(), ".\\Assets\\Textures\\red_dot.png");

	while (std::getline(inFile, line))
	{
		for (auto it = line.begin(); it != line.end(); ++it)
		{
			char tileType = toupper(*it);
			std::shared_ptr<Tile> tile = std::make_shared<Tile>(levelWidth, levelHeight, tileType);

			tileWidth = tile->GetWidth();
			tileHeight = tile->GetHeight();

			switch (tileType)
			{
			case Tile::enemy:
			{
				std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> enemySprites;
				enemySprites["walk"] = std::make_shared<SpriteSheet>(baddieWalkSheet, 8, 1.f);

				std::shared_ptr<AIActor> e = std::make_shared<AIActor>(tile->GetWorldPosition(), gameManager, Vector2(100.0f, 341.3f), enemySprites, "walk");
				level->AddEnemy(e);
				tile->SetID(Tile::blank);
			}
			break;
			case Tile::origin:
			{
				std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
				sprites.reserve(4);
				sprites["sideDig"] = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\Borin_sidedig_56x56.png", 4, 0.30, false);
				sprites["verticalDig"] = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\Borin_downdig_56x56.png", 4, 0.30, false, SpriteSheet::XAxisDirection::RIGHT, SpriteSheet::YAxisDirection::DOWN);
				sprites["walk"] = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\Borin_walk_56x56.png", 8, 1);
				sprites["idle"] = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\Borin_idle_56x56.png", 4, 0.8);

				player = std::make_shared<PlayerActor>(tile->GetWorldPosition(), gameManager, Vector2(.0f, 341.3f), sprites, "idle");
				tile->SetID(Tile::blank);
			}
			break;
			case Tile::npc:
			{
				std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
				sprites["idle"] = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\Toad_idle.png", 6, 0.50, true, SpriteSheet::XAxisDirection::LEFT);

				std::shared_ptr<NPCActor> npc = std::make_shared<NPCActor>(tile->GetWorldPosition(), gameManager, Vector2(0, 0), sprites, "idle", SpriteSheet::XAxisDirection::LEFT);
				level->AddNPC(npc);
				tile->SetID(Tile::blank);
			}
			break;
			case Tile::checkpoint:
			{
				std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
				sprites["raise"] = std::make_shared<SpriteSheet>(flagSheet, 6, 1.0, false);
				sprites["raise"]->Pause();

				std::shared_ptr<ObjectActor> flag = std::make_shared<ObjectActor>(tile->GetWorldPosition(), gameManager, Vector2(0, 0), ObjectActor::flag, sprites, "raise");
				level->AddActorObject(flag);
				tile->SetID(Tile::blank);
			}
			break;
			case Tile::collectible:
				{
					std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
					double infinity = std::numeric_limits<double>::infinity();
					sprites["whateverPancakesDo"] = std::make_shared<SpriteSheet>(pancakeSheet, 1, infinity);

					std::shared_ptr<ObjectActor> collectible = std::make_shared<ObjectActor>(tile->GetWorldPosition(), gameManager, Vector2(0, 0), ObjectActor::pancake, sprites, "whateverPancakesDo");
					level->AddActorObject(collectible);
					tile->SetID(Tile::blank);
				}
				break;
			case Tile::boss:
				{
					std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
					sprites["idle"] = std::make_shared<SpriteSheet>(".\\Assets\\Textures\\Toad_idle.png", 6, 0.50, true, SpriteSheet::XAxisDirection::LEFT);

					std::shared_ptr<BossActor> boss = std::make_shared<BossActor>(tile->GetWorldPosition(), gameManager, Vector2(0, 0), sprites, "idle", SpriteSheet::XAxisDirection::LEFT);
					level->AddBoss(boss);
					tile->SetID(Tile::blank);
				}
				break;
			case Tile::projectile:
				{
					std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
					double infinity = std::numeric_limits<double>::infinity();
					sprites["shoot"] = std::make_shared<SpriteSheet>(projectileSheet, 1, infinity);

					std::shared_ptr<ProjectileActor> projectile = std::make_shared<ProjectileActor>(tile->GetWorldPosition(), gameManager, Vector2(0, 0), sprites, "shoot", SpriteSheet::XAxisDirection::LEFT);
					level->AddProjectileObject(projectile);
					tile->SetID(Tile::blank);
				}
				break;
			///Implement this later
			//case Tile::turret:
			//{
			//	std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
			//	double infinity = std::numeric_limits<double>::infinity();
			//	sprites["turretThings"] = std::make_shared<SpriteSheet>(pancakeSheet, 1, infinity);

			//	std::shared_ptr<ObjectActor> collectible = std::make_shared<ObjectActor>(tile->GetWorldPosition(), gameManager, Vector2(0, 0), ObjectActor::pancake, sprites, "whateverPancakesDo");
			//	level->AddActorObject(collectible);
			//	tile->SetID(Tile::blank);
			//}
			//break;
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
