#include "BossBehavTree.h"
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
//
//struct HeatStatus
//{
//	bool overheated;
//	int heat;
//};
//
//struct HealthStatus
//{
//	bool dead;
//	int health;
//};

class CheckIfOverheatedTask : public Node
{
private:
	int _btHeat;
public:
	CheckIfOverheatedTask(int heat) : _btHeat(heat){}
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

class CheckHealthTask : public Node
{
private:
	int _btHealth;
public:
	CheckHealthTask(int health) : _btHealth(health){}
	virtual bool run() override
	{
		if (_btHealth > 50)
		{
			cout << "phase 1" << endl;
			return true;
		}
		else
		{
			cout << "phase 2" << endl;
			return false;
		}
	}
};

class CheckIfPlayerCloseTask : public Node
{
private:
	float _btDist;
public:
	CheckIfPlayerCloseTask(float dist) : _btDist(dist) {}
	virtual bool run() override
	{
		if (_btDist < 5)
		{
			cout << "close" << endl;
			return true;
		}
		else
		{
			cout << "far" << endl;
			return false;
		}
	}
};

class OverheatedTask : public Node
{
private:
	bool _btOverheated;
public:
	OverheatedTask(bool overheated) : _btOverheated(overheated) {}
	virtual bool run() override
	{
		if (_btOverheated)
		{
			cout << "#rekt, cooling down" << endl;
			return true;
		}
		else
		{
			cout << "still going" << endl;
			return false;
		}
	}	 
};

class PunchTask : public Node
{
private:
	bool _btClose;
public:
	PunchTask(bool close) : _btClose(close) {}
	virtual bool run() override
	{
		if (_btClose)
		{
			cout << "punch" << endl;
			return true;
		}
		else
		{
			cout << "not punching" << endl;
			return false;
		}
	}
};

void BossBehavTree::UpdateTree()
{

}

void BossBehavTree::UpdateVariables(Vector2 pPos, Vector2 bPos, int health, int heat)
{
	_pDist = CalculateDist(pPos, bPos);
	_health = health;
	_heat = heat;
}

