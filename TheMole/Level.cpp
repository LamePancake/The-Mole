#include "Level.h"
#include "GameScreen.h"
#include "Actor.h"
#include "AIActor.h"
#include "NPCActor.h"
#include "ObjectActor.h"
#include "ProjectileActor.h"
#include "BossActor.h"

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

std::shared_ptr<Tile> Level::GetNeighbourTile(std::shared_ptr<Tile> tile, Actor::Edge edge)
{
	SDL2pp::Point indices = tile->GetIndices();
	switch (edge)
	{
	case Actor::Edge::BOTTOM:
		return GetTileFromLevel(indices.x, indices.y + 1);
	case Actor::Edge::TOP:
		return GetTileFromLevel(indices.x, indices.y - 1);
	case Actor::Edge::RIGHT:
		return GetTileFromLevel(indices.x + 1, indices.y);
	case Actor::Edge::LEFT:
		return GetTileFromLevel(indices.x - 1, indices.y);
	}
}

bool Level::HasNeighbourTile(std::shared_ptr<Tile> tile, Actor::Edge edge) const
{
	SDL2pp::Point indices = tile->GetIndices();
	switch (edge)
	{
	case Actor::Edge::BOTTOM:
		return indices.y < _level.size() - 1;
	case Actor::Edge::TOP:
		return indices.y > 0;
	case Actor::Edge::RIGHT:
		return indices.x < _level[0].size() - 1;
	case Actor::Edge::LEFT:
		return indices.x > 0;
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

std::shared_ptr<BossActor> Level::GetBoss()
{
	return _boss;
}

void Level::AddActor(std::shared_ptr<Actor> actor)
{
	// Hack, but if we don't do this then a huge chunk of Tim's code will have unwanted dependencies
	// Solve later if possible
	if (actor->GetType() == Actor::Type::boss)
	{
		_boss = dynamic_pointer_cast<BossActor>(actor);
	}

	_actorCounts[actor->GetType()]++;

	_actorsToAdd.push_back(actor);
}

std::vector<std::shared_ptr<Actor>>& Level::GetActors()
{
	return _actors;
}

size_t Level::GetNumberOfActors(Actor::Type type)
{
	return _actorCounts[type];
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
				it->second = 0;
				isOccupied = true;
			}

			if (!isOccupied)
			{
				for (auto actor : _actors)
				{
					if (actor->GetType() != Actor::Type::enemy) continue;
					shared_ptr<AIActor> e = dynamic_pointer_cast<AIActor>(actor);

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
						it->second = 0;
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

	// Add any actors added during the update
	_actors.insert(_actors.end(), _actorsToAdd.begin(), _actorsToAdd.end());
	_actorsToAdd.clear();

	if (_actors.empty()) return;

	// Clean up all destroyed actors
	auto destroyedFinder = [](shared_ptr<Actor> a) {return a->IsDestroyed(); };
	_actors.erase(std::remove_if(_actors.begin(), _actors.end(), destroyedFinder), _actors.end());
}

void Level::Reset()
{
	size_t enemyNum = 0;
	for (auto actor : _actors)
	{
		Actor::Type type = actor->GetType();
		switch (type)
		{
		case Actor::Type::player:
			actor->Reset(GetSpawnPoint());
			break;
		case Actor::Type::enemy:
			actor->Reset(_enemySpawns[enemyNum++]);
			break;
		default:
			actor->Reset(actor->GetPosition());
			break;
		}
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

