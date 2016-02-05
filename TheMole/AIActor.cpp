#include "AIActor.h"

AIActor::~AIActor()
{
}

void AIActor::Update(double elapsedSecs, std::shared_ptr<Level> & level)
{
	Actor::Update(elapsedSecs, level);

	// While the AI is alive, do stuff.
	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	if (_health <= 0)
	{
		//std::cout << "Dead\n";
	}
	if (_health > 0)
	{
		//_aabb.UpdatePosition(*this);
		UpdatePosition(elapsedSecs);
		ScanNeighbouringTiles(level);
	}
}
;
void AIActor::UpdatePosition(double elapsedSecs)
{
	Actor::UpdatePosition(elapsedSecs);

	_position.SetX(_position.GetX() + _speed.GetX() * elapsedSecs);
	_position.SetY(_position.GetY() + _speed.GetY() * elapsedSecs);
}

bool AIActor::CollisionCheck(Actor &otherAI)
{
	return _aabb.CheckCollision(otherAI.GetAABB());
}

void AIActor::ScanNeighbouringTiles(std::shared_ptr<Level>& level)
{
	Edge xEdge, yEdge;
	int xPenetration, yPenetration;
	std::vector<std::shared_ptr<Tile>> xIntersection, yIntersection;

	GetTileCollisionInfo(xEdge, yEdge, xPenetration, yPenetration, xIntersection, yIntersection, level);

	double reverseX = _speed.GetX() * -1;
	for (auto& tile : xIntersection)
	{
		switch (tile->GetID())
		{
		case Tile::stone:
			_speed.SetX(reverseX);
			_actorDir = _actorDir == SpriteSheet::XAxisDirection::RIGHT ? SpriteSheet::XAxisDirection::LEFT : SpriteSheet::XAxisDirection::RIGHT;
			break;
		}
	}
}

void AIActor::Draw(Camera& camera)
{
	Actor::Draw(camera);
}
