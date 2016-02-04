#pragma once
#include <vector>
#include <memory>
#include <map>

class Actor;

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

	/**
	 * Stores the positions of a particular type of tile
	 *
	 * @param	key			The key.
	 * @param	position	The position.
	 */
	void AddPositions(char key, SDL2pp::Point position);

	/**
	 * Gets number positions stored for a particular type of tile.
	 *
	 * @param	key	The key.
	 *
	 * @return	The number positions.
	 */
	int GetNumPositions(char key);

	/**
	 * Gets a position for a particular type of tile.
	 *
	 * @param	key	The key.
	 * @param	idx	The index.
	 *
	 * @return	The position.
	 */
	SDL2pp::Point GetPosition(char key, size_t idx);

	/**
	 * Adds an enemy to the enemy array.
	 *
	 * @param	e	The std::shared_ptr&lt;SimpleAgent&gt; to process.
	 */
	void AddEnemy(std::shared_ptr<Actor> e);

	/**
	 * Gets an enemy.
	 *
	 * @param	idx	The index into the enemy array.
	 *
	 * @return	The enemy.
	 */
	std::shared_ptr<Actor> GetEnemy(size_t idx);

	/**
	 * Gets enemy array size.
	 *
	 * @return	The enemy size.
	 */
	size_t GetEnemySize() const;

	/**
	 * Gets tile width.
	 *
	 * @return	The tile width.
	 */
	size_t GetTileWidth() const;

	/**
	 * Sets tile width.
	 *
	 * @param	width	The width.
	 */
	void SetTileWidth(size_t width);

	/**
	* Gets tile height.
	*
	* @return	The tile height.
	*/
	size_t GetTileHeight() const;

	/**
	* Sets tile Height.
	*
	* @param	height	The height.
	*/
	void SetTileHeight(size_t height);

private:
	
	/** The enemies in the level. */
	std::vector<std::shared_ptr<Actor>> _enemies;

	std::map<char, std::vector<SDL2pp::Point>> _tilePositions;

	/** The level. */
	std::vector<std::vector<std::shared_ptr<Tile>>> _level;

	/** Width of the tile. */
	size_t _tileWidth;

	/** height of the tile. */
	size_t _tileHeight;
};