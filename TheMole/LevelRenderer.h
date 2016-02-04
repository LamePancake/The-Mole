#pragma once
#include "Level.h"
#include "GameManager.h"
#include <SDL2pp\SDL2pp.hh>
#include <map>
#include <memory>


/** Renders a given level */
class LevelRenderer
{
public:

	/** Default constructor. */
	LevelRenderer() {}

	/**
	 * Loads in the textures needed to render the level.
	 *
	 * @param [in,out]	mgr	The manager needed to load textures.
	 */
	void Load(GameManager & mgr);


	/** Unloads this the texutres stored in this object. */
	void Unload();

	/**
	 * Renders the level described by level.
	 *
	 * @param	level   	The level to be rendered.
	 */
	void RenderLevel(std::shared_ptr<Level> level);

private:

	std::map<char, SDL2pp::Texture*> _tileTextures;

	/** Reference to the game manager. */
	GameManager* _mgr;
};