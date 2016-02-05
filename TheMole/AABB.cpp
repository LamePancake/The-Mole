#include "AABB.h"
#include "Actor.h"

AABB::AABB()
{
}


AABB::~AABB()
{
}

AABB::AABB(double width, double height, Actor & actor)
{
	// This sets _x and _y as the top left corner of the box
	_x = actor.GetPosition().GetX() - (width / 2);
	_y = actor.GetPosition().GetY() - (height / 2);

	_width = width;
	_height = height;

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

void AABB::UpdatePosition(Actor & actor)
{
	_x = actor.GetPosition().GetX() - (_width / 2);
	_y = actor.GetPosition().GetY() - (_height / 2);
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