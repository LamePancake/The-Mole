#include "Tile.h"

void Tile::Draw()
{
	//Drawing code goes here
}

void Tile::Update(float deltaTime)
{
	//Update code goes here
}

size_t const Tile::GetWidth() const
{
	return _width; 
}

size_t const Tile::GetHeight() const
{
	return _height; 
}

void Tile::SetWidth(size_t width)
{
	_width = width;
}

void Tile::SetHeight(size_t height)
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

Vector2 Tile::GetWorldPosition()
{
	return Vector2(_xIndex * _width, _yIndex * _height);
}

char Tile::GetID() const
{
	return _id;
}

void Tile::SetID(char id)
{
	_id = id;
}

