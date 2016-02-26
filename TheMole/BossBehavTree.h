#pragma once
#include <list>
#include <iostream>
#include "Vector2.h"
using namespace std;

class Node
{
public:
	virtual bool run() = 0;
};

class CompositeNode : public Node
{
private:
	list<Node*> children;
public:
	const list<Node*>& getChildren() const
	{
		return children;
	}
	void addChild(Node* child)
	{
		children.emplace_back(child);
	}
};

class Selector : public CompositeNode
{
public:
	virtual bool run() override
	{
		for (Node* child : getChildren())
		{
			if (child->run())
			{
				return true;
			}
			return false;
		}
	}
};

class Sequence : public CompositeNode
{
public:
	virtual bool run() override
	{
		for (Node* child : getChildren())
		{
			if (!child->run())
			{
				return false;
			}
			return true;
		}
	}
};

class CheckHeatTask : public Node
{
private:
	int _btHeat;
public:
	CheckHeatTask(int heat) : _btHeat(heat) {}
	virtual bool run() override
	{
		if (_btHeat < 100)
		{
			cout << "overheated" << endl;
			return true;
		}
		else
		{
			cout << "not quite heated" << endl;
			return false;
		}
	}
};

class CheckAliveTask : public Node
{
private:
	int _btHealth;
public:
	CheckAliveTask(int health) : _btHealth(health) {}
	virtual bool run() override
	{
		if (_btHealth > 0)
		{
			cout << "alive" << endl;
			return true;
		}
		else
		{			
			return false;
		}
	}
};

class CheckDeadTask : public Node
{
private:
	int _btHealth;
public:
	CheckDeadTask(int health) : _btHealth(health) {}
	virtual bool run() override
	{
		if (_btHealth <= 0)
		{
			cout << "dead" << endl;
			return true;
		}
		else
		{
			return false;
		}
	}
};

class CheckIfOverheatedTask : public Node
{
private:
	int _btHeat;
public:
	CheckIfOverheatedTask(int heat) : _btHeat(heat) {}
	virtual bool run() override
	{
		if (_btHeat >= 100)
		{
			cout << "overheated" << endl;
			return true;
		}
		else
		{
			cout << "not quite heated" << endl;
			return false;
		}
	}
};

class PrePunchTask : public Node
{
private:
	float _btDist;
public:
	PrePunchTask(float dist) : _btDist(dist) {}
	virtual bool run() override
	{
		if (_btDist < 10)
		{
			cout << "close enough" << endl;
			return true;
		}
		else
		{
			cout << "far" << endl;
			return false;
		}
	}
};

class PunchTask : public Node
{
private:

public:
	PunchTask() {}
	virtual bool run() override
	{
		cout << "close" << endl;
		return true;
	}
};

class PreRollTask : public Node
{
private:
	float _btDist;
public:
	PreRollTask(float dist) : _btDist(dist) {}
	virtual bool run() override
	{
		if (_btDist > 10)
		{
			cout << "far enough" << endl;
			return true;
		}
		else
		{
			cout << "close" << endl;
			return false;
		}
	}
};

class RollTask : public Node
{
private:

public:
	RollTask() {}
	virtual bool run() override
	{
		cout << "roll" << endl;
		return true;
	}
};

class ShortHopTask : public Node
{
private:

public:
	ShortHopTask() {}
	virtual bool run() override
	{
		cout << "hop" << endl;
		return true;
	}
};

class ShockWaveTask : public Node
{
private:

public:
	ShockWaveTask() {}
	virtual bool run() override
	{
		cout << "wave" << endl;
		return true;
	}
};

class IdleTask : public Node
{
private:

public:
	IdleTask() {}
	virtual bool run() override
	{
		cout << "cooldown" << endl;
		return true;
	}
};

class EjectTask : public Node
{
private:

public:
	EjectTask() {}
	virtual bool run() override
	{
		cout << "eject" << endl;
		return true;
	}
};

class BossBehavTree
{
public:
	BossBehavTree();

	~BossBehavTree();
	void ExecuteTree();
	void UpdateVariables(Vector2 pPos, Vector2 bPos, int health, int heat);
private:
	float CalculateDist(Vector2 pPos, Vector2 bPos)
	{
		return sqrt(pow((pPos._x - bPos._x), 2) + pow((pPos._y - bPos._y), 2));
	}
	float _pDist;
	int _health;
	int _heat;
	CheckIfOverheatedTask* _tChkOverheat;
	CheckHeatTask* _tChkHeat;
	CheckAliveTask* _tChkAlive;
	CheckDeadTask* _tChkDead;
	PrePunchTask* _tPrePunch;
	PunchTask* _tPunch;
	PreRollTask* _tPreRoll;
	RollTask* _tRoll;
	ShortHopTask* _tShortHop;
	ShockWaveTask* _tShockWave;
	IdleTask* _tIdle;
	EjectTask* _tEject;

	Selector *_root;
	Selector *_selAlive;
	Sequence *_seqDead;
	Sequence *_seq1Close;
	Sequence *_seq1Far;
	Sequence *_seqOverHeat;
};