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
	Edge colEdge, rowEdge;
	int colPenetration, rowPenetration;
	std::vector<std::shared_ptr<Tile>> rowIntersection, colIntersection;

	GetTileCollisionInfo(rowEdge, colEdge, rowPenetration, colPenetration, rowIntersection, colIntersection, level);

	int pruneRow = rowIntersection[0]->GetIndices().y;
	int pruneCol = colIntersection[0]->GetIndices().x;

	const auto pruneColIntersections = [pruneRow](std::shared_ptr<Tile>& tile) {return tile->GetIndices().y == pruneRow; };
	const auto pruneRowIntersections = [pruneCol](std::shared_ptr<Tile>& tile) {return tile->GetIndices().x == pruneCol; };

	colIntersection.erase(std::remove_if(colIntersection.begin(), colIntersection.end() - 1, pruneColIntersections));
	rowIntersection.erase(std::remove_if(rowIntersection.begin(), rowIntersection.end() - 1, pruneRowIntersections));

	float correctedYPos = _position.GetY();
	if (rowEdge == Edge::BOTTOM) correctedYPos -= rowPenetration;
	else if (rowEdge == Edge::TOP) correctedYPos += level->GetTileHeight() - rowPenetration; 

	for (auto& tile : rowIntersection)
	{
		switch (tile->GetID())
		{
		case Tile::blank:
			break;
		default:
			_position.SetY(correctedYPos);
			break;
		}
	}
	
	float correctedXPos = _position.GetX();
	if (colEdge == Edge::RIGHT) correctedXPos -= colPenetration;
	else if (colEdge == Edge::LEFT) correctedXPos += level->GetTileWidth() - colPenetration;

	float reverseX = _speed.GetX() * -1;
	SpriteSheet::XAxisDirection reverseDir = _actorDir == SpriteSheet::XAxisDirection::LEFT ? SpriteSheet::XAxisDirection::RIGHT : SpriteSheet::XAxisDirection::LEFT;
	for (auto& tile : colIntersection)
	{
		switch (tile->GetID())
		{
		case Tile::blank:
			break;
		default:
			_position.SetX(correctedXPos);
			_speed.SetX(reverseX);
			_actorDir = reverseDir;
			break;
		}
	}
}

void AIActor::Draw(Camera& camera)
{
	Actor::Draw(camera);
}
