#include "BombAIActor.h"

bool BombAIActor::IsBlowingUp() const
{
    return _isBlowingUp;
}

void BombAIActor::BlowUp()
{
    std::string oldSheet = _currentSpriteSheet;

    // Start blowing up
    PlaySpriteSound("blow_up");
    SetSprite("blow_up", true);
    _isBlowingUp = true;

    // Cancel mind control
    CancelMindControl();

    // Stop movement and adjust position to look centred on previous sprite
    _curKinematic.velocity = Vector2(0, 0);
    _curKinematic.position.SetX(_curKinematic.position.GetX() - 
        (_sprites[_currentSpriteSheet]->GetFrameWidth() - _sprites[oldSheet]->GetFrameWidth()));
}

void BombAIActor::Update(double deltaTime)
{
    // We do some pretty awful things here
    AIActor::Update(deltaTime);
    if (_isBlowingUp && _sprites[_currentSpriteSheet]->IsFinished())
    {
        SetVisibility(false);
        SetActive(false);
        _isBlowingUp = false;
    }
}

BombAIActor * BombAIActor::Clone()
{
    return new BombAIActor(*this);
}

void BombAIActor::Reset(Vector2 position)
{
    AIActor::Reset(position);
    _curKinematic.velocity = Vector2(100.0f, 341.3f);
    _isBlowingUp = false;
}
