#include "Level.h"

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
	return SDL2pp::Point(_level[0].size(), _level.size());
}

std::shared_ptr<Tile> Level::GetTileFromLevel(size_t x, size_t y)
{
	if (y >= _level.size() || x > _level[y].size())
		return nullptr;
	return _level[y][x];
}

void Level::AddPosiition(char key, SDL2pp::Point position)
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
