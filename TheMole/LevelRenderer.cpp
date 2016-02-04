#include "LevelRenderer.h"

void LevelRenderer::Load(GameManager & manager)
{
	_mgr = &manager;

	_tileTextures[Tile::dirt]        = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_dirt.png");
	_tileTextures[Tile::metal]       = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_dirt.png");
	_tileTextures[Tile::spike]       = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_spike.png");
	_tileTextures[Tile::stone]       = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_stone.png");
	_tileTextures[Tile::collectible] = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\Pancake.png");
	_tileTextures[Tile::goal]        = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\Toad.png");
	_tileTextures[Tile::tunnel]      = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\mineShaft.png");

	_shadowTileTextures[Tile::dirt]        = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_dirt.png");
	_shadowTileTextures[Tile::metal]       = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_dirt.png");
	_shadowTileTextures[Tile::spike]       = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_spike.png");
	_shadowTileTextures[Tile::stone]       = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\block_stone.png");
	_shadowTileTextures[Tile::collectible] = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\Pancake.png");
	_shadowTileTextures[Tile::goal]        = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\Toad.png");
	_shadowTileTextures[Tile::tunnel]      = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\mineShaft.png");

	for (auto it : _shadowTileTextures)
	{
		SDL_SetTextureColorMod(it.second->Get(), 120, 120, 120);
		SDL_SetTextureAlphaMod(it.second->Get(), 127);
	}
}

void LevelRenderer::Unload()
{ 
	for (auto text : _tileTextures)
	{
		delete text.second;
	}

	for (auto text : _shadowTileTextures)
	{
		delete text.second;
	}
}

void LevelRenderer::RenderLevel(std::shared_ptr<Level> level, Camera& camera)
{
	const SDL2pp::Rect& viewport = camera.GetViewport();
	int offsetX = 5;
	int offsetY = 5;

	SDL2pp::Renderer& rend  = _mgr->GetRenderer();
	SDL2pp::Point levelSize = level->GetLevelSize();

	SDL2pp::Point tempPoint;

	if (levelSize.x == 0 || levelSize.y == 0)
		return;

	for (int height = 0; height < levelSize.y; ++height)
	{
		for (int width = 0; width < levelSize.x; ++width)
		{
			std::shared_ptr<Tile> tempTile = level->GetTileFromLevel(width, height);
			char id = tempTile->GetID();

			if (id == Tile::blank || id == Tile::enemy)
				continue;

			tempPoint = { (int)tempTile->GetWorldPosition().GetX(), (int)tempTile->GetWorldPosition().GetY() };

			//Render shadow
			rend.Copy(*_shadowTileTextures[id], SDL2pp::NullOpt, tempPoint + SDL2pp::Point(offsetX - viewport.x, offsetY - viewport.y));
		
			//Render normal
			rend.Copy(*_tileTextures[id], SDL2pp::NullOpt, tempPoint + SDL2pp::Point(-viewport.x, -viewport.y));
		}
	}
}
