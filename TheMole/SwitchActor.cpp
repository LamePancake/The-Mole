#include "SwitchActor.h"
#include "GameScreen.h"

using std::shared_ptr;

void SwitchActor::Update(double deltaTime)
{
    Actor::Update(deltaTime);

	// No need to check for collisions if we're not a weight pad and already on
	if (_isOn && !_isWeightPad) return;

	shared_ptr<Level> level = _gameScreen->GetLevel();
    bool wasOn = _isOn;

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
				goto checkstate; // Heh
			}
		default:
			break;
		}
	}
    
	checkstate:
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
