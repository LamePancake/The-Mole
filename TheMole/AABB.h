#ifndef AABB_H
#define AABB_H

#pragma once
#include <SDL2pp\SDL2pp.hh>
class Actor;

class AABB
{
public:
	
	AABB();
	~AABB();
	/**
	* Creates Axis Aligned Bounding Box from source image.
	*
	* @param source image to create AABB from, SimpleAgent object to which we base our x, y position on
	**/
	AABB(SDL2pp::Texture & srcImg, Actor & actor);

	/**
	* Checks collision against another AABB
	*
	* @param the AABB to check against
	*
	* @return true if hit, false if no hit
	**/
	bool CheckCollision(AABB otherBox);

	/**
	* Updates the AABB's position (x, y) to those of the Agent's position
	*
	* @param the SimpleAgent object we update our position to
	**/
	void UpdatePosition(Actor & actor);

	/**
	* Getter for _x of this AABB
	*
	* @return _x, which is a float
	*/
	float AABB::GetX();

	/**
	* Getter for _y of this AABB
	*
	* @return _y, which is a float
	*/
	float AABB::GetY();

	/**
	* Getter for _width of this AABB
	*
	* @return _width, which is a float
	*/
	float AABB::GetWidth();

	/**
	* Getter for _height of this AABB
	*
	* @return _height, which is a float
	*/
	float AABB::GetHeight();



private:
	float _x;
	float _y;
	float _width;
	float _height;
};

#endif