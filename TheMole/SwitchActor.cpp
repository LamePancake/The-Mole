#include "SwitchActor.h"
#include "GameScreen.h"

using std::shared_ptr;

void SwitchActor::Update(double deltaTime)
{

	_sprites[_currentSpriteSheet]->Update(deltaTime);
	
	// No need to check for collisions if we're a weight pad and already on
	if (_isOn && !_isWeightPad) return;

	shared_ptr<Level> level = _gameScreen->GetLevel();
	size_t numEnemies = level->GetEnemySize();
	size_t numProjectiles = level->GetProjectileActorSize();

	for (size_t i = 0; i < numEnemies; i++)
	{
		shared_ptr<Actor> ai = level->GetEnemy(i);
		if (ai->GetAABB().CheckCollision(this->_aabb))
		{
			_isOn = true;
			return;
		}
	}

	for (size_t i = 0; i < numProjectiles; i++)
	{
		shared_ptr<Actor> proj = level->GetProjectile(i);
		if (proj->GetAABB().CheckCollision(this->_aabb))
		{
			_isOn = true;
			return;
		}
	}

	_isOn = _gameScreen->GetPlayer()->GetAABB().CheckCollision(_aabb);
}

bool SwitchActor::IsOn() const
{
	return _isOn;
}

Actor::Edge SwitchActor::GetEdge() const
{
	return _edge;
}
