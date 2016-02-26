#include "Level.h"
#include "GameScreen.h"
#include "Actor.h"
#include "AIActor.h"
#include "NPCActor.h"
#include "ObjectActor.h"
#include "ProjectileActor.h"

int Level::AddTileToLevel(std::shared_ptr<Tile> tile, int row)
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
	_dugDirt.push_back(t);
	_dugTimers.push_back(0);
}

void Level::UpdateDugTile(double deltaTime)
{
	int tileWidth = GetTileWidth();
	int tileHeight = GetTileHeight();
	std::shared_ptr<PlayerActor> player = std::dynamic_pointer_cast<GameScreen>(GameManager::GetInstance()->GetCurrentScreen())->GetPlayer();

	float leftPlayerBound = player->GetPosition()._x;
	float topPlayerBound = player->GetPosition()._y;
	float rightPlayerBound = player->GetPosition()._x + player->GetTexture()->GetFrameWidth();
	float bottomPlayerBound = player->GetPosition()._y + player->GetTexture()->GetFrameHeight();

	int leftPlayerIdx = ((int)leftPlayerBound / tileWidth);
	int topPlayerIdx = ((int)topPlayerBound / tileHeight);
	int rightPlayerIdx = ((int)rightPlayerBound / tileWidth);
	int bottomPlayerIdx = ((int)bottomPlayerBound / tileHeight);

	for (size_t i = 0; i < _dugDirt.size(); ++i)
	{
		_dugTimers[i] += deltaTime;

		if (_dugTimers[i] > 10)
		{
			bool isOccupied = false;
			SDL2pp::Point tileIdx = _dugDirt[i]->GetIndices();

			if ((leftPlayerIdx == tileIdx.x && topPlayerIdx == tileIdx.y) ||
				(rightPlayerIdx == tileIdx.x && bottomPlayerIdx == tileIdx.y))
			{
				isOccupied = true;
			}

			if (!isOccupied)
			{
				for (std::shared_ptr<AIActor> e : _enemies)
				{
					float leftEnemyBound = e->GetPosition()._x;
					float topEnemyBound = e->GetPosition()._y;
					float rightEnemyBound = e->GetPosition()._x + e->GetTexture()->GetFrameWidth();
					float botomEnemyBound = e->GetPosition()._y + e->GetTexture()->GetFrameHeight();

					int leftEnemyIdx = leftEnemyBound / tileWidth;
					int topEnemyIdx = topEnemyBound / tileHeight;
					int rightEnemyIdx = rightEnemyBound / tileWidth;
					int bottomEnemyIdx = botomEnemyBound / tileHeight;

					if ((leftEnemyIdx == tileIdx.x && topEnemyIdx == tileIdx.y) ||
						(rightEnemyIdx == tileIdx.x && bottomEnemyIdx == tileIdx.y))
					{
						isOccupied = true;
						break;
					}
				}
			}

			if (isOccupied)
			{
				continue;
			}
			else
			{
				std::shared_ptr<Tile> t = _dugDirt[i];
				_dugDirt.erase(_dugDirt.begin() + i);
				_dugTimers.erase(_dugTimers.begin() + i);
				t->SetID(Tile::dirt);
			}
		}
	}
}
