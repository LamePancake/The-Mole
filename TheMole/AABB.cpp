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
	_x = actor.GetPosition().GetX();
	_y = actor.GetPosition().GetY();

	_width = width;
	_height = height;

}

bool AABB::CheckCollision(AABB otherBox)
{
    //
	//     b0--------b1
	//      |        |
	// a0---|----a1  |
	//  |   |    |   |
	//  |  b3--------b2
	//  |        |
	//  |        |
	// a3--------a2
    //
	// (a0.x is less than b1.x and a1.x is greater than b0.x) collided in x axis
	// (a0.y is less than b3.y and a3.y is greater than b0.y) collided in y axis
	//
	if ((_x < otherBox.GetX() + otherBox.GetWidth() &&  _x + _width > otherBox.GetX()) &&
		(_y < otherBox.GetY() + otherBox.GetHeight() && _y + _height > otherBox.GetY()))
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
	_x = actor.GetPosition().GetX();
	_y = actor.GetPosition().GetY();
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

Vector2 AABB::GetOverlap(AABB other, bool makeIntegral) const
{
	Vector2 overlap;
	const AABB *left = _x < other._x ? this : &other;
	const AABB *right = left == this ? &other : this;
	overlap.SetX(left->_x + left->_width - right->_x);
	if (left == this) overlap.SetX(overlap.GetX() * -1);
	
	const AABB *top = _y < other._y ? this : &other;
	const AABB *bottom = top == this ? &other : this;
	overlap.SetY(top->_y + top->_height - bottom->_y);
	if (top == this) overlap.SetY(overlap.GetY() * -1);

	if (makeIntegral)
	{
		float x = overlap.GetX();
		float y = overlap.GetY();
		x = x < 0 ? ::floorf(x) : ::ceilf(x);
		y = y < 0 ? ::floorf(y) : ::ceilf(y);
		overlap.SetX(x);
		overlap.SetY(y);
	}

	return overlap;
}

void AABB::Contains(AABB other, bool & xAxis, bool & yAxis) const
{
	xAxis = _x < other._x && (other._x + other._width < _x + _width);
	yAxis = _x < other._y && (other._y + other._height < _y + _height);
}
