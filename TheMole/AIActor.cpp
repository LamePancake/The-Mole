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
	Actor::UpdatePosition(elapsedSecs);

	_position.SetX(_position.GetX() + _speed.GetX() * elapsedSecs);
	_position.SetY(_position.GetY() + _speed.GetY() * elapsedSecs);
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
		float correctedYPos = _position.GetY();
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
				_position.SetY(correctedYPos);
				break;
			}
		}
	}
	
	if (colEdge != Edge::NONE)
	{
		float correctedXPos = _position.GetX();
		if (colEdge == Edge::RIGHT) correctedXPos -= colPenetration;
		else if (colEdge == Edge::LEFT) correctedXPos += colPenetration;

		float reverseX = _speed.GetX() * -1;
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
				_position.SetX(correctedXPos);
				_speed.SetX(reverseX);
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
