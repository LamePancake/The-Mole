#pragma once
#include <list>
#include <iostream>
#include "Vector2.h"

using namespace std;

//Vector2 _targetPos;
//Vector2 _playerPos;
//Vector2 _bossPos;

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
	const list<Node*>& getChildren() const;
	void addChild(Node* child);
};

class Selector : public CompositeNode
{
public:
	virtual bool run() override;
};

class Sequence : public CompositeNode
{
public:
	virtual bool run() override;
};

class CheckHeatTask : public Node
{
private:
	int _btHeat;
public:
	CheckHeatTask(int heat) : _btHeat(heat) {}
	virtual bool run() override;
};

class CheckAliveTask : public Node
{
private:
	int _btHealth;
public:
	CheckAliveTask(int health) : _btHealth(health) {}
	virtual bool run() override;
};

class CheckDeadTask : public Node
{
private:
	int _btHealth;
public:
	CheckDeadTask(int health) : _btHealth(health) {}
	virtual bool run() override;
};

class CheckIfOverheatedTask : public Node
{
private:
	int _btHeat;
public:
	CheckIfOverheatedTask(int heat) : _btHeat(heat) {}
	virtual bool run() override;
};

class PrePunchTask : public Node
{
private:
	float &_btDist;
	float _triggerRange;
public:
	PrePunchTask(float &dist, float _range) : _btDist(dist), _triggerRange(_range) {}
	virtual bool run() override;
};

class PunchTask : public Node
{
private:

public:
	PunchTask() {}
	virtual bool run() override;
};

class PreRollTask : public Node
{
private:
	float &_btDist;
	float _triggerRange;
public:
	PreRollTask(float &dist, float _range) : _btDist(dist), _triggerRange(_range) {}
	virtual bool run() override;
};

class RollTask : public Node
{
private:

public:
	RollTask() {}
	virtual bool run() override;
};

class ShortHopTask : public Node
{
private:

public:
	ShortHopTask() {}
	virtual bool run() override;
};

class ShockWaveTask : public Node
{
private:

public:
	ShockWaveTask() {}
	virtual bool run() override;
};

class IdleTask : public Node
{
private:

public:
	IdleTask() {}
	virtual bool run() override;
};

class EjectTask : public Node
{
private:

public:
	EjectTask() {}
	virtual bool run() override;
};

class BossBehavTree
{
public:
	BossBehavTree();

	void ExecuteTree();
	void UpdateVariables(Vector2* pPos, Vector2* bPos, int health, int heat);
	//Vector2 GetTarget();
private:
	float _pDist;
	int _health;
	int _heat;
	float _meleeRange;

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