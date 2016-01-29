#include "LevelRenderer.h"

void LevelRenderer::Load(GameManager & manager)
{
	_mgr = &manager;

	_blank = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_dirt.png");
	_dirt  = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_dirt.png");
	_metal = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_dirt.png");
	_spike = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_spike.png");
	_stone = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_stone.png");
}

void LevelRenderer::Unload()
{ 
	delete _blank;
	delete _dirt;
	delete _metal;
	delete _spike;
	delete _stone;
}

void LevelRenderer::RenderLevel(std::shared_ptr<Level> level)
{
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	SDL2pp::Window& window = _mgr->GetWindow();
	SDL2pp::Point levelSize = level->GetLevelSize();

	SDL2pp::Point tempPoint;
	SDL2pp::Rect tempRect;

	//HARDCODED, SHOULD BE CHANGED AND NOT NECESSARY IN FINAL GAME, USED FOR TESTING ONLY IN ORDER TO VISUALIZE THE WHOLE MAP
	int xScale = window.GetWidth() / levelSize.x;
	int yScale = window.GetHeight() / levelSize.y;

	if (levelSize.x == 0 || levelSize.y == 0)
		return;

	if (level->GetDirtSize() != 0)
	{
		for (int i = 0; i < level->GetDirtSize(); i++)
		{
			//tempPoint = level->GetDirtTile(i)->GetWorldPosition();
			tempPoint = level->GetDirtTile(i)->GetIndices();
			tempRect = SDL2pp::Rect(tempPoint.x * xScale, tempPoint.y * yScale, xScale, yScale);
			
			rend.Copy(*_dirt, SDL2pp::NullOpt, tempRect);
		}
	}

	if (level->GetStoneSize() != 0)
	{
		for (int i = 0; i < level->GetStoneSize(); i++)
		{
			//tempPoint = level->GetStoneTile(i)->GetWorldPosition();
			tempPoint = level->GetStoneTile(i)->GetIndices();
			tempRect = SDL2pp::Rect(tempPoint.x * xScale, tempPoint.y * yScale, xScale, yScale);

			rend.Copy(*_stone, SDL2pp::NullOpt, tempRect);
		}
	}

	if (level->GetSpikeSize() != 0)
	{
		for (int i = 0; i < level->GetSpikeSize(); i++)
		{
			//tempPoint = level->GetSpikeTile(i)->GetWorldPosition();
			tempPoint = level->GetSpikeTile(i)->GetIndices();
			tempRect = SDL2pp::Rect(tempPoint.x * xScale, tempPoint.y * yScale, xScale, yScale);

			rend.Copy(*_spike, SDL2pp::NullOpt, tempRect);
		}
	}

	if (level->GetMetalSize() != 0)
	{
		for (int i = 0; i < level->GetMetalSize(); i++)
		{
			//tempPoint = level->GetMetalTile(i)->GetWorldPosition();
			tempPoint = level->GetMetalTile(i)->GetIndices();
			tempRect = SDL2pp::Rect(tempPoint.x * xScale, tempPoint.y * yScale, xScale, yScale);

			rend.Copy(*_metal, SDL2pp::NullOpt, tempRect);
		}
	}
}
