#pragma once
#include <cmath>

/**
* Class that holds X,Y position in floats.
**/
class Vector2
{
public:
	Vector2();
	~Vector2();
	/**
	* Constructor with x and y initialized by param values.
	*
	*@param x and y are initial values of vector
	*/
	Vector2(float x, float y);

	/**
	* Returns x component of vector
	*
	*@return _x, a float
	*/
	float GetX() const;

	/**
	* Returns y component of vector
	*
	*@return _y, a float
	*/
	float GetY() const;

	/**
	* Sets x component of vector
	*
	*@param new x value, in float
	*/
	void SetX(float x);
	
	/**
	* Sets y component of vector
	*
	*@param new y value, in float
	*/
	void SetY(float y);

	Vector2 operator-(Vector2 other);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Gets the absolute distance to another vector.</summary>
	/// 
	///<remarks> Shane, 2/22/2016.</remarks>
	///
	///<param name="other">The other vector to which to compute the distance.</param>
	///<returns>The distance to the other vector.</returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	float Distance(Vector2 other);

private:
	float _x;
	float _y;
};