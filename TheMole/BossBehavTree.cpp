#include "BossBehavTree.h"
#include "GameScreen.h"

Node::Node() : _gameScreen(std::dynamic_pointer_cast<GameScreen>(GameManager::GetInstance()->GetCurrentScreen()))
{
}

const list<Node*>& CompositeNode::getChildren() const
{
	return children;
}
void CompositeNode::addChild(Node* child)
{
	children.emplace_back(child);
}



bool Selector::run() 
{
	for (Node* child : getChildren())
	{
		if (child->run())
		{
			return true;
		}

	}
	return false;
}


bool Sequence::run() 
{
	for (Node* child : getChildren())
	{
		if (!child->run())
		{
			return false;
		}

	}
	return true;
}

bool CheckHeatTask::run() 
{
	if (_btHeat < 100)
	{
		//cout << "not overheated" << endl;
		return true;
	}
	else
	{
		//cout << "too heated" << endl;
		return false;
	}
}

bool CheckAliveTask::run() 
{
	if (_btHealth > 0)
	{
		//cout << "alive" << endl;
		return true;
	}
	else
	{
		return false;
	}
}


bool CheckDeadTask::run() 
{
	if (_btHealth <= 0)
	{
		//cout << "dead" << endl;
		return true;
	}
	else
	{
		return false;
	}
}



bool CheckIfOverheatedTask::run() 
{
	if (_btHeat >= 100)
	{
		//cout << "overheated" << endl;
		return true;
	}
	else
	{
		//cout << "not quite heated" << endl;
		return false;
	}
}



bool PrePunchTask::run() 
{
	if (_btDist < _triggerRange)
	{
		//cout << "close enough" << endl;
		return true;
	}
	else
	{
		//cout << "not close enough to punch" << endl;
		return false;
	}
}



bool PunchTask::run() 
{
	cout << "punch" << endl;
	return true;
}



bool PreRollTask::run() 
{
	if (_btDist > _triggerRange)
	{
		//cout << "far enough, pre roll" << endl;
		return true;
	}
	else
	{
		//cout << "not far enough to roll" << endl;
		return false;
	}
}



bool RollTask::run() 
{
	cout << "roll" << endl;
	targetPos = _gameScreen->GetPlayer()->GetPosition();
	//cout << "bPos: " << targetPos->GetX() << endl;
	//cout << "pPos: " << _gameScreen->GetPlayer()->GetPosition().GetX() << endl;
	return true;
}



bool ShortHopTask::run() 
{
	//cout << "hop" << endl;
	return true;
}



bool ShockWaveTask::run() 
{
	//cout << "wave" << endl;
	return true;
}



bool IdleTask::run() 
{
	cout << "cooldown" << endl;
	//_targetPos = _bossPos;
	return true;
}


bool EjectTask::run() 
{
	cout << "eject" << endl;
	return true;
}


BossBehavTree::BossBehavTree()
{
	_heat = 0;
	_health = 100;
	_meleeRange = 200;
	//Initialize Sequences and Selectors
	_root = new Selector;
	_selAlive = new Selector;
	_seqDead = new Sequence;
	_seq1Close = new Sequence;
	_seq1Far = new Sequence;
	_seqOverHeat = new Sequence;

	//Initialize Tasks
	_tChkOverheat = new CheckIfOverheatedTask(_heat);
	_tChkHeat = new CheckHeatTask(_heat);
	_tChkAlive = new CheckAliveTask(_health);
	_tChkDead = new CheckDeadTask(_health);
	_tPrePunch = new PrePunchTask(_pDist, _meleeRange);
	_tPunch = new PunchTask();
	_tPreRoll = new PreRollTask(_pDist, _meleeRange);
	_tRoll = new RollTask(&_targetPos);
	_tShortHop = new ShortHopTask();
	_tShockWave = new ShockWaveTask();
	_tIdle = new IdleTask();
	_tEject = new EjectTask();

	//Add alive/dead condition nodes
	_root->addChild(_selAlive);
	_root->addChild(_seqDead);

	//Add alive sequences;
	_selAlive->addChild(_seq1Close);
	_selAlive->addChild(_seq1Far);
	_selAlive->addChild(_seqOverHeat);

	//Add all alive tasks
	_seq1Close->addChild(_tChkHeat);
	_seq1Close->addChild(_tChkAlive);
	_seq1Close->addChild(_tPrePunch);
	_seq1Close->addChild(_tPunch);
	_seq1Close->addChild(_tIdle);

	_seq1Far->addChild(_tChkHeat);
	_seq1Far->addChild(_tChkAlive);
	_seq1Far->addChild(_tPreRoll);
	_seq1Far->addChild(_tRoll);
	_seq1Far->addChild(_tShortHop);
	_seq1Far->addChild(_tShockWave);
	_seq1Far->addChild(_tPreRoll);

	_seqOverHeat->addChild(_tChkOverheat);
	_seqOverHeat->addChild(_tChkAlive);
	_seqOverHeat->addChild(_tIdle);

	//Add dead tasks
	_seqDead->addChild(_tEject);
}

void BossBehavTree::ExecuteTree()
{
	while (!_root->run())
	{
		cout << "---------------------" << endl;
	}
	//cout << "Done Tree Execute" << endl;
}

void BossBehavTree::UpdateVariables(Vector2* pPos, Vector2* bPos, int health, int heat)
{
	_pDist = pPos->Distance(*bPos);
	_health = health;
	_heat = heat;
	_playerPos = *pPos;
	_bossPos = *bPos;
}

Vector2 BossBehavTree::GetTarget()
{
	return _targetPos;
}

