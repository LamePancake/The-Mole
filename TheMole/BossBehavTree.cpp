#include "BossBehavTree.h"



BossBehavTree::BossBehavTree()
{
	_heat = 0;
	_health = 100;
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
	_tPrePunch = new PrePunchTask(_pDist);
	_tPunch = new PunchTask();
	_tPreRoll = new PreRollTask(_pDist);
	_tRoll = new RollTask();
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
	cout << "Done Tree Execute" << endl;
}

void BossBehavTree::UpdateVariables(Vector2 pPos, Vector2 bPos, int health, int heat)
{
	_pDist = CalculateDist(pPos, bPos);
	_health = health;
	_heat = heat;
}

