#ifndef AABB_H
#define AABB_H

#pragma once
#include <SDL2pp\SDL2pp.hh>
#include "Vector2.h"
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
	AABB(double width, double height, Actor & actor);

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
	float GetX();

	/**
	* Getter for _y of this AABB
	*
	* @return _y, which is a float
	*/
	float GetY();

	/**
	* Getter for _width of this AABB
	*
	* @return _width, which is a float
	*/
	float GetWidth();

	/**
	* Getter for _height of this AABB
	*
	* @return _height, which is a float
	*/
	float GetHeight();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets the amount of overlap between this AABB and the other.</summary>
	///
	///<remarks> The overlap in a given direction can be added to an actor's position to push them out of
	///          the overlap. For instance, if this actor is on the right of another actor, the overlap
	///          will be a positive value which will push the actor to the right of the other actor.</remarks>
	///
	///<param name="other">        The other AABB to check against.</param>
	///<param name="makeIntegral"> Whether to convert the overlap values to integers (maximised in the
	///                            appropriate direction.</param>
	///
	///<returns> The amount of overlap between the two boxes.</returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Vector2 GetOverlap(AABB other, bool makeIntegral) const;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Checks whether this AABB contains the other in either axis.</summary>
	///
	///<param name="other"> The other AABB to check against.</param>
	///<param name="xAxis"> [out] Whether this AABB contains the other one in the x axis.</param>
	///<param name="yAxis"> [out] Whether this AABB contains the other one in the y axis.</param>
	///
	///<returns> true if the actors are colliding.</returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void Contains(AABB other, bool & xAxis, bool & yAxis) const;

private:
	float _x;
	float _y;
	float _width;
	float _height;
};

#endif