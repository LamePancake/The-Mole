#ifndef BOMB_AI_ACTOR_H
#define BOMB_AI_ACTOR_H
#include "AIActor.h"

class BombAIActor : public AIActor
{
	virtual void Update(double deltaTime) override;
	virtual void Reset(Vector2 pos) override;
	virtual void GetType() const override { return Type::bombenemy; }
};

#endif