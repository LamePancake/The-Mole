#include "AIActor.h"
#include "GameScreen.h"

AIActor::~AIActor()
{
}

void AIActor::Update(double elapsedSecs)
{
	Actor::Update(elapsedSecs);

	// While the AI is alive, do stuff.
	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	if (_health <= 0)
	{
		//std::cout << "Dead\n";
	}
	else
	{
		_aabb.UpdatePosition(*this);
		UpdatePosition(elapsedSecs);
		ScanNeighbouringTiles(_gameScreen->GetLevel());
	}
}
;
void AIActor::UpdatePosition(double elapsedSecs)
{
	// Copy everything over so that we can use this in collision detection stuff later
	_prevKinematic = _curKinematic;

	Actor::UpdatePosition(elapsedSecs);

	_curKinematic.position.x += _curKinematic.velocity.GetX() * elapsedSecs;
	_curKinematic.position.y += _curKinematic.velocity.GetY() * elapsedSecs;
}

bool AIActor::CollisionCheck(Actor &otherActor)
{
	return _aabb.CheckCollision(otherActor.GetAABB());
}

void AIActor::ScanNeighbouringTiles(std::shared_ptr<Level>& level)
{
	Edge colEdge, rowEdge;
	int colPenetration, rowPenetration;
	std::vector<std::shared_ptr<Tile>> rowIntersection, colIntersection;

	DetectTileCollisions(rowEdge, colEdge, rowPenetration, colPenetration, rowIntersection, colIntersection, level);

	if (rowEdge != Edge::NONE)
	{
		float correctedYPos = _curKinematic.position.y;
		if (rowEdge == Edge::BOTTOM) correctedYPos -= rowPenetration;
		else if (rowEdge == Edge::TOP) correctedYPos += rowPenetration;

		for (auto& tile : rowIntersection)
		{
			switch (tile->GetID())
			{
			case Tile::blank:
				break;
			case Tile::spike:
				_health = 0;
			default:
				_curKinematic.position.y = correctedYPos;
				break;
			}
		}
	}
	
	if (colEdge != Edge::NONE)
	{
		float correctedXPos = _curKinematic.position.x;
		if (colEdge == Edge::RIGHT) correctedXPos -= colPenetration;
		else if (colEdge == Edge::LEFT) correctedXPos += colPenetration;

		float reverseX = _curKinematic.velocity.GetX() * -1;
		SpriteSheet::XAxisDirection reverseDir = _spriteXDir == SpriteSheet::XAxisDirection::LEFT ? SpriteSheet::XAxisDirection::RIGHT : SpriteSheet::XAxisDirection::LEFT;
		for (auto& tile : colIntersection)
		{
			switch (tile->GetID())
			{
			case Tile::blank:
				break;
			case Tile::spike:
				_health = 0;
			default:
				_curKinematic.position.x = correctedXPos;
				_curKinematic.velocity.SetX(reverseX);
				_spriteXDir = reverseDir;
				break;
			}
		}
	}
}

void AIActor::Draw(Camera& camera)
{
	Actor::Draw(camera);
}
