#pragma once
#include <list>
#include <iostream>
#include <memory>
#include "Vector2.h"

using namespace std;
class GameScreen;
class BossActor;

class Node
{
public:
	Node();
	virtual bool run(double elapsedSecs) = 0;
	std::shared_ptr<GameScreen> _gameScreen;
private:

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
	virtual bool run(double elapsedSecs) override;
};

class Sequence : public CompositeNode
{
public:
	virtual bool run(double elapsedSecs) override;
};

class CheckHeatTask : public Node
{
private:
	int _btHeat;
public:
	CheckHeatTask(int heat) : _btHeat(heat) {}
	virtual bool run(double elapsedSecs) override;
};

class CheckAliveTask : public Node
{
private:
	int _btHealth;
public:
	CheckAliveTask(int health) : _btHealth(health) {}
	virtual bool run(double elapsedSecs) override;
};

class CheckDeadTask : public Node
{
private:
	int _btHealth;
public:
	CheckDeadTask(int health) : _btHealth(health) {}
	virtual bool run(double elapsedSecs) override;
};

class CheckIfOverheatedTask : public Node
{
private:
	int _btHeat;
public:
	CheckIfOverheatedTask(int heat) : _btHeat(heat) {}
	virtual bool run(double elapsedSecs) override;
};

class PrePunchTask : public Node
{
private:
	float &_btDist;
	float _triggerRange;
public:
	PrePunchTask(float &dist, float _range) : _btDist(dist), _triggerRange(_range) {}
	virtual bool run(double elapsedSecs) override;
};

class PunchTask : public Node
{
private:
	Vector2* _targetPos;
public:
	PunchTask(Vector2 *tPos) : _targetPos(tPos) {}
	virtual bool run(double elapsedSecs) override;
};

class PreRollTask : public Node
{
private:
	float &_btDist;
	float _triggerRange;
public:
	PreRollTask(float &dist, float _range) : _btDist(dist), _triggerRange(_range) {}
	virtual bool run(double elapsedSecs) override;
};

class RollTask : public Node
{
private:
	Vector2* _targetPos;
public:
	RollTask(Vector2 *tPos) : _targetPos(tPos) {}
	virtual bool run(double elapsedSecs) override;	
};

class ShortHopTask : public Node
{
private:

public:
	ShortHopTask() {}
	virtual bool run(double elapsedSecs) override;
};

class ShockWaveTask : public Node
{
private:
	Vector2* _targetPos;
public:
	ShockWaveTask(Vector2 *tPos) : _targetPos(tPos) {}
	virtual bool run(double elapsedSecs) override;
};

class IdleTask : public Node
{
private:
	Vector2* _targetPos;
public:
	IdleTask(Vector2 *tPos) : _targetPos(tPos) {}
	virtual bool run(double elapsedSecs) override;
};

class EjectTask : public Node
{
private:

public:
	EjectTask() {}
	virtual bool run(double elapsedSecs) override;
};

class BossBehavTree
{
public:
	BossBehavTree();

	void ExecuteTree();
	void UpdateVariables(Vector2* pPos, Vector2* bPos, int health, int heat, double elapsedSecs);
	Vector2 GetTarget();
private:
	double _elapsedSecs;
	double _idleDur;
	double _rollDur;
	float _pDist;
	int _health;
	int _heat;
	float _meleeRange;

	Vector2 _targetPos;
	Vector2 _playerPos;
	Vector2 _bossPos;

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