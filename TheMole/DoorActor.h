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
		Edge edge,
		const std::shared_ptr<ToggleActor> doorSwitch)
		: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection),
		_edge(edge),
		_switch(doorSwitch),
        _curState(Closed)
	{
		_sprites[_currentSpriteSheet]->Pause();
	}

	virtual void Update(double deltaTime) override;
	virtual void Reset(Vector2 pos) override;

    virtual DoorActor* Clone() override;
    virtual bool IsCloneable() const { return true; }

	virtual Type GetType() const override { return Type::door; }

	bool IsOpening() const;
	bool IsClosing() const;
	bool IsOpen() const;
	Edge GetEdge() const;

private:
    enum State {
        Closed,
        Closing,
        Opening,
        Open
    };

    State _curState;
	Edge _edge;
	std::shared_ptr<ToggleActor> _switch;
};

#endif
