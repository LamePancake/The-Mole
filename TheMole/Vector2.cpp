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

float Vector2::GetX()
{
	return _x;
}

float Vector2::GetY()
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
