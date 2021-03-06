#include "DoorActor.h"
#include "GameScreen.h"

void DoorActor::Update(double deltaTime)
{
    Actor::Update(deltaTime);
    if (_isDestroyed || !_isActive) return;

	std::shared_ptr<SpriteSheet> currentSheet = _sprites[_currentSpriteSheet];
	bool switchOn = _switch->IsOn();
    SoundEffectBank& bank = _gameScreen->GetSoundBank();

	/*
	  Switch turns on  - start opening
      Switch on and animating - keep opening
      Switch on and finished animating - stop opening, set open
      Switch turns off - start closing, set !open
      Switch off and animating - keep closing
      Switch off and not animating - stop closing
	 */
    switch (_curState)
    {
    case Closed:
        if (switchOn)
        {
            _curState = Opening;
            _gameScreen->PlaySoundIfVisible("door_progress", this, true);
            currentSheet->SetReversed(false);
            currentSheet->Start();
        }
        break;
    case Closing:
        if (currentSheet->IsFinished())
        {
            _curState = Closed;
            bank.StopSound("door_progress");
            _gameScreen->PlaySoundIfVisible("door_finish", this);
        }
        else if (switchOn)
        {
            _curState = Opening;
            currentSheet->SetReversed(false);
        }
        break;
    case Open:
        if (!switchOn)
        {
            _curState = Closing;
            _gameScreen->PlaySoundIfVisible("door_progress", this, true);
            currentSheet->SetReversed(true);
            currentSheet->Start();
        }
        break;
    case Opening:
        if (currentSheet->IsFinished())
        {
            _curState = Open;
            bank.StopSound("door_progress");
            _gameScreen->PlaySoundIfVisible("door_finish", this);
        }
        else if (!switchOn)
        {
            _curState = Closing;
            currentSheet->SetReversed(true);
        }
        break;
    }
}

void DoorActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
    SoundEffectBank& bank = _gameScreen->GetSoundBank();
    _curState = Closed;
    bank.StopSound("door_progress");
    bank.StopSound("door_finish");
	_sprites[_currentSpriteSheet]->SetReversed(false);
	_sprites[_currentSpriteSheet]->Reset();
}

DoorActor * DoorActor::Clone()
{
    return new DoorActor(*this);
}

bool DoorActor::IsOpening() const
{
    return _curState == Opening;
}

bool DoorActor::IsClosing() const
{
	return _curState == Closing;
}

bool DoorActor::IsOpen() const
{
    return _curState == Open;
}

Actor::Edge DoorActor::GetEdge() const
{
	return _edge;
}
