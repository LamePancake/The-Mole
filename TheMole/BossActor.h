#pragma once
#include "Actor.h"
#include "BossBehavTree.h"

using namespace std;
class BossActor : public Actor
{
public:
	BossActor(Vector2 position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites, const std::string&& startSprite,
		SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT, SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP);
	~BossActor();

	// Updates position of the agent by adding _speed to it.
	virtual void UpdatePosition(double elapsedSecs);

	/**
	* Draws.
	*
	* @param [in,out]	camera	ref to the camera.
	*
	* Draws this object.
	*/
	virtual void Draw(Camera& camera) override;

	virtual void Update(double elapsedSecs);

	// Resets the actor
	virtual void Reset(Vector2 pos);

private:
	BossBehavTree _bossTree;
	int _heat;
};
 