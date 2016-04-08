#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#include "Tile.h"
#include "Level.h"
#include "PlayerActor.h"
#include "NPCActor.h"

/** Loads a level from a text file and returns a vector of tiles that represent the level. */
class LevelLoader
{
public:

	/** Default Constructor. */
	LevelLoader() {};

	/**
	 * Loads in the level and returns an array of tiles.
	 *
	 * @param	levelPath	Full pathname of the level file.
	 * @param	player   	The player, gets assigned to in the load.
	 *
	 * @return	null if it fails, else the level.
	 */
	std::shared_ptr<Level> LoadLevel(std::string levelPath, std::shared_ptr<PlayerActor> & player, bool den = false);

private:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Loads actors whose tile doesn't fully describe them.</summary>
	///
	///<remarks> This method loads switches, doors, etc. which have additional data not easily conveyed
	///          by a single letter (e.g. which switches open which doors).</remarks>
	///
	///<param name="file">      The map file input stream.</param>
	///<param name="lastLine">  The most recently read line from the file.</param>
	///<param name="positions"> A map of tile letters to the tiles' respective positions.</param>
	///<param name="level">     The level into which to load the actors.</param>
    ///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void LoadActorSpecifics(std::ifstream& file, std::string& lastLine, std::unordered_map<char, std::vector<SDL2pp::Point>>& positions, std::shared_ptr<Level> level);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Creates weight pads and doors described in the level's metadata and adds them to the level.</summary>
	///
	///<param name="file">      The map file input stream.</param>
	///<param name="togglePos"> The list of tiles containing toggles.</param>
	///<param name="doorPos">   The list of tiles containing doors.</param>
	///<param name="level">     The level into which to load the actors.</param>
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void LoadTogglesAndDoors(std::ifstream& file, std::vector<SDL2pp::Point> & togglePos, std::vector<SDL2pp::Point> & doorPos, std::shared_ptr<Level> level);

	void LoadDialog(std::ifstream& file, std::vector<SDL2pp::Point> & dialogPos, std::shared_ptr<Level> level);

	void LoadCheckPoints(std::ifstream& file, std::vector<SDL2pp::Point> & checkPointPos, std::shared_ptr<Level> level);

	void LoadPancakes(std::ifstream& file, std::vector<SDL2pp::Point>& pancakePos, std::shared_ptr<Level> level);

	void LoadHelpSigns(std::ifstream& file, std::vector<SDL2pp::Point>& signPos, std::shared_ptr<Level> level);

	void LoadHats(std::ifstream& file, std::shared_ptr<Level> level);

	void LoadNPCS(std::ifstream& file, std::vector<SDL2pp::Point>& npcPos, std::shared_ptr<Level> level);
};