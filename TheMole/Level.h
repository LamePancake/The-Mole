#pragma once
#include <vector>
#include <memory>
#include <tuple>

#include "Tile.h"


/** Stores data for a level. It may be overkill, so chief refactorer may refactor if he wants to. */
class Level
{
public:

	/** Default constructor. */
	Level(){}

	/**
	 * Adds a tile to the level.
	 *
	 * @param	tile	The tile to be added to the level.
	 * @param	row 	The row.
	 *
	 * @return	An 0 if row is out of bounds, 1 otherwise.
	 */
	int AddTileToLevel(std::shared_ptr<Tile> tile, int row);

	/**
	* Gets level  array size.
	*
	* @return	The level array size.
	*/
	SDL2pp::Point GetLevelSize() const;

	/**
	* Gets a particular tile from the level.
	*
	* @param [in,out]	idx	The index into the level.
	*
	* @return	The level tile at that index.
	*/
	std::shared_ptr<Tile> GetTileFromLevel(size_t x, size_t y);

private:

	/** The level. */
	std::vector<std::vector<std::shared_ptr<Tile>>> _level;
};