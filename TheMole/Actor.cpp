#include "Actor.h"
#include "GameScreen.h"

Actor::Actor(Vector2 position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
			const std::string&& startSprite, SpriteSheet::XAxisDirection startXDirection, SpriteSheet::YAxisDirection startYDirection)
	:_curKinematic{ position, spd },
	_prevKinematic{ position, spd },
	_collisionInfo(),
    _mgr(&manager),
	_gameScreen(std::dynamic_pointer_cast<GameScreen>(manager.GetCurrentScreen())),
	_isVisible(true),
	_isDestroyed(false),
	_startXDir(startXDirection),
	_startYDir(startYDirection),
	_spriteXDir(startXDirection),
	_spriteYDir(startYDirection)
{
	SetHealth(100);
	
	// Copy the list of sprites
	_sprites = sprites;
	_currentSpriteSheet = startSprite;

	_aabb = AABB(_sprites[startSprite]->GetFrameWidth(), _sprites[startSprite]->GetFrameHeight(), *this);
	_sprites[_currentSpriteSheet]->SetXAxisDirection(_spriteXDir);
	_sprites[_currentSpriteSheet]->SetYAxisDirection(_spriteYDir);
	_sprites[_currentSpriteSheet]->Start();
}

Actor::~Actor()
{
}

AABB Actor::GetAABB()
{
	return _aabb;
}

std::shared_ptr<SpriteSheet> Actor::GetTexture()
{
	return _sprites[_currentSpriteSheet];
}

std::shared_ptr<SpriteSheet> Actor::GetTextureShadow()
{
	return _sprites[_currentSpriteSheet];
}

Vector2 Actor::GetPosition()
{
	return _curKinematic.position;
}

Vector2 Actor::GetSpeed()
{
	return _curKinematic.velocity;
}

size_t Actor::GetHealth()
{
	return _health;
}

void Actor::SetSpeed(Vector2 spd)
{
	_curKinematic.velocity = spd;
}

void Actor::SetHealth(size_t health)
{
	_health = health;
}

void Actor::SetPosition(Vector2 pos)
{
	_curKinematic.position = pos;
}

SpriteSheet::XAxisDirection Actor::GetActorXDirection() const
{
	return _spriteXDir;
}

SpriteSheet::YAxisDirection Actor::GetActorYDirection() const
{
	return _spriteYDir;
}

void Actor::SetActorXDirection(SpriteSheet::XAxisDirection dir)
{
	_spriteXDir = dir;
}

void Actor::SetActorYDirection(SpriteSheet::YAxisDirection dir)
{
	_spriteYDir = dir;
}

void Actor::SetVisibility(bool isVisible)
{
	_isVisible = isVisible;
}

bool Actor::IsVisible() const
{
	return _isVisible;
}

bool Actor::IsDestroyed() const
{
	return _isDestroyed;
}

void Actor::Destroy()
{
	_isVisible = false;
	_isDestroyed = true;
}

void Actor::Update(double elapsedSecs)
{
	if (_isDestroyed) return;

	_sprites[_currentSpriteSheet]->Update(elapsedSecs);
}

void Actor::UpdatePosition(double elapsedSecs)
{
}

void Actor::Draw(Camera& camera)
{
	if (_isDestroyed || !_isVisible) return;

	const SDL2pp::Rect& viewport = camera.GetViewport();
	int offsetX = 4;
	int offsetY = 0;

	SDL2pp::Renderer& rend = _mgr->GetRenderer();

	std::shared_ptr<SpriteSheet> spriteSheet = _sprites[_currentSpriteSheet];
    SDL2pp::Point tempPoint = SDL2pp::Point((int)_curKinematic.position.GetX(), (int)_curKinematic.position.GetY());

	// Draw shadow first, so we need to adjust drawing parameters
	SDL_Color shadowColour = { 127, 127, 127, 127 };
	SDL_Color normalColour = { 255, 255, 255, 255 };
	spriteSheet->SetColourMod(shadowColour);
	spriteSheet->SetXAxisDirection(_spriteXDir);
	spriteSheet->Draw(tempPoint + SDL2pp::Point(offsetX - viewport.x, offsetY - viewport.y));

	spriteSheet->SetColourMod(normalColour);
	spriteSheet->SetYAxisDirection(_spriteYDir);
	spriteSheet->Draw(tempPoint + SDL2pp::Point(-viewport.x, -viewport.y));
}

void Actor::Reset(Vector2 pos)
{
	SetPosition(pos);
	SetHealth(100);
	SetActorXDirection(_startXDir);
	SetActorYDirection(_startYDir);
	_aabb.UpdatePosition(*this);
}

void Actor::DetectTileCollisions(TileCollisionInfo& colInfo, std::shared_ptr<Level>& level)
{
	int tileWidth = level->GetTileWidth();
	int tileHeight = level->GetTileHeight();

	Bounds curBounds;
	Bounds prevBounds;

	GetBounds(_curKinematic, curBounds);
	GetBounds(_prevKinematic, prevBounds);

	colInfo.colEdge = Edge::NONE;
    colInfo.rowEdge = Edge::NONE;
    colInfo.rowPenetration = 0;
    colInfo.colPenetration = 0;

    float xVel = _curKinematic.velocity.GetX();
    float yVel = _curKinematic.velocity.GetY();

    if (xVel != 0 && yVel != 0)
    {
        colInfo.colEdge = xVel > 0 ? Edge::RIGHT : Edge::LEFT;
        colInfo.rowEdge = yVel > 0 ? Edge::BOTTOM : Edge::TOP;

        int curCol;
        int prevCol;
        int actualRowPenetration;
        int actualColPenetration;
        if (colInfo.colEdge == Edge::RIGHT)
        {
            curCol = curBounds.rightCol;
            prevCol = prevBounds.rightCol;
            std::shared_ptr<Tile> tile = level->GetTileFromLevel(curCol, 0);
            actualColPenetration = (int)ceil((curBounds.rightBound - tile->GetWorldPosition().GetX()));
        }
        else
        {
            curCol = curBounds.leftCol;
            prevCol = prevBounds.leftCol;
            std::shared_ptr<Tile> tile = level->GetTileFromLevel(curCol, 0);
            actualColPenetration = (int)ceil((tile->GetWorldPosition().GetX() + tileWidth - curBounds.leftBound));
        }

        level->GetTileRange(curBounds.topRow, curBounds.bottomRow + 1, curCol, curCol + 1, colInfo.colIntersect);

        int curRow;
        int prevRow;
        if (colInfo.rowEdge == Edge::TOP)
        {
            curRow = curBounds.topRow;
            prevRow = prevBounds.topRow;
            actualRowPenetration = (int)ceil((level->GetTileFromLevel(0, curRow)->GetWorldPosition().GetY() + tileHeight - curBounds.topBound));
        }
        else
        {
            curRow = curBounds.bottomRow;
            prevRow = prevBounds.bottomRow;
            actualRowPenetration = (int)ceil((curBounds.bottomBound - level->GetTileFromLevel(0, curRow)->GetWorldPosition().GetY()));
        }

        level->GetTileRange(curRow, curRow + 1, curBounds.leftCol, curBounds.rightCol + 1, colInfo.rowIntersect);

        std::shared_ptr<Tile> corner = level->GetTileFromLevel(curCol, curRow);

        int colNonCornerBlanks = 0;
        for (auto tile : colInfo.colIntersect)
        {
            if (tile != corner && tile->GetID() != Tile::blank)
            {
                colInfo.colPenetration = actualColPenetration;
                break;
            }
            else
            {
                colNonCornerBlanks++;
            }
        }

        int rowNonCornerBlanks = 0;
        for (auto tile : colInfo.rowIntersect)
        {
            if (tile != corner && tile->GetID() != Tile::blank)
            {
                colInfo.rowPenetration = actualRowPenetration;
                break;
            }
            else
            {
                rowNonCornerBlanks++;
            }
        }

		// Every tile that the actor intersects - except for the for the corner tile - is blank
        if (corner->GetID() != Tile::blank && colNonCornerBlanks == colInfo.colIntersect.size() && rowNonCornerBlanks == colInfo.rowIntersect.size())
        {
			bool corrected = false;

            // Only correct in a direction if we weren't already there
            if (curCol != prevCol)
            {
                colInfo.colPenetration = actualColPenetration;
				corrected = true;
            }
            if (curRow != prevRow)
            {
                colInfo.rowPenetration = actualRowPenetration;
				corrected = true;
            }

			// Due to rounding, we were actually in this column AND row in the last frame
			if (!corrected)
			{
				// Correct in the direction with least penetration
				// We could be completely penetrating a tile in one direction, so the other direction is the one we probably want to correct in
				if (actualColPenetration < actualRowPenetration)
					colInfo.colPenetration = actualColPenetration;
				else
					colInfo.rowPenetration = actualRowPenetration;
			}
        }
    }
    else if (xVel != 0)
    {
        colInfo.colEdge = xVel > 0 ? Edge::RIGHT : Edge::LEFT;
        int col = colInfo.colEdge == Edge::RIGHT ? curBounds.rightCol : curBounds.leftCol;
        level->GetTileRange(curBounds.topRow, curBounds.bottomRow + 1, col, col + 1, colInfo.colIntersect);

        for (auto tile : colInfo.colIntersect)
        {
            if (tile->GetID() != Tile::blank)
            {
                colInfo.colPenetration = colInfo.colEdge == Edge::LEFT
                    ? (int)ceil((tile->GetWorldPosition().GetX() + tileWidth - curBounds.leftBound))
                    : (int)ceil((curBounds.rightBound - tile->GetWorldPosition().GetX()));
                break;
            }
        }
    }
    else if (yVel != 0)
    {
        colInfo.rowEdge = yVel > 0 ? Edge::BOTTOM : Edge::TOP;
        int row = colInfo.rowEdge == Edge::TOP ? curBounds.topRow : curBounds.bottomRow;
        level->GetTileRange(row, row + 1, curBounds.leftCol, curBounds.rightCol + 1, colInfo.rowIntersect);

        for (auto tile : colInfo.rowIntersect)
        {
            if (tile->GetID() != Tile::blank)
            {
                colInfo.rowPenetration = colInfo.rowEdge == Edge::TOP
                    ? (int)ceil((tile->GetWorldPosition().GetY() + tileHeight - curBounds.topBound))
                    : (int)ceil((curBounds.bottomBound - tile->GetWorldPosition().GetY()));
                break;
            }
        }
    }
}

void Actor::GetBounds(const KinematicState & state, Bounds & bounds)
{
	int tileWidth = _gameScreen->GetLevel()->GetTileWidth();
	int tileHeight = _gameScreen->GetLevel()->GetTileHeight();

	// Calculate the actor's bounds
	// Note that this should use the AABB, but it's reporting incorrect positions currently
	bounds.rightBound = state.position.GetX() + _sprites[_currentSpriteSheet]->GetFrameWidth();
	bounds.leftBound = state.position.GetX();
	bounds.topBound = state.position.GetY();
	bounds.bottomBound = state.position.GetY() + _sprites[_currentSpriteSheet]->GetFrameHeight();

	// Determine which tiles we intersect
	bounds.topRow = (int)(bounds.topBound / tileHeight);
	bounds.bottomRow = (int)(bounds.bottomBound / tileHeight);
	bounds.leftCol = (int)(bounds.leftBound / tileWidth);
	bounds.rightCol = (int)(bounds.rightBound / tileWidth);
}