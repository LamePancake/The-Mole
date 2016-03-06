#include "SwitchActor.h"
#include "GameScreen.h"

using std::shared_ptr;

void SwitchActor::Update(double deltaTime)
{
    Actor::Update(deltaTime);

	// No need to check for collisions if we're not a weight pad and already on
	if (_isOn && !_isWeightPad) return;

	shared_ptr<Level> level = _gameScreen->GetLevel();
	size_t numEnemies = level->GetEnemySize();
	size_t numProjectiles = level->GetProjectileActorSize();
    bool wasOn = _isOn;

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
    
    // Some hacks to depress/elevate the switch (there are only two frames, and there's no delay between them)
    if (wasOn && !_isOn)
    {
        _sprites[_currentSpriteSheet]->SetReversed(true);
        _sprites[_currentSpriteSheet]->Start();
    }
    else if (!wasOn && _isOn)
    {
        _sprites[_currentSpriteSheet]->SetReversed(false);
        _sprites[_currentSpriteSheet]->Start();
    }
}

bool SwitchActor::IsOn() const
{
	return _isOn;
}

Actor::Edge SwitchActor::GetEdge() const
{
	return _edge;
}
