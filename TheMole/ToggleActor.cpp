#include "ToggleActor.h"
#include "GameScreen.h"

using std::shared_ptr;

void ToggleActor::Update(double deltaTime)
{
    Actor::Update(deltaTime);

	// No need to check for collisions if we're not a weight pad and already on
	if (_isOn && !_isWeightPad) return;

	shared_ptr<Level> level = _gameScreen->GetLevel();
    bool wasOn = _isOn;

	vector<shared_ptr<Actor>> actors = level->GetActors();

	_isOn = false;
	for (auto it = actors.begin(); it != actors.end() && !_isOn; it++)
	{
		Actor::Type type = (*it)->GetType();
		switch (type)
		{
		case player:
		case enemy:
		case projectile:
			if ((*it)->GetAABB().CheckCollision(this->_aabb))
			{
				_isOn = true;
			}
		default:
			break;
		}
	}

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

void ToggleActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
	_isOn = false;
	_sprites[_currentSpriteSheet]->SetReversed(false);
	_sprites[_currentSpriteSheet]->Reset();
}

bool ToggleActor::IsOn() const
{
	return _isOn;
}

Actor::Edge ToggleActor::GetEdge() const
{
	return _edge;
}
