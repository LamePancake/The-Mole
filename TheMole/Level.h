#pragma once
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include "Actor.h"
#include "Tile.h"

class BossActor;

/** Stores data for a level. It may be overkill, so chief refactorer may refactor if he wants to. */
class Level
{
public:

	/** Default constructor. */
	Level(): _currentSpawnPoint(0, 0), _checkPointID(-1), _availableHats(), _hitCheckpoint(false) {}

	/**
	 * Adds a tile to the level.
	 *
	 * @param	tile	The tile to be added to the level.
	 * @param	row 	The row.
	 *
	 * @return	An 0 if row is out of bounds, 1 otherwise.
	 */
	int AddTileToLevel(std::shared_ptr<Tile> tile, size_t row);

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

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets the neighbouring tile in the given direction.</summary>
	///
	///<remarks> This method assumes that the neighbour is within bounds. Use HasNeighbourTile to check
	///          before calling this method.</remarks>
	///
	///<param name="tile"> The tile for which to retrieve a neighbour.</param>
	///<param name="edge"> The edge in which to get the neighbour.</param>
	///
	///<returns> The neighbour on the given tile edge.</returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	std::shared_ptr<Tile> GetNeighbourTile(std::shared_ptr<Tile> tile, Actor::Edge edge);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Determines whether the given tile has a neighbour on the given edge.</summary>
	///
	///<param name="tile"> The tile for which to check for a neighbour.</param>
	///<param name="edge"> The edge on which to check.</param>
	///
	///<returns> True if the tile has a neighbour, false otherwise.</returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	bool HasNeighbourTile(std::shared_ptr<Tile> tile, Actor::Edge edge) const;

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
	* Gets an boss.
	*
	* @param	idx	The index into the boss array.
	*
	* @return	The boss.
	*/
	std::shared_ptr<BossActor> Level::GetBoss();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Adds an actor to the level.</summary>
	///
	///<param name="actor"> The actor to add.</param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddActor(std::shared_ptr<Actor> actor);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets the list of all actors.</summary>
	///
	///<returns> The list of actors in the level.</returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<std::shared_ptr<Actor>> & GetActors();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets the list of actors of the specified type.</summary>
	///
	///<remarks> Note that this involves a linear search of the list of actors on each invocation; don't
	///          use it unless necessary.</remarks>
	///
	///<param name="T"> The type of actor to be retrieved. The shared_ptr's will be cast to contain
	///                 this type of actor.</param>
	///<param name="type"> The enum value corresponding to the type of actor to retrieve.</param>
	///
	///<returns> The list of actors of the given type.</returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	std::vector<std::shared_ptr<T>> GetActorsOfType(Actor::Type type);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets the list of actors of the specified type.</summary>
	///
	///<remarks> Note that this involves a linear search of the list of actors on each invocation; don't
	///          use it unless necessary.</remarks>
	///
	///<param name="T"> The type of actor to be retrieved. The shared_ptr's will be cast to contain
	///                 this type of actor.</param>
	///<param name="type"> The enum value corresponding to the type of actor to retrieve.</param>
	///<param name="list"> A list into which the actors will be pushed.</param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	void GetActorsOfType(Actor::Type type, std::vector<std::shared_ptr<T>> &list);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets the number of actors of the given type in the level.</summary>
    ///
	///<param name="type"> The enum value corresponding to the type of actor.</param>
	///
	///<returns> The number of actors of the specified type in the level.</returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	size_t GetNumberOfActors(Actor::Type type);

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

	/**
	* Stores a tile that was dug.
	*
	* @param	t	The tile that was dug.
	*/
	void AddDugTile(std::shared_ptr<Tile> t);

	/**
	* Updates objects for the level
	*
	* @param	deltaTime	Time since last frame.
	*/
	void Update(double deltaTime);

	void Reset();

	void SetSpawnPoint(Vector2 point, int id);

	Vector2 GetSpawnPoint();

	void InitialzeNumberOfPancakes(int numPancakes);

	void CollectPancake(int id);

	std::vector<bool> GetPancakes();

	void InsertHat(std::string hat, bool available);

	bool IsHatAvailable(std::string hat);

private:
	
	/** Stores the tiles that have been dug and need to be respawned. */
	std::map<std::shared_ptr<Tile>, double> _dugDirtTiles;

	/** Stores the spawn point of the enemies */
	std::vector<Vector2> _enemySpawns;

	/** Stores the list of every actor in the level. */
	std::vector<std::shared_ptr<Actor>> _actors;

	/** Stores a list of actors to add at the end of the current update. */
	std::vector<std::shared_ptr<Actor>> _actorsToAdd;

	/** Stores the number of each different type of actor. */
	std::unordered_map<Actor::Type, size_t> _actorCounts;

	std::map<char, std::vector<SDL2pp::Point>> _tilePositions;

	/** The level. */
	std::vector<std::vector<std::shared_ptr<Tile>>> _level;

	/** The bawss. */
	std::shared_ptr<BossActor> _boss;

	/** Width of the tile. */
	size_t _tileWidth;

	/** height of the tile. */
	size_t _tileHeight;

	Vector2 _currentSpawnPoint;

	int _checkPointID;

	std::vector<bool> _collectedPancakes;

	std::map<std::string, bool> _availableHats;

    bool _hitCheckpoint;

	/**
	* Updates the dug tiles to see if they can be respaned.
	*
	* @param	deltaTime	Time since last frame.
	*/
	void UpdateDugTiles(double deltaTime);
};

template<typename T>
inline std::vector<std::shared_ptr<T>> Level::GetActorsOfType(Actor::Type type)
{
	std::vector<std::shared_ptr<T>> actors;
	actors.reserve(GetNumberOfActors(type));
	GetActorsOfType(type, actors);
	return actors;
}

template<typename T>
inline void Level::GetActorsOfType(Actor::Type type, std::vector<std::shared_ptr<T>>& list)
{
	for (auto actor : _actors)
	{
		if (actor->GetType() == type)
		{
			std::shared_ptr<T> typedActor = dynamic_pointer_cast<T>(actor);
			list.push_back(typedActor);
		}
	}
}
