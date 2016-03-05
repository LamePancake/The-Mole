#pragma once
#include "Vector2.h"

Vector2::Vector2()
{
}

Vector2::~Vector2()
{
}

Vector2::Vector2(float x, float y)
{
	_x = x;
	_y = y;
}

float Vector2::GetX() const
{
	return _x;
}

float Vector2::GetY() const
{
	return _y;
}

void Vector2::SetX(float  x)
{
	_x = x;
}

void Vector2::SetY(float y)
{
	_y = y;
}

Vector2 Vector2::operator-(Vector2 other)
{
	return Vector2(_x - other.GetX(), _y - other.GetY());
}

float Vector2::Distance(Vector2 other)
{
	float x = _x - other._x;
	float y = _y - other._y;
	
	x *= x;
	y *= y;

	return std::sqrtf(x + y);
}
