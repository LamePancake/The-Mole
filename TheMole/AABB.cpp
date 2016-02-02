#include "AABB.h"



AABB::AABB()
{
}


AABB::~AABB()
{
}

AABB::AABB(SDL2pp::Texture srcImg, SimpleAgent ai)
{
	// This sets _x and _y as the top left corner of the box
	_x = ai.GetPosition().GetX() - (srcImg.GetWidth() / 2);
	_y = ai.GetPosition().GetY() - (srcImg.GetHeight() / 2);

	_width = srcImg.GetWidth();
	_height = srcImg.GetHeight();

}

bool AABB::CheckCollision(AABB otherBox)
{
	if (_x < otherBox.GetX() + otherBox.GetWidth() &&
		_x + _width > otherBox.GetX() &&
		_y < otherBox.GetY() + otherBox.GetHeight() &&
		_y + _height > otherBox.GetY())
	{
		// we collided
		return true;
	}
	else
	{
		return false;
	}
}

void AABB::UpdatePosition(SimpleAgent ai)
{
	_x = ai.GetPosition().GetX() - (_width / 2);
	_y = ai.GetPosition().GetY() - (_height / 2);
}

float AABB::GetX()
{
	return _x;
}

float AABB::GetY()
{
	return _y;
}

float AABB::GetWidth()
{
	return _width;
}

float AABB::GetHeight()
{
	return _height;
}