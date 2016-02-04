#include "LevelRenderer.h"
#include <iostream>

void LevelRenderer::Load(GameManager & manager)
{
	_mgr = &manager;

	_tileTextures[Tile::dirt]        = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_dirt.png");
	_tileTextures[Tile::metal]       = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_dirt.png");
	_tileTextures[Tile::spike]       = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_spike.png");
	_tileTextures[Tile::stone]       = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_stone.png");
	//_tileTextures[Tile::collectible] = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_spike.png");
	//_tileTextures[Tile::enemy]       = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_spike.png");
	//_tileTextures[Tile::goal]        = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_spike.png");
	//_tileTextures[Tile::blank]       = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_dirt.png");
}

void LevelRenderer::Unload()
{ 
	for (auto text : _tileTextures)
	{
		delete text.second;
	}
}

void LevelRenderer::RenderLevel(std::shared_ptr<Level> level)
{
	SDL2pp::Renderer& rend  = _mgr->GetRenderer();
	SDL2pp::Window& window  = _mgr->GetWindow();
	SDL2pp::Point levelSize = level->GetLevelSize();

	SDL2pp::Point tempPoint;
	SDL2pp::Rect tempRect;

	//HARDCODED, SHOULD BE CHANGED AND NOT NECESSARY IN FINAL GAME, USED FOR TESTING ONLY IN ORDER TO VISUALIZE THE WHOLE MAP
	int xScale = window.GetWidth() / levelSize.x;
	int yScale = window.GetHeight() / levelSize.y;

	if (levelSize.x == 0 || levelSize.y == 0)
		return;

	for (int height = 0; height < levelSize.y; ++height)
	{
		for (int width = 0; width < levelSize.x; ++width)
		{
			std::shared_ptr<Tile> tempTile = level->GetTileFromLevel(width, height);
			char id = tempTile->GetID();

			if (id == Tile::blank || id == Tile::enemy || id == Tile::goal || id == Tile::collectible)
				continue;

			//tempPoint = tempTile->GetWorldPosition();
			tempPoint = tempTile->GetIndices();
			tempRect = SDL2pp::Rect(tempPoint.x * xScale, tempPoint.y * yScale, xScale, yScale);
			rend.Copy(*_tileTextures[tempTile->GetID()], SDL2pp::NullOpt, tempRect);
		}
	}
}
