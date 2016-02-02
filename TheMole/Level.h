#pragma once
#include <vector>
#include <memory>
#include <tuple>

#include "Tiles\Tile.h"


/** Stores data for a level. It may be overkill, so chief refactorer may refactor if he wants to. */
class Level
{
public:

	/** Default constructor. */
	Level(){}

	/**
	 * Adds a blank tile.
	 *
	 * @param	tile	The blank tile.
	 */
	void AddBlank(std::shared_ptr<Tile> tile);

	/**
	 * Adds a dirt tile.
	 *
	 * @param	tile	The dirt tile.
	 */
	void AddDirt(std::shared_ptr<Tile> tile);
	
	/**
	* Adds a metal tile.
	*
	* @param	tile	The metal tile.
	*/
	void AddMetal(std::shared_ptr<Tile> tile);
	
	/**
	* Adds a spike tile.
	*
	* @param	tile	The spike tile.
	*/
	void AddSpike(std::shared_ptr<Tile> tile);
	
	/**
	* Adds a stone tile.
	*
	* @param	tile	The stone tile.
	*/
	void AddStone(std::shared_ptr<Tile> tile);

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
	 * Gets blank tiles array size.
	 *
	 * @return	The blank tile array size.
	 */
	int GetBlankSize() const;

	/**
	* Gets dirt tiles array size.
	*
	* @return	The dirt tile array size.
	*/
	int GetDirtSize() const;
	
	/**
	* Gets metal tiles array size.
	*
	* @return	The metal tile array size.
	*/
	int GetMetalSize() const;
	
	/**
	* Gets spike tiles array size.
	*
	* @return	The spike tile array size.
	*/
	int GetSpikeSize() const;
	
	/**
	* Gets stone tiles array size.
	*
	* @return	The stone tile array size.
	*/
	int GetStoneSize() const;

	/**
	* Gets level  array size.
	*
	* @return	The level array size.
	*/
	SDL2pp::Point GetLevelSize() const;

	/**
	 * Gets a particular blank tile.
	 *
	 * @param [in,out]	idx	The index into the array.
	 *
	 * @return	The blank tile at that index.
	 */
	std::shared_ptr<Tile> GetBlankTile(size_t idx);
	
	/**
	* Gets a particular dirt tile.
	*
	* @param [in,out]	idx	The index into the array.
	*
	* @return	The dirt tile at that index.
	*/
	std::shared_ptr<Tile> GetDirtTile(size_t idx);
	
	/**
	* Gets a particular metal tile.
	*
	* @param [in,out]	idx	The index into the array.
	*
	* @return	The metal tile at that index.
	*/
	std::shared_ptr<Tile> GetMetalTile(size_t idx);
	
	/**
	* Gets a particular spike tile.
	*
	* @param [in,out]	idx	The index into the array.
	*
	* @return	The spike tile at that index.
	*/
	std::shared_ptr<Tile> GetSpikeTile(size_t idx);
	
	/**
	* Gets a particular stone tile.
	*
	* @param [in,out]	idx	The index into the array.
	*
	* @return	The stone tile at that index.
	*/
	std::shared_ptr<Tile> GetStoneTile(size_t idx);

	/**
	* Gets a particular tile from the level.
	*
	* @param [in,out]	idx	The index into the level.
	*
	* @return	The level tile at that index.
	*/
	std::shared_ptr<Tile> GetTileFromLevel(size_t x, size_t y);

private:

	/** The blank tiles. Cached for rendering purposes*/
	std::vector<std::shared_ptr<Tile>> _blankTiles;

	/** The dirt tiles. Cached for rendering purposes*/
	std::vector<std::shared_ptr<Tile>> _dirtTiles;

	/** The metal tiles. Cached for rendering purposes*/
	std::vector<std::shared_ptr<Tile>> _metalTiles;

	/** The spike tiles. Cached for rendering purposes*/
	std::vector<std::shared_ptr<Tile>> _spikeTiles;

	/** The stone tiles. Cached for rendering purposes*/
	std::vector<std::shared_ptr<Tile>> _stoneTiles;

	/** The level. */
	std::vector<std::vector<std::shared_ptr<Tile>>> _level;
};