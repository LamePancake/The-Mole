#pragma once
#include <list>
#include <iostream>
#include "Vector2.h"

class BossBehavTree
{
public:
	BossBehavTree();
	~BossBehavTree();
	void UpdateTree();
	void UpdateVariables(Vector2 pPos, Vector2 bPos, int health, int heat)
	{
		_pDist = CalculateDist(pPos, bPos);
		_health = health;
		_heat = heat;
	}
private:
	float CalculateDist(Vector2 pPos, Vector2 bPos)
	{
		return sqrt(pow((pPos._x - bPos._x), 2) + pow((pPos._y - bPos._y), 2));
	}
	float _pDist;
	int _health;
	int _heat;
};