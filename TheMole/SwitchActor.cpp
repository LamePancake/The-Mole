#include "SwitchActor.h"
#include "GameScreen.h"

using std::shared_ptr;

void SwitchActor::Update(double deltaTime)
{

	_sprites[_currentSpriteSheet]->Update(deltaTime);
	
	// No need to check for collisions if we're a weight pad and already on
	if (_isOn && !_isWeightPad) return;

	shared_ptr<Level> level = _gameScreen->GetLevel();

	for (auto actor : level->GetActors())
	{
		Actor::Type type = actor->GetType();
		switch (type)
		{
		case player:
		case enemy:
		case projectile:
			if (actor->GetAABB().CheckCollision(this->_aabb))
			{
				_isOn = true;
				return;
			}
		default:
			break;
		}
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
