#pragma once
#include "Level.h"
#include "GameManager.h"
#include <SDL2pp\SDL2pp.hh>
#include <vector>
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
	 * @param [in,out]	level	The level to be rendered.
	 */
	void RenderLevel(Level & level);

private:

	/** The blank tile texture. */
	SDL2pp::Texture* _blank;

	/** The dirt tile texture. */
	SDL2pp::Texture* _dirt;

	/** The metal tile texture. */
	SDL2pp::Texture* _metal;

	/** The spike tile texture. */
	SDL2pp::Texture* _spike;

	/** The stone tile texture. */
	SDL2pp::Texture* _stone;

	/** Reference to the game manager. */
	GameManager* _mgr;
};