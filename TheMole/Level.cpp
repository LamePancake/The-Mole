#include "Level.h"

void Level::AddBlank(std::shared_ptr<Tile> tile)
{
	_blankTiles.push_back(std::move(tile));
}

void Level::AddDirt(std::shared_ptr<Tile> tile)
{
	_dirtTiles.push_back(std::move(tile));
}

void Level::AddMetal(std::shared_ptr<Tile> tile)
{
	_metalTiles.push_back(std::move(tile));
}

void Level::AddSpike(std::shared_ptr<Tile> tile)
{
	_spikeTiles.push_back(std::move(tile));
}

void Level::AddStone(std::shared_ptr<Tile> tile)
{
	_stoneTiles.push_back(std::move(tile));
}

int Level::AddTileToLevel(std::shared_ptr<Tile> tile, int row)
{
	if (row >= _level.size())
	{
		_level.push_back(std::vector<std::shared_ptr<Tile>>());
	}

	_level[row].push_back(std::move(tile));
	return 1;
}

int Level::GetBlankSize() const
{
	return _blankTiles.size();
}

int Level::GetDirtSize() const
{
	return _dirtTiles.size();
}

int Level::GetMetalSize() const
{
	return _metalTiles.size();
}

int Level::GetSpikeSize() const
{
	return _spikeTiles.size();
}

int Level::GetStoneSize() const
{
	return _stoneTiles.size();
}

SDL2pp::Point& Level::GetLevelSize() const
{
	return SDL2pp::Point(_level[0].size(), _level.size());
}

std::shared_ptr<Tile> Level::GetBlankTile(int & idx)
{

	return idx >= _blankTiles.size() ? nullptr : _blankTiles[idx];
}

std::shared_ptr<Tile> Level::GetDirtTile(int & idx)
{
	return idx >= _dirtTiles.size() ? nullptr : _dirtTiles[idx];
}

std::shared_ptr<Tile> Level::GetMetalTile(int & idx)
{
	return idx >= _metalTiles.size() ? nullptr : _metalTiles[idx];
}

std::shared_ptr<Tile> Level::GetSpikeTile(int & idx)
{
	return idx >= _spikeTiles.size() ? nullptr : _spikeTiles[idx];
}

std::shared_ptr<Tile> Level::GetStoneTile(int & idx)
{
	return idx >= _stoneTiles.size() ? nullptr : _stoneTiles[idx];
}

std::shared_ptr<Tile> Level::GetTileFromLevel(int & x, int & y)
{
	if (y >= _level.size() || x > _level[y].size())
		return nullptr;
	return _level[y][x];
}
