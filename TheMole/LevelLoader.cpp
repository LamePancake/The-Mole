#include "LevelLoader.h"
#include "GameScreen.h"
#include "Util.h"

#include "Tile.h"
#include "Level.h"
#include "AIActor.h"
#include "BombAIActor.h"
#include "Actor.h"
#include "PlayerActor.h"
#include "NPCActor.h"
#include "ObjectActor.h"
#include "BossActor.h"
#include "ProjectileActor.h"
#include "TurretActor.h"
#include "ToggleActor.h"
#include "DoorActor.h"
#include "ActorSpawner.h"
#include "TargetedActorSpawner.h"

using std::shared_ptr;
using std::vector;
using std::unordered_map;
using std::string;
using std::ifstream;

#define DEN 0
#define POSTGRASS 1

// Loads the level
std::shared_ptr<Level> LevelLoader::LoadLevel(std::string levelPath, std::shared_ptr<PlayerActor> & player, int specialLevel)
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
	// So long as we don't have *too* many repeated textures, I'm sure that this list will be totally manageable :):):):):):):););)
	// 10/10 would read again - Trey
	std::shared_ptr<SDL2pp::Texture> baddieWalkSheet = std::make_shared<SDL2pp::Texture>(gameManager.GetRenderer(), "./Assets/Textures/Baddie_walk_56x56.png");
	std::shared_ptr<SDL2pp::Texture> bombSheet = std::make_shared<SDL2pp::Texture>(gameManager.GetRenderer(), "./Assets/Textures/Explosion.png");
	std::shared_ptr<SDL2pp::Texture> mindControlIndicator = std::make_shared<SDL2pp::Texture>(gameManager.GetRenderer(), "./Assets/Textures/Controlled_indicator.png");

	std::unordered_map<char, std::vector<SDL2pp::Point>> positions;

	// Read the actual map data, stopping when we get to switch descriptions etc.
	do
	{
		std::getline(inFile, line);
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
                std::unordered_map<std::string, std::pair<std::string, bool>> enemySounds = {};

				std::shared_ptr<AIActor> e = std::make_shared<AIActor>(gameManager, tile->GetWorldPosition(), Vector2(100.0f, 341.3f), enemySprites, "walk",
                    enemySounds, mindControlIndicator, tile->GetWorldPosition());
				level->AddActor(e);
				tile->SetID(Tile::blank);
			}
			break;
            case Tile::bombenemy:
            {
                std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> bombSprites;
                bombSprites["walk"] = std::make_shared<SpriteSheet>(baddieWalkSheet, 8, 1.f);
                bombSprites["blow_up"] = std::make_shared<SpriteSheet>(bombSheet, 11, 2.f, false);

                std::unordered_map<std::string, std::pair<std::string, bool>> bomberSounds = { {"blow_up", {"explosion", false}} };

                std::shared_ptr<BombAIActor> e = std::make_shared<BombAIActor>(tile->GetWorldPosition(), gameManager, Vector2(100.0f, 341.3f), bombSprites, "walk",
                    bomberSounds, mindControlIndicator, tile->GetWorldPosition());
                level->AddActor(e);
                tile->SetID(Tile::blank);
            }
            break;
			case Tile::origin:
			{
				std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
				sprites.reserve(14);
				if (specialLevel == DEN)
				{
					sprites["walk"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_den_walk_56x56.png", 8, 1);
					sprites["idle"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_den_idle_56x56.png", 4, 0.8);
					sprites["sideDig"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_den_sidedig_56x56.png", 4, 0.30);
				}
				else if (specialLevel == POSTGRASS)
				{
					sprites["walk"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_walk_56x56_sidewithchicken.png", 8, 1);
					sprites["idle"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_idle_56x56_sidewithchicken.png", 4, 0.8);
					sprites["sideDig"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_sidedig_56x56.png", 4, 0.30);
				}
				else
				{
					sprites["walk"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_walk_56x56.png", 8, 1);
					sprites["idle"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_idle_56x56.png", 4, 0.8);
					sprites["sideDig"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_sidedig_56x56.png", 4, 0.30);
				}

				sprites["verticalDig"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_downdig_56x56.png", 4, 0.30, true, SpriteSheet::XAxisDirection::RIGHT, SpriteSheet::YAxisDirection::DOWN);
				sprites["shieldOff"] = std::make_shared<SpriteSheet>("./Assets/Textures/Empty_Shield.png", 1, 1);
				sprites["shieldOn"] = std::make_shared<SpriteSheet>("./Assets/Textures/Full_Shield.png", 1, 1);
				sprites["chickenHatWalk"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_walk_56x56_chicken.png", 8, 1);
				sprites["chickenHatIdle"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_idle_56x56_chicken.png", 4, 0.8);
				sprites["mindControlHatWalk"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_walk_56x56_mindcontrol.png", 8, 1);
				sprites["mindControlHatIdle"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_idle_56x56_mindcontrol.png", 4, 0.8);
				sprites["vikingHatWalk"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_walk_56x56_viking.png", 8, 1);
				sprites["vikingHatIdle"] = std::make_shared<SpriteSheet>("./Assets/Textures/Borin_idle_56x56_viking.png", 4, 0.8);

                std::unordered_map<std::string, std::pair<std::string, bool>> playerSounds = {};
				player = std::make_shared<PlayerActor>(tile->GetWorldPosition(), gameManager, Vector2(.0f, 341.3f), sprites, "idle", playerSounds);
				tile->SetID(Tile::blank);
				level->SetSpawnPoint(tile->GetWorldPosition(), 0);
				level->AddActor(player);
			}
			break;
			case Tile::npc:
			{
				positions[Tile::npc].push_back(SDL2pp::Point(tile->GetWorldPosition().GetX(), tile->GetWorldPosition().GetY()));
				tile->SetID(Tile::blank);
			}
			break;
			case Tile::checkpoint:
			{
				positions[Tile::checkpoint].push_back(SDL2pp::Point(tile->GetWorldPosition().GetX(), tile->GetWorldPosition().GetY()));
				tile->SetID(Tile::blank);
			}
			break;
			case Tile::collectible:
			{
				positions[Tile::collectible].push_back(SDL2pp::Point(tile->GetWorldPosition().GetX(), tile->GetWorldPosition().GetY()));
				tile->SetID(Tile::blank);
			}
			break;
			case Tile::boss:
			{
				double infinity = std::numeric_limits<double>::infinity();
                    // Boss sprites
					std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
					sprites["idle"] = std::make_shared<SpriteSheet>("./Assets/Textures/Watch_idle.png", 4, 0.50, true, SpriteSheet::XAxisDirection::LEFT);
                    sprites["preroll"] = std::make_shared<SpriteSheet>("./Assets/Textures/Watch_preroll.png", 4, 0.75, false, SpriteSheet::XAxisDirection::LEFT);
                    sprites["roll"] = std::make_shared<SpriteSheet>("./Assets/Textures/Watch_roll.png", 10, 0.50, true, SpriteSheet::XAxisDirection::LEFT);
                    sprites["overheat"] = std::make_shared<SpriteSheet>("./Assets/Textures/Watch_overheat.png", 12, 1, false, SpriteSheet::XAxisDirection::LEFT);
                    sprites["dead"] = std::make_shared<SpriteSheet>("./Assets/Textures/Watch_death.png", 1, infinity, false, SpriteSheet::XAxisDirection::LEFT);

                    // Boss sounds
                    std::unordered_map<std::string, std::pair<std::string, bool>> bossSounds = {};

                    std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> alienSprites;
                    alienSprites["turret"] = shared_ptr<SpriteSheet>(new SpriteSheet("./Assets/Textures/Graylien_shoot.png", 4, 0.75f, false));
                    alienSprites["dead"] = shared_ptr<SpriteSheet>(new SpriteSheet("./Assets/Textures/Graylien_dead.png", 1, infinity, false));
                    alienSprites["shoot"] = shared_ptr<SpriteSheet>(new SpriteSheet("./Assets/Textures/laser.png", 1, infinity, false));

                    std::unordered_map<std::string, std::pair<std::string, bool>> alienSounds = { {"turret", {"laser", false}} };
                    std::shared_ptr<TurretActor> alien = std::make_shared<TurretActor>(
                                Vector2(0, 0),
                                *GameManager::GetInstance(),
                                Vector2(0, 0),
                                TurretActor::Aim::XAxis,
                                true,
                                0.75,
                                alienSprites,
                                "turret",
                                alienSounds,
                                line == "0" ? SpriteSheet::XAxisDirection::LEFT : SpriteSheet::XAxisDirection::RIGHT,
                                SpriteSheet::YAxisDirection::UP);

                    // Set up the object actor for explosions
                    std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> explosion;
                    explosion["explode"] = shared_ptr<SpriteSheet>(new SpriteSheet(bombSheet, 11, 1.0, false));
                    std::unordered_map<std::string, std::pair<std::string, bool>> explosionSounds = { {"explode", {"explosion", false}} };
                    std::shared_ptr<Actor> explodeProto = shared_ptr<Actor>(new ObjectActor(Vector2(0, 0), 
                                                                                            *GameManager::GetInstance(),
                                                                                            Vector2(0, 0),
                                                                                            -1,
                                                                                            explosion,
                                                                                            "explode",
                                                                                            explosionSounds));
                    std::shared_ptr<TargetedActorSpawner> explosionSpawner = shared_ptr<TargetedActorSpawner>(
                        new TargetedActorSpawner(level, explodeProto, 0.2, 13, Vector2(0, 0), 80));

					std::shared_ptr<BossActor> boss = shared_ptr<BossActor>(new BossActor(tile->GetWorldPosition(),
                                                                                  gameManager,
                                                                                  Vector2(200, 0),
                                                                                  sprites,
                                                                                  "idle",
                                                                                  bossSounds,
                                                                                  alien,
                                                                                  explosionSpawner,
                                                                                  SpriteSheet::XAxisDirection::LEFT));
					level->AddActor(boss);
					tile->SetID(Tile::blank);
				}
				break;
			/*case Tile::projectile:
				{
					std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
					double infinity = std::numeric_limits<double>::infinity();
					sprites["shoot"] = std::make_shared<SpriteSheet>(projectileSheet, 1, infinity);

					std::shared_ptr<ProjectileActor> projectile = std::make_shared<ProjectileActor>(
						tile->GetWorldPosition() ///Vec2 position
						, gameManager ///Gamemanager
						, Vector2(100.0f, 0.0f) ///Vec2 spd
						, sprites ///sprites
						, "shoot" ///startsprite
						, SpriteSheet::XAxisDirection::LEFT); ///direction
					level->AddActor(projectile);
					tile->SetID(Tile::blank);
				}
				break;*/
			case Tile::turret:
			{
				positions[Tile::turret].push_back(SDL2pp::Point(tile->GetWorldPosition().GetX(), tile->GetWorldPosition().GetY()));
				tile->SetID(Tile::blank);
			}
			break;

			case Tile::door:
				positions[Tile::door].push_back(SDL2pp::Point(levelWidth, levelHeight));
				tile->SetID(Tile::blank);
				break;
			case Tile::toggle:
				positions[Tile::toggle].push_back(SDL2pp::Point(levelWidth, levelHeight));
				tile->SetID(Tile::blank);
				break;

			case Tile::dialog:
				positions[Tile::dialog].push_back(SDL2pp::Point(tile->GetWorldPosition().GetX(), tile->GetWorldPosition().GetY()));
				tile->SetID(Tile::blank);
				break;

			case Tile::help:
				positions[Tile::help].push_back(SDL2pp::Point(tile->GetWorldPosition().GetX(), tile->GetWorldPosition().GetY()));
				tile->SetID(Tile::blank);
				break;

			case Tile::spawner:
				positions[Tile::spawner].push_back(SDL2pp::Point(tile->GetWorldPosition().GetX(), tile->GetWorldPosition().GetY()));
				tile->SetID(Tile::blank);
				break;
			}

			level->AddTileToLevel(tile, levelHeight);
			level->AddPositions(toupper((*it)), SDL2pp::Point(levelWidth, levelHeight));

			levelWidth++;
		}

		levelWidth = 0;
		levelHeight++;
	} while (inFile.good() && line.length() == level->GetLevelSize().x);

	level->SetTileWidth(tileWidth);
	level->SetTileHeight(tileHeight);

	// We stopped because we read a line != to level->GetLevelSize().x; must be more data to load
	if (inFile.good())
	{
		LoadActorSpecifics(inFile, line, positions, level);
	}

	inFile.close();

	return level;
}

void LevelLoader::LoadActorSpecifics(ifstream & file, string & lastLine, unordered_map<char, vector<SDL2pp::Point>>& positions, shared_ptr<Level> level)
{
	std::string line = lastLine;

	// Loop until we get to the first actual line
	while (std::getline(file, line))
	{
		while (line.empty() && std::getline(file, line));

		std::transform(line.begin(), line.end(), line.begin(), ::tolower); // make line lowercase
		if (line == "toggles")
		{
			LoadTogglesAndDoors(file, positions[Tile::toggle], positions[Tile::door], level);
		}
		else if (line == "dialog")
		{
			LoadDialog(file, positions[Tile::dialog], level);
		}
		else if (line == "checkpoints")
		{
			LoadCheckPoints(file, positions[Tile::checkpoint], level);
		}
		else if (line == "pancakes")
		{
			LoadPancakes(file, positions[Tile::collectible], level);
		}
		else if (line == "signs")
		{
			LoadHelpSigns(file, positions[Tile::help], level);
		}
		else if (line == "hats")
		{
			LoadHats(file, level);
		}
		else if (line == "npcs")
		{
			LoadNPCS(file, positions[Tile::npc], level);
		}
		else if (line == "spawners")
		{
			LoadActorSpawners(file, positions[Tile::spawner], level);
		}
		else if (line == "turrets")
		{
			LoadTurrets(file, positions[Tile::turret], level);
		}
	}
}

void LevelLoader::LoadTogglesAndDoors(ifstream & file, vector<SDL2pp::Point> & togglePos, vector<SDL2pp::Point> & doorPos, shared_ptr<Level> level)
{
	string line;
	vector<string> tokens;
	tokens.reserve(2);
	GameManager* manager = GameManager::GetInstance();
	SDL2pp::Renderer &rend = manager->GetRenderer();

	shared_ptr<SDL2pp::Texture> door = std::make_shared<SDL2pp::Texture>(rend, "./Assets/Textures/Door.png");
	shared_ptr<SDL2pp::Texture> doorHoriz = std::make_shared<SDL2pp::Texture>(rend, "./Assets/Textures/Door_horiz.png");
	shared_ptr<SDL2pp::Texture> activeToggle = std::make_shared<SDL2pp::Texture>(rend, "./Assets/Textures/Active_toggle.png");
	shared_ptr<SDL2pp::Texture> activeToggleVertical = std::make_shared<SDL2pp::Texture>(rend, "./Assets/Textures/Active_toggle_vertical.png");
	shared_ptr<SDL2pp::Texture> oneShotToggle = std::make_shared<SDL2pp::Texture>(rend, "./Assets/Textures/Oneshot_toggle.png");
	shared_ptr<SDL2pp::Texture> oneShotToggleVertical = std::make_shared<SDL2pp::Texture>(rend, "./Assets/Textures/Oneshot_toggle_vertical.png");


	vector<shared_ptr<ToggleActor>> toggles;

    std::unordered_map<std::string, std::pair<std::string, bool>> doorSounds = { };

    std::unordered_map<std::string, std::pair<std::string, bool>> toggleSounds = { {"toggle", {"switch_on", false}} };

	// We would have just read "toggles", so we can go ahead and start reading all of the pad info in
	for (size_t i = 0; i < togglePos.size(); ++i)
	{
		std::getline(file, line);
		std::transform(line.begin(), line.end(), line.begin(), ::tolower);
		split(line, ' ', tokens);

		Actor::Edge edge = Actor::Edge::NONE;
		shared_ptr<SpriteSheet> sheet;
		SpriteSheet::XAxisDirection xDir = SpriteSheet::XAxisDirection::RIGHT;
		SpriteSheet::YAxisDirection yDir = SpriteSheet::YAxisDirection::UP;
		Vector2 startPos(togglePos[i].x * level->GetTileWidth(), togglePos[i].y * level->GetTileHeight());
		bool isActive = tokens[1] == "active";

		if (tokens[0] == "top")
		{
			edge = Actor::Edge::TOP;
			yDir = SpriteSheet::YAxisDirection::DOWN;
			sheet = std::make_shared<SpriteSheet>(isActive ? activeToggle : oneShotToggle, 2, 0, false);
		}
		else if (tokens[0] == "bottom")
		{
			edge = Actor::Edge::BOTTOM;
			sheet = std::make_shared<SpriteSheet>(isActive ? activeToggle : oneShotToggle, 2, 0, false);
			startPos.SetY(startPos.GetY() + level->GetTileHeight() - sheet->GetFrameHeight());
		}
		else if (tokens[0] == "right")
		{
			edge = Actor::Edge::RIGHT;
			sheet = std::make_shared<SpriteSheet>(isActive ? activeToggleVertical : oneShotToggleVertical, 2, 0, false);
			xDir = SpriteSheet::XAxisDirection::LEFT;
			startPos.SetX(startPos.GetX() + level->GetTileWidth() - sheet->GetFrameWidth());
		}
		else if (tokens[0] == "left")
		{
			edge = Actor::Edge::LEFT;
			sheet = std::make_shared<SpriteSheet>(isActive ? activeToggleVertical : oneShotToggleVertical, 2, 0, false);
		}

		unordered_map<string, shared_ptr<SpriteSheet>> sprites;
		sprites["toggle"] = sheet;
        shared_ptr<ToggleActor> toggle(new ToggleActor(startPos, *GameManager::GetInstance(), Vector2(0, 0), sprites, "toggle", toggleSounds, xDir, yDir, edge, isActive));
        level->AddActor(toggle);
        toggles.push_back(toggle);
        tokens.clear();
 	}

	// Skip any newlines and the "doors" label
     while (std::getline(file, line) && line.empty() || (line.size() == 1 && line[0] == '\r'));

    // Read in all of the doors
    for (size_t i = 0; i < doorPos.size(); ++i)
    {
        std::getline(file, line);
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        split(line, ' ', tokens);

        Actor::Edge edge = Actor::Edge::NONE;
        shared_ptr<SpriteSheet> sheet;
        SpriteSheet::XAxisDirection xDir = SpriteSheet::XAxisDirection::RIGHT;
        SpriteSheet::YAxisDirection yDir = SpriteSheet::YAxisDirection::UP;
        Vector2 startPos(doorPos[i].x * level->GetTileWidth(), doorPos[i].y * level->GetTileHeight());
        if (tokens[0] == "top")
        {
            edge = Actor::Edge::TOP;
            yDir = SpriteSheet::YAxisDirection::DOWN;
            sheet = std::make_shared<SpriteSheet>(doorHoriz, 9, 1.0, false);
        }
        else if (tokens[0] == "bottom")
        {
            edge = Actor::Edge::BOTTOM;
            sheet = std::make_shared<SpriteSheet>(doorHoriz, 9, 1.0, false);
            startPos.SetY(startPos.GetY() + level->GetTileHeight() - sheet->GetFrameHeight());
        }
        else if (tokens[0] == "right")
        {
            edge = Actor::Edge::RIGHT;
            sheet = std::make_shared<SpriteSheet>(door, 9, 1.0, false);
            xDir = SpriteSheet::XAxisDirection::LEFT;
            startPos.SetX(startPos.GetX() + level->GetTileWidth() - sheet->GetFrameWidth());
        }
        else if (tokens[0] == "left")
        {
            edge = Actor::Edge::LEFT;
            sheet = std::make_shared<SpriteSheet>(door, 9, 1.0, false);
        }

        std::istringstream iss(tokens[2]);
        size_t toggleIdx;
        iss >> toggleIdx;

        unordered_map<string, shared_ptr<SpriteSheet>> sprites;
        sprites["open"] = sheet;

        shared_ptr<DoorActor> doorActor(new DoorActor(startPos, 
                                                   *GameManager::GetInstance(),
                                                   Vector2(0, 0),
                                                   sprites,
                                                   "open",
                                                   doorSounds,
                                                   xDir,
                                                   yDir,
	                                               edge,
                                                   toggles[toggleIdx]));
        level->AddActor(doorActor);
        tokens.clear();
    }
}

void LevelLoader::LoadDialog(ifstream & file, vector<SDL2pp::Point> & dialogPos, shared_ptr<Level> level)
{
	string line;
	std::shared_ptr<SDL2pp::Texture> blankSheet = std::make_shared<SDL2pp::Texture>(GameManager::GetInstance()->GetRenderer(), "./Assets/Textures/Pancake.png");
    std::unordered_map<std::string, std::pair<std::string, bool>> dialogSounds = {};

	for (size_t i = 0; i < dialogPos.size(); ++i)
	{
		std::getline(file, line);

		std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
		double infinity = std::numeric_limits<double>::infinity();
		sprites["nothing"] = std::make_shared<SpriteSheet>(blankSheet, 1, infinity);

		std::shared_ptr<ObjectActor> dialog = std::make_shared<ObjectActor>(Vector2(dialogPos[i].GetX(), dialogPos[i].GetY()),
                                                                            *GameManager::GetInstance(),
                                                                            Vector2(0, 0),
                                                                            ObjectActor::dialogTrigger,
                                                                            sprites,
                                                                            "nothing",
                                                                            dialogSounds);
		dialog->SetDialog(line);
		level->AddActor(dialog);
	}
}

void LevelLoader::LoadCheckPoints(ifstream & file, vector<SDL2pp::Point>& checkPointPos, shared_ptr<Level> level)
{
    string line;
    std::shared_ptr<SDL2pp::Texture> flagSheet = std::make_shared<SDL2pp::Texture>(GameManager::GetInstance()->GetRenderer(), "./Assets/Textures/Flag_raise.png");
    std::unordered_map<std::string, std::pair<std::string, bool>> checkpointSounds = {}; // There actually are sounds, but I'm not going to mess with this

	for (size_t i = 0; i < checkPointPos.size(); ++i)
	{
		std::getline(file, line);

		std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
		sprites["raise"] = std::make_shared<SpriteSheet>(flagSheet, 6, 1.0, false);
		sprites["raise"]->Pause();

		std::shared_ptr<ObjectActor> flag = std::make_shared<ObjectActor>(Vector2(checkPointPos[i].GetX(), checkPointPos[i].GetY()),
                                                                          *GameManager::GetInstance(),
                                                                          Vector2(0, 0),
                                                                          ObjectActor::flag,
                                                                          sprites,
                                                                          "raise",
                                                                          checkpointSounds);
		flag->SetNumericID(atoi(line.c_str()));
		level->AddActor(flag);
	}
}

void LevelLoader::LoadPancakes(ifstream & file, vector<SDL2pp::Point>& pancakePos, shared_ptr<Level> level)
{
    string line;
    std::shared_ptr<SDL2pp::Texture> pancakeSheet = std::make_shared<SDL2pp::Texture>(GameManager::GetInstance()->GetRenderer(), "./Assets/Textures/PancakeSheet.png");
    std::unordered_map<std::string, std::pair<std::string, bool>> pancakeSounds = {};
	for (size_t i = 0; i < pancakePos.size(); ++i)
	{
		std::getline(file, line);

		std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
		sprites["whateverPancakesDo"] = std::make_shared<SpriteSheet>(pancakeSheet, 8, 0.8f);

		std::shared_ptr<ObjectActor> collectible = std::make_shared<ObjectActor>(Vector2(pancakePos[i].GetX(), pancakePos[i].GetY()),
                                                                                 *GameManager::GetInstance(),
                                                                                 Vector2(0, 0),
                                                                                 ObjectActor::pancake,
                                                                                 sprites,
                                                                                 "whateverPancakesDo",
                                                                                 pancakeSounds);
		collectible->SetNumericID(atoi(line.c_str()));
		level->AddActor(collectible);
	}

	level->InitialzeNumberOfPancakes(pancakePos.size());
}

void LevelLoader::LoadHelpSigns(ifstream & file, vector<SDL2pp::Point>& signPos, shared_ptr<Level> level)
{
	string line;
    std::unordered_map<std::string, std::pair<std::string, bool>> helpSounds = {};
	for (size_t i = 0; i < signPos.size(); ++i)
	{
		std::getline(file, line);

		std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
		std::shared_ptr<SDL2pp::Texture> signSheet = std::make_shared<SDL2pp::Texture>(GameManager::GetInstance()->GetRenderer(), line);
		double infinity = std::numeric_limits<double>::infinity();
		sprites["idle"] = std::make_shared<SpriteSheet>(signSheet, 1, infinity);

		std::shared_ptr<ObjectActor> sign = std::make_shared<ObjectActor>(Vector2(signPos[i].GetX(), signPos[i].GetY()),
                                                                          *GameManager::GetInstance(),
                                                                          Vector2(0, 0),
                                                                          ObjectActor::tutorialSign,
                                                                          sprites,
                                                                          "idle",
                                                                          helpSounds);
		level->AddActor(sign);
	}
}

void LevelLoader::LoadHats(ifstream & file, shared_ptr<Level> level)
{
	string line;
	vector<string> tokens;
	for (int i = 0; i < 3; ++i)
	{
		std::getline(file, line);
		split(line, ' ', tokens);
		level->InsertHat(tokens[0], tokens[1] == "1" ? true : false);
		tokens.clear();
	}
}

void LevelLoader::LoadNPCS(ifstream & file, vector<SDL2pp::Point>& npcPos, shared_ptr<Level> level)
{
	string line;
	vector<string> tokens;

    std::unordered_map<std::string, std::pair<std::string, bool>> npcSounds;

	for (size_t i = 0; i < npcPos.size(); ++i)
	{
		std::getline(file, line);
		split(line, ' ', tokens);

		std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
		std::shared_ptr<SDL2pp::Texture> npcSheet = std::make_shared<SDL2pp::Texture>(GameManager::GetInstance()->GetRenderer(), tokens[0]);
		sprites["idle"] = std::make_shared<SpriteSheet>(npcSheet, atoi(tokens[1].c_str()), atof(tokens[2].c_str()));

		std::shared_ptr<NPCActor> npc = std::make_shared<NPCActor>(Vector2(npcPos[i].x, npcPos[i].y),
                                                                   *GameManager::GetInstance(),
                                                                   Vector2(0, 0),
                                                                   sprites,
                                                                   "idle",
                                                                   npcSounds,
                                                                   atoi(tokens[3].c_str()) ? SpriteSheet::XAxisDirection::LEFT : SpriteSheet::XAxisDirection::RIGHT);
		level->AddActor(npc);
		tokens.clear();
	}
}

void LevelLoader::LoadActorSpawners(std::ifstream & file, std::vector<SDL2pp::Point>& spawnerPos, std::shared_ptr<Level> level)
{
    vector<shared_ptr<ActorSpawner>> spawners;
    string line;
    for (int i = 0; i < spawnerPos.size(); i++)
    {
        // Skip any blank lines at the start
        do
        {
            getline(file, line);
            if(line.size() != 0) line.erase(std::remove(line.end() - 1, line.end(), '\r'), line.end());
        } while (line.size() == 0);

        // Read in requested period for spawner
        std::istringstream spawnPeriodStream(line);
        double period;
        spawnPeriodStream >> period;

        // Read in spawner simulataneous actor limit
        getline(file, line);
        line.erase(std::remove(line.end() - 1, line.end(), '\r'), line.end());
        std::istringstream limitStream(line);
        int limit;
        limitStream >> limit;

        // Hacky way to check whether we're specifying the actor prototype or copying another
        // This line will only ever have one character if we're specifying an actual prototype
        getline(file, line);
        line.erase(std::remove(line.end() - 1, line.end(), '\r'), line.end());
        if (line.size() == 1)
        {
            char enumVal = line[0];
            string serialised;
            
            // Get the serialised prototype
            while (getline(file, line) && line.size() > 0 && line != "\r")
                serialised.append(line).append("\n");

            switch (enumVal)
            {
            case Tile::enemy:
            {
                shared_ptr<Actor> enemy = shared_ptr<AIActor>(new AIActor(serialised));
                enemy->SetPosition(Vector2(spawnerPos[i].x, spawnerPos[i].y));
                shared_ptr<ActorSpawner> spawner = shared_ptr<ActorSpawner>(new ActorSpawner(level, enemy, period, limit));
                spawners.push_back(spawner);
            }
                break;
            case Tile::bombenemy:
            {
                shared_ptr<Actor> enemy = shared_ptr<AIActor>(new BombAIActor(serialised));
                enemy->SetPosition(Vector2(spawnerPos[i].x, spawnerPos[i].y));
                shared_ptr<ActorSpawner> spawner = shared_ptr<ActorSpawner>(new ActorSpawner(level, enemy, period, limit));
                spawners.push_back(spawner);
            }
                break;
            case Tile::npc:
            {
                shared_ptr<Actor> npc = shared_ptr<NPCActor>(new NPCActor(serialised));
                npc->SetPosition(Vector2(spawnerPos[i].x, spawnerPos[i].y));
                shared_ptr<ActorSpawner> spawner = shared_ptr<ActorSpawner>(new ActorSpawner(level, npc, period, limit));
                spawners.push_back(spawner);
            }
            }
        }
        else
        {
            // Determine which prototype to copy
            vector<string> splitLine = split(line, ' ');
            int toCopy;
            std::istringstream toCopyReader(splitLine[1]);
            toCopyReader >> toCopy;

            // Copy the prototype and create the spawner
            shared_ptr<Actor> prototype = shared_ptr<Actor>(spawners[toCopy]->GetPrototype()->Clone());
            prototype->SetPosition(Vector2(Vector2(spawnerPos[i].x, spawnerPos[i].y)));
            shared_ptr<ActorSpawner> spawner = shared_ptr<ActorSpawner>(new ActorSpawner(level, prototype, period, limit));
            spawners.push_back(spawner);
        }
    }

    for (auto & s : spawners)
        level->AddSpawner(s);
}

void LevelLoader::LoadTurrets(ifstream & file, vector<SDL2pp::Point>& turretPos, shared_ptr<Level> level)
{
    string line;
    std::unordered_map<std::string, std::pair<std::string, bool>> sounds = { {"turret", {"turret", false}} };
	for (size_t i = 0; i < turretPos.size(); ++i)
	{
		std::getline(file, line);

		std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
		std::shared_ptr<SDL2pp::Texture> turretSheet = std::make_shared<SDL2pp::Texture>(GameManager::GetInstance()->GetRenderer(), "./Assets/Textures/Turret.png");
		std::shared_ptr<SDL2pp::Texture> projectileSheet = std::make_shared<SDL2pp::Texture>(GameManager::GetInstance()->GetRenderer(), "./Assets/Textures/projectile.png");
		double infinity = std::numeric_limits<double>::infinity();
		sprites["turret"] = std::make_shared<SpriteSheet>(turretSheet, 1, infinity);
		sprites["shoot"] = std::make_shared<SpriteSheet>(projectileSheet, 1, infinity);

		std::shared_ptr<TurretActor> turret;
		if (line == "2")
			turret = std::make_shared<TurretActor>(
				Vector2(turretPos[i].GetX(), turretPos[i].GetY())
				, *GameManager::GetInstance()
				, Vector2(0, 0)
				, TurretActor::Aim::Player
				, false
				, 3.0
				, sprites
				, "turret"
                , sounds
				, SpriteSheet::XAxisDirection::RIGHT
				, SpriteSheet::YAxisDirection::UP);
		else
			turret = std::make_shared<TurretActor>(
				Vector2(turretPos[i].GetX(), turretPos[i].GetY())
				, *GameManager::GetInstance()
				, Vector2(0, 0)
				, sprites
				, "turret"
                , sounds
				, line == "0" ? SpriteSheet::XAxisDirection::LEFT : SpriteSheet::XAxisDirection::RIGHT
				, SpriteSheet::YAxisDirection::UP);
		level->AddActor(turret);
	}
}
