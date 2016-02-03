#pragma once
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
	float GetX();

	/**
	* Returns y component of vector
	*
	*@return _y, a float
	*/
	float GetY();

	/**
	* Sets x component of vector
	*
	*@param new x value, in float
	*/
	void SetX(float  x);
	
	/**
	* Sets y component of vector
	*
	*@param new y value, in float
	*/
	void SetY(float y);
private:
	float _x;
	float _y;
};