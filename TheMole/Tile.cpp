#include "Tile.h"

void Tile::Draw()
{
	//Drawing code goes here
}

void Tile::Update(float deltaTime)
{
	//Update code goes here
}

int const Tile::GetWidth() const
{
	return _width; 
}

int const Tile::GetHeight() const
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

SDL2pp::Point Tile::GetIndices() const
{
	return SDL2pp::Point(_xIndex, _yIndex);
}

void Tile::SetIndices(int x, int y)
{
	_xIndex = x;
	_yIndex = y;
}

SDL2pp::Point Tile::GetWorldPosition()
{
	return SDL2pp::Point(_xIndex * _width, _yIndex * _height);
}

char Tile::GetID() const
{
	return _id;
}

void Tile::SetID(char id)
{
	_id = id;
}
