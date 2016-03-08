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



bool Selector::run(double elapsedSecs)
{
	for (Node* child : getChildren())
	{
		if (child->run(elapsedSecs))
		{
			return true;
		}

	}
	return false;
}


bool Sequence::run(double elapsedSecs)
{
	for (Node* child : getChildren())
	{
		if (!child->run(elapsedSecs))
		{
			return false;
		}

	}
	return true;
}

bool CheckHeatTask::run(double elapsedSecs)
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

bool CheckAliveTask::run(double elapsedSecs)
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


bool CheckDeadTask::run(double elapsedSecs)
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



bool CheckIfOverheatedTask::run(double elapsedSecs)
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



bool PrePunchTask::run(double elapsedSecs)
{
	if (_btDist < _triggerRange)
	{
		//cout << "close enough" << endl;
		_gameScreen->GetLevel()->GetBoss()->SetSprite("prepunch");
		return true;
	}
	else
	{
		//cout << "not close enough to punch" << endl;
		return false;
	}
}



bool PunchTask::run(double elapsedSecs)
{
	if (_gameScreen->GetLevel()->GetBoss()->_rollDur > 0)
	{
		cout << "punch" << endl;
		*_targetPos = _gameScreen->GetPlayer()->GetPosition();
		//cout << "bPos: " << targetPos->GetX() << endl;
		//cout << "pPos: " << _gameScreen->GetPlayer()->GetPosition().GetX() << endl;
		_gameScreen->GetLevel()->GetBoss()->_rollDur -= elapsedSecs;
		_gameScreen->GetLevel()->GetBoss()->SetSprite("punch");
		return false;
	}
	else
	{
		return true;
	}
}



bool PreRollTask::run(double elapsedSecs)
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



bool RollTask::run(double elapsedSecs)
{
	if (_gameScreen->GetLevel()->GetBoss()->_rollDur > 0)
	{
		cout << "roll" << endl;
		*_targetPos = _gameScreen->GetPlayer()->GetPosition();
		//cout << "bPos: " << targetPos->GetX() << endl;
		//cout << "pPos: " << _gameScreen->GetPlayer()->GetPosition().GetX() << endl;
		_gameScreen->GetLevel()->GetBoss()->_rollDur -= elapsedSecs;
		_gameScreen->GetLevel()->GetBoss()->SetSprite("roll");
		return false;
	}
	else
	{
		return true;
	}
}





bool ShortHopTask::run(double elapsedSecs)
{
	cout << "hop" << endl;
	return true;
}



bool ShockWaveTask::run(double elapsedSecs)
{
	if (_gameScreen->GetLevel()->GetBoss()->_shockWaveDur > 0)
	{
		GameManager& gameManager = *GameManager::GetInstance();

		std::shared_ptr<SDL2pp::Texture> projectileSheet = std::make_shared<SDL2pp::Texture>(gameManager.GetRenderer(), ".\\Assets\\Textures\\red_dot.png");
		std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> sprites;
		double infinity = std::numeric_limits<double>::infinity();
		sprites["shoot"] = std::make_shared<SpriteSheet>(projectileSheet, 1, infinity);
		std::shared_ptr<ProjectileActor> projectile = std::make_shared<ProjectileActor>(
			_gameScreen->GetLevel()->GetBoss()->GetPosition() ///Vec2 position
			, gameManager ///Gamemanager
			, Vector2(100.0f, 0.0f) ///Vec2 spd
			, sprites ///sprites
			, "shoot" ///startsprite
			, SpriteSheet::XAxisDirection::LEFT); ///direction

		cout << "wave" << endl;
		*_targetPos = _gameScreen->GetLevel()->GetBoss()->GetPosition();
		//cout << "bPos: " << targetPos->GetX() << endl;
		//cout << "pPos: " << _gameScreen->GetPlayer()->GetPosition().GetX() << endl;
		_gameScreen->GetLevel()->GetBoss()->_shockWaveDur -= elapsedSecs;
		//_gameScreen->GetLevel()->GetBoss()->SetSprite("roll");
		_gameScreen->GetLevel()->AddActor(projectile);
		return false;
	}
	else
	{
		return true;
	}
}



bool IdleTask::run(double elapsedSecs)
{
	if(_gameScreen->GetLevel()->GetBoss()->_idleDur > 0)
	{
		*_targetPos = _gameScreen->GetLevel()->GetBoss()->GetPosition();
		cout << "cooldown" << endl;
		//_targetPos = _bossPos;
		_gameScreen->GetLevel()->GetBoss()->_idleDur -= elapsedSecs;
		_gameScreen->GetLevel()->GetBoss()->SetSprite("idle");
		return false;
	}
	else
	{
		return true;
	}
}


bool EjectTask::run(double elapsedSecs)
{
	cout << "eject" << endl;
	return true;
}


BossBehavTree::BossBehavTree()
{
	_heat = 0;
	_health = 100;
	_meleeRange = 10;
	_idleDur = 3;
	_rollDur = 3;
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
	_tPunch = new PunchTask(&_targetPos);
	_tPreRoll = new PreRollTask(_pDist, _meleeRange);
	_tRoll = new RollTask(&_targetPos);
	_tShortHop = new ShortHopTask();
	_tShockWave = new ShockWaveTask(&_targetPos);
	_tIdle = new IdleTask(&_targetPos);
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
	_seq1Far->addChild(_tIdle);

	_seqOverHeat->addChild(_tChkOverheat);
	_seqOverHeat->addChild(_tChkAlive);
	_seqOverHeat->addChild(_tIdle);

	//Add dead tasks
	_seqDead->addChild(_tEject);
}

void BossBehavTree::ExecuteTree()
{
	while (!_root->run(_elapsedSecs))
	{
		cout << "---------------------" << endl;
	}
	//_tRoll->resetDuration();
	//cout << "Done Tree Execute" << endl;
}

void BossBehavTree::UpdateVariables(Vector2* pPos, Vector2* bPos, int health, int heat, double elapsedSecs)
{
	_pDist = pPos->Distance(*bPos);
	_health = health;
	_heat = heat;
	_playerPos = *pPos;
	_bossPos = *bPos;
  	_elapsedSecs = elapsedSecs;
}

Vector2 BossBehavTree::GetTarget()
{
	return _targetPos;
}

