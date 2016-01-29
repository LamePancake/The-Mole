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

void LevelRenderer::RenderLevel(Level & level)
{
	//Render level here
}
