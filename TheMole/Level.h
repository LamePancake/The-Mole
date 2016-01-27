#pragma once
#include <vector>
#include "Tiles\Tile.h"

class Level
{
public:
	Level() {};

private:
	std::vector<Tile*> _blankTiles;
	std::vector<Tile*> _dirtTiles;
	std::vector<Tile*> _metalTiles;
	std::vector<Tile*> _spikeTiles;
	std::vector<Tile*> _stoneTiles;

	std::vector<std::vector<Tile*>> _level;
};