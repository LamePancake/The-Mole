#include "Tile.h"

void Tile::Draw()
{
	//Drawing code goes here
}

void Tile::Update(float deltaTime)
{
	//Update code goes here
}

int const Tile::GetWidth()
{
	return _width; 
}

int const Tile::GetHeight() 
{
	return _height; 
}

void Tile::SetWidth(int width)
{
	_width = width;
}

void Tile::SetHeight(int height)
{
	_height = height;
}

std::pair<int, int> Tile::GetIndices()
{
	return std::pair<int, int>(_xIndex, _yIndex);
}

void Tile::SetIndices(int x, int y)
{
	_xIndex = x;
	_yIndex = y;
}

std::pair<int, int> Tile::GetWorldPosition()
{
	return std::pair<int, int>(_xIndex * _width, _yIndex * _height);
}

