#include "Level.h"
#include "Actor.h"
#include "AIActor.h"
#include "NPCActor.h"
#include "ObjectActor.h"

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

const std::vector<std::shared_ptr<AIActor>>& Level::GetEnemies() const
{
	return _enemies;
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
