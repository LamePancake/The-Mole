#include "Level.h"
#include "GameScreen.h"
#include "Actor.h"
#include "AIActor.h"
#include "NPCActor.h"
#include "ObjectActor.h"
#include "ProjectileActor.h"

int Level::AddTileToLevel(std::shared_ptr<Tile> tile, size_t row)
{
	if (row >= _level.size())
	{
		_level.push_back(std::vector<std::shared_ptr<Tile>>());
	}

	_level[row].push_back(std::move(tile));
	return 1;
}

SDL2pp::Point Level::GetLevelSize() const
{
	return _level.size() > 0 ? SDL2pp::Point(_level[0].size(), _level.size()) : SDL2pp::Point(0, 0);
}

std::shared_ptr<Tile> Level::GetTileFromLevel(size_t x, size_t y)
{
	if (y >= _level.size() || x > _level[y].size())
		return nullptr;

	return _level[y][x];
}

void Level::GetTileRange(int startRow, int endRow, int startCol, int endCol, std::vector<std::shared_ptr<Tile>>& out) {
    for (int row = startRow; row < endRow; ++row) {
		for (int col = startCol; col < endCol; ++col) {
			out.push_back(_level[row][col]);
		}
	}
}

void Level::AddPositions(char key, SDL2pp::Point position)
{
	_tilePositions[key].push_back(position);
}

int Level::GetNumPositions(char key)
{
	return _tilePositions.size();
}

SDL2pp::Point Level::GetPosition(char key, size_t idx)
{
	return _tilePositions[key][idx];
}

void Level::AddEnemy(std::shared_ptr<AIActor> e)
{
	_enemies.push_back(e);
}

std::shared_ptr<Actor> Level::GetEnemy(size_t idx)
{
	if (idx > _enemies.size())
		return nullptr;

	return _enemies[idx];
}

size_t Level::GetEnemySize() const
{
	return _enemies.size();
}

void Level::AddEnemySpawn(Vector2 e)
{
	_enemySpawns.push_back(e);
}

Vector2 Level::GetEnemySpawn(size_t idx)
{
	if (idx > _enemySpawns.size())
		return Vector2(0, 0);

	return _enemySpawns[idx];
}

void Level::AddNPC(std::shared_ptr<NPCActor> n)
{
	_NPCs.push_back(n);
}

std::shared_ptr<Actor> Level::GetNPC(size_t idx)
{
	if (idx > _NPCs.size())
		return nullptr;

	return _NPCs[idx];
}

void Level::AddBoss(std::shared_ptr<BossActor> n)
{
	_boss = n;
}

std::shared_ptr<BossActor> Level::GetBoss()
{
	return _boss;
}

size_t Level::GetNPCSize() const
{
	return _NPCs.size();
}

void Level::AddActorObject(std::shared_ptr<ObjectActor> o)
{
	_objects.push_back(o);
}

std::shared_ptr<Actor> Level::GetActorObject(size_t idx)
{
	if (idx > _objects.size())
		return nullptr;

	return _objects[idx];
}

void Level::AddProjectileObject(std::shared_ptr<ProjectileActor> prj)
{
	_projectiles.push_back(prj);
}

size_t Level::GetProjectileActorSize() const
{
	return _projectiles.size();
}

std::shared_ptr<ProjectileActor> Level::GetProjectile(size_t idx)
{
	if (idx > _projectiles.size())
		return nullptr;

	return _projectiles[idx];
}


size_t Level::GetActorObjectSize() const
{
	return _objects.size();
}

size_t Level::GetTileWidth() const
{
	return _tileWidth;
}

void Level::SetTileWidth(size_t width)
{
	_tileWidth = width;
}

size_t Level::GetTileHeight() const
{
	return _tileHeight;
}

void Level::SetTileHeight(size_t height)
{
	_tileHeight = height;
}

void Level::AddDugTile(std::shared_ptr<Tile> t)
{
	_dugDirtTiles.insert(std::make_pair(t, 0));
}

void Level::UpdateDugTiles(double deltaTime)
{
	int tileWidth  = GetTileWidth();
	int tileHeight = GetTileHeight();
	std::shared_ptr<PlayerActor> player = std::dynamic_pointer_cast<GameScreen>(GameManager::GetInstance()->GetCurrentScreen())->GetPlayer();

	float leftPlayerBound   = player->GetPosition().GetX();
	float topPlayerBound    = player->GetPosition().GetY();
	float rightPlayerBound  = player->GetPosition().GetX() + player->GetTexture()->GetFrameWidth();
	float bottomPlayerBound = player->GetPosition().GetY() + player->GetTexture()->GetFrameHeight();

	int leftPlayerIdx   = ((int)leftPlayerBound / tileWidth);
	int topPlayerIdx    = ((int)topPlayerBound / tileHeight);
	int rightPlayerIdx  = ((int)rightPlayerBound / tileWidth);
	int bottomPlayerIdx = ((int)bottomPlayerBound / tileHeight);

	for (auto it = _dugDirtTiles.begin(); it != _dugDirtTiles.end(); )
	{
		it->second += deltaTime;

		if (it->second > 7)
		{
			bool isOccupied = false;
			SDL2pp::Point tileIdx = it->first->GetIndices();

			if ((leftPlayerIdx  == tileIdx.x && topPlayerIdx   == tileIdx.y) ||
				(rightPlayerIdx == tileIdx.x && bottomPlayerIdx == tileIdx.y))
			{
				isOccupied = true;
			}

			if (!isOccupied)
			{
				for (std::shared_ptr<AIActor> e : _enemies)
				{
					float leftEnemyBound  = e->GetPosition().GetX();
					float topEnemyBound   = e->GetPosition().GetY();
					float rightEnemyBound = e->GetPosition().GetX() + e->GetTexture()->GetFrameWidth();
					float botomEnemyBound = e->GetPosition().GetY() + e->GetTexture()->GetFrameHeight();

					int leftEnemyIdx   = ((int)leftEnemyBound / tileWidth);
					int topEnemyIdx    = ((int)topEnemyBound / tileHeight);
					int rightEnemyIdx  = ((int)rightEnemyBound / tileWidth);
					int bottomEnemyIdx = ((int)botomEnemyBound / tileHeight);

					if ((leftEnemyIdx  == tileIdx.x && topEnemyIdx   == tileIdx.y) ||
						(rightEnemyIdx == tileIdx.x && bottomEnemyIdx == tileIdx.y))
					{
						isOccupied = true;
						break;
					}
				}
			}

			if (!isOccupied)
			{
				std::shared_ptr<Tile> t = it->first;
				_dugDirtTiles.erase(it++);
				t->SetID(Tile::dirt);
				continue;
			}
		}
		++it;
	}
}

void Level::Update(double deltaTime)
{
	UpdateDugTiles(deltaTime);
}

void Level::Reset()
{
	for (size_t i = 0; i < _enemies.size(); ++i)
	{
		_enemies[i]->Reset(_enemySpawns[i]);
	}

	for (auto it = _dugDirtTiles.begin(); it != _dugDirtTiles.end(); ++it)
	{
		it->first->SetID(Tile::dirt);
	}
	_dugDirtTiles.clear();
}

void Level::SetSpawnPoint(Vector2 point)
{
	_currentSpawnPoint = point;
}

Vector2 Level::GetSpawnPoint()
{
	return _currentSpawnPoint;
}

