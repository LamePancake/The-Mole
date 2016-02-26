#pragma once
#include <vector>
#include <memory>
#include <map>

class Actor;
class AIActor;
class NPCActor;
class ObjectActor;
class BossActor;

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
	 * @brief	Gets tiles in the specified range.
	 * 			The range is exclusive, so (for example) to retrieve one tile, pass startRow = tile's row, endRow = tile's row + 1, startCol = tile's column, endCol = tile's column + 1.
	 *
	 * @author	Shane
	 * @date	2/5/2016
	 *
	 * @param	startRow   	The starting row.
	 * @param	endRow	   	The end row. Must be > startRow.
	 * @param	startCol   	The start column.
	 * @param	endCol	   	The end column. Must be > startCol.
	 * @param [in,out]	out	A vector containing the specified tile range as a flat array.
	 */
	void GetTileRange(int startRow, int endRow, int startCol, int endCol, std::vector<std::shared_ptr<Tile>>& out);

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
	void AddEnemy(std::shared_ptr<AIActor> e);

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
	 * Adds an npc to the npc array.
	 *
	 * @param	n	The std::shared_ptr&lt;SimpleAgent&gt; to process.
	 */
	void AddNPC(std::shared_ptr<NPCActor> n);

	/**
	 * Gets an NPC.
	 *
	 * @param	idx	The index into the npc array.
	 *
	 * @return	The npc.
	 */
	std::shared_ptr<Actor> GetNPC(size_t idx);

	/**
	 * Gets npc array size.
	 *
	 * @return	The npc size.
	 */
	size_t GetNPCSize() const;

	/**
	* Adds an boss to the boss array.
	*
	* @param	n	The std::shared_ptr&lt;BossActor&gt; to process.
	*/
	void Level::AddBoss(std::shared_ptr<BossActor> n);

	/**
	* Gets an boss.
	*
	* @param	idx	The index into the boss array.
	*
	* @return	The boss.
	*/
	std::shared_ptr<BossActor> Level::GetBoss();

	/**
	 * Adds an actor object to the object array.
	 *
	 * @param	o	The std::shared_ptr&lt;SimpleAgent&gt; to process.
	 */
	void AddActorObject(std::shared_ptr<ObjectActor> o);

	/**
	* Gets an Actor Object.
	*
	* @param	idx	The index into the object array.
	*
	* @return	The object.
	*/
	std::shared_ptr<Actor> GetActorObject(size_t idx);

	/**
	* Gets object array size.
	*
	* @return	The object size.
	*/
	size_t GetActorObjectSize() const;

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
	std::vector<std::shared_ptr<AIActor>> _enemies;

	/** The npcs in the level. */
	std::vector<std::shared_ptr<NPCActor>> _NPCs;

	/** Other actors in the level, like checkpoints */
	std::vector<std::shared_ptr<ObjectActor>> _objects;

	std::map<char, std::vector<SDL2pp::Point>> _tilePositions;

	/** The level. */
	std::vector<std::vector<std::shared_ptr<Tile>>> _level;

	/** The bawss. */
	std::shared_ptr<BossActor> _boss;

	/** Width of the tile. */
	size_t _tileWidth;

	/** height of the tile. */
	size_t _tileHeight;
};