#include "DoorActor.h"

void DoorActor::Update(double deltaTime)
{
    Actor::Update(deltaTime);

	std::shared_ptr<SpriteSheet> currentSheet = _sprites[_currentSpriteSheet];
	bool switchOn = _switch->IsOn();

	/*
	  Switch turns on  - start opening
      Switch on and animating - keep opening
      Switch on and finished animating - stop opening, set open
      Switch turns off - start closing, set !open
      Switch off and animating - keep closing
      Switch off and not animating - stop closing
	 */
	if(switchOn)
	{
		if (!_isOpening && !_isOpen)
		{
			_isOpening = true;
			_isClosing = false;
			currentSheet->SetReversed(false);
			currentSheet->Start();
		}
		else if (currentSheet->IsFinished())
		{
			_isOpening = false;
			_isClosing = false;
			_isOpen = true;
		}
	}
	else
	{
		if (_isOpening || _isOpen)
		{
			_isOpen = false;
			_isOpening = false;
			_isClosing = true;
			currentSheet->SetReversed(true);
			currentSheet->Start();
		}
		else if (currentSheet->IsFinished())
		{
			_isOpening = false;
			_isClosing = false;
		}
	}
}

void DoorActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
	_isClosing = false;
	_isOpening = false;
	_isOpen = false;
	_sprites[_currentSpriteSheet]->SetReversed(false);
	_sprites[_currentSpriteSheet]->Reset();
}

bool DoorActor::IsOpening() const
{
	return _isOpening;
}

bool DoorActor::IsClosing() const
{
	return _isClosing;
}

bool DoorActor::IsOpen() const
{
	return _isOpen;
}
