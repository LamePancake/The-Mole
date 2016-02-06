#pragma once
class Math
{
public:
	/**
	* Clamps value between minimum and maximum input.
	*
	* @param val = value to clamp
	* @param min = lower bound of the clamp
	* @param max = upper bound of the clamp
	*
	**/
	static float Clamp(float val, float min, float max)
	{
		float minVal;
		float maxVal;

		// Handles the case where max is smaller than min, people might be stupid.
		if (min > max)
		{
			minVal = max;
			maxVal = min;
		}
		else
		{
			minVal = min;
			maxVal = max;
		}

		if (val < min)
		{
			val = min;
		}
		if (val > max)
		{
			val = max;
		}
		return val;
	}
};