#ifndef DOOR_ACTOR_H
#define DOOR_ACTOR_H

#include <memory>
#include "Actor.h"
#include "ToggleActor.h"

class DoorActor : public Actor
{
public:
	DoorActor(Vector2 position,
		GameManager & manager,
		Vector2 spd,
		std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
		const std::string&& startSprite,
		SpriteSheet::XAxisDirection startXDirection,
		SpriteSheet::YAxisDirection startYDirection,
		const std::shared_ptr<ToggleActor> doorSwitch)
		: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection),
		_isOpen(false),
		_isClosing(false),
		_isOpening(false),
		_switch(doorSwitch)
	{
		_sprites[_currentSpriteSheet]->Pause();
	}

	virtual void Update(double deltaTime) override;
	virtual void Reset(Vector2 pos) override;

	virtual Type GetType() const override { return Type::door; }

	bool IsOpening() const;
	bool IsClosing() const;
	bool IsOpen() const;

private:
	bool _isOpening;
	bool _isClosing;
	bool _isOpen;
	std::shared_ptr<ToggleActor> _switch;
};

#endif
