#ifndef SWITCH_ACTOR_H
#define SWITCH_ACTOR_H

#include "Actor.h"

class SwitchActor : public Actor
{
public:
	SwitchActor(Vector2 position,
		GameManager & manager,
		Vector2 spd,
		std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
		const std::string&& startSprite,
		SpriteSheet::XAxisDirection startXDirection,
		SpriteSheet::YAxisDirection startYDirection,
		Edge edge,
		bool isWeightPad)
		: Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection),
		_isOn(false),
		_isWeightPad(isWeightPad)
	{
		_sprites[_currentSpriteSheet]->Pause();
	}

	virtual void Update(double deltaTime) override;

	bool IsOn() const;

	Edge GetEdge() const;

	virtual Type GetType() const override { return Type::weightpad; }

private:
	Edge _edge;
	bool _isOn;
	bool _isWeightPad;
};

#endif