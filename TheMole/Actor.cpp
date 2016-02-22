#include "Actor.h"
#include "GameScreen.h"

Actor::Actor(SDL2pp::Point position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
			const std::string&& startSprite, SpriteSheet::XAxisDirection startXDirection, SpriteSheet::YAxisDirection startYDirection)
	:_curKinematic{ position, spd }, _prevKinematic{ position, spd }, _mgr(&manager), _gameScreen(std::dynamic_pointer_cast<GameScreen>(manager.GetCurrentScreen())), _isVisible(true)
{
	_health = 100;
	
	// Copy the list of sprites
	_sprites = sprites;
	_currentSpriteSheet = startSprite;

	_aabb = AABB(_sprites[startSprite]->GetFrameWidth(), _sprites[startSprite]->GetFrameHeight(), *this);
	_spriteXDir = startXDirection;
	_spriteYDir = startYDirection;
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

SDL2pp::Point Actor::GetPosition()
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

void Actor::SetPosition(SDL2pp::Point pos)
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

void Actor::Update(double elapsedSecs)
{
	_sprites[_currentSpriteSheet]->Update(elapsedSecs);
}

void Actor::UpdatePosition(double elapsedSecs)
{
}

void Actor::Draw(Camera& camera)
{
	if (!_isVisible) return;

	const SDL2pp::Rect& viewport = camera.GetViewport();
	int offsetX = 4;
	int offsetY = 0;

	SDL2pp::Renderer& rend = _mgr->GetRenderer();

	std::shared_ptr<SpriteSheet> spriteSheet = _sprites[_currentSpriteSheet];
	SDL_Texture* rawTexture = spriteSheet->GetTexture().Get();

	// Draw shadow first, so we need to adjust drawing parameters
	SDL_SetTextureColorMod(rawTexture, 127, 127, 127);
	SDL_SetTextureAlphaMod(rawTexture, 127);
	spriteSheet->Draw(_curKinematic.position + SDL2pp::Point(offsetX - viewport.x, offsetY - viewport.y), _spriteXDir, _spriteYDir);

	SDL_SetTextureColorMod(rawTexture, 255, 255, 255);
	SDL_SetTextureAlphaMod(rawTexture, 255);
	spriteSheet->Draw(_curKinematic.position + SDL2pp::Point(-viewport.x, -viewport.y), _spriteXDir, _spriteYDir);
}

void Actor::DetectTileCollisions(Edge & rowEdge, Edge & colEdge, int & rowPenetration, int & colPenetration,
	std::vector<std::shared_ptr<Tile>>& rowIntersect, std::vector<std::shared_ptr<Tile>>& colIntersect, std::shared_ptr<Level>& level)
{
	int tileWidth = level->GetTileWidth();
	int tileHeight = level->GetTileHeight();

	Bounds curBounds;
	Bounds prevBounds;

	GetBounds(_curKinematic, curBounds);
	GetBounds(_prevKinematic, prevBounds);

	colEdge = Edge::NONE;
	rowEdge = Edge::NONE;
    rowPenetration = 0;
    colPenetration = 0;

    float xVel = _curKinematic.velocity.GetX();
    float yVel = _curKinematic.velocity.GetY();

    if (xVel != 0 && yVel != 0)
    {
        colEdge = xVel > 0 ? Edge::RIGHT : Edge::LEFT;
        rowEdge = yVel > 0 ? Edge::BOTTOM : Edge::TOP;

        int curCol;
        int prevCol;
        int actualRowPenetration;
        int actualColPenetration;
        if (colEdge == Edge::RIGHT)
        {
            curCol = curBounds.rightCol;
            prevCol = prevBounds.rightCol;
            std::shared_ptr<Tile> tile = level->GetTileFromLevel(curCol, 0);
            actualColPenetration = (int)(curBounds.rightBound - tile->GetWorldPosition().GetX());
        }
        else
        {
            curCol = curBounds.leftCol;
            prevCol = prevBounds.leftCol;
            std::shared_ptr<Tile> tile = level->GetTileFromLevel(curCol, 0);
            actualColPenetration = (int)(tile->GetWorldPosition().GetX() + tileWidth - curBounds.leftBound);
        }

        level->GetTileRange(curBounds.topRow, curBounds.bottomRow + 1, curCol, curCol + 1, colIntersect);

        int curRow;
        int prevRow;
        if (rowEdge == Edge::TOP)
        {
            curRow = curBounds.topRow;
            prevRow = prevBounds.topRow;
            actualRowPenetration = (int)(level->GetTileFromLevel(0, curRow)->GetWorldPosition().GetY() + tileHeight - curBounds.topBound);
        }
        else
        {
            curRow = curBounds.bottomRow;
            prevRow = prevBounds.bottomRow;
            actualRowPenetration = (int)(curBounds.bottomBound - level->GetTileFromLevel(0, curRow)->GetWorldPosition().GetY());
        }

        level->GetTileRange(curRow, curRow + 1, curBounds.leftCol, curBounds.rightCol + 1, rowIntersect);

        std::shared_ptr<Tile> corner = level->GetTileFromLevel(curCol, curRow);

        int colNonCornerBlanks = 0;
        for (auto tile : colIntersect)
        {
            if (tile != corner && tile->GetID() != Tile::blank)
            {
                colPenetration = actualColPenetration;
                break;
            }
            else
            {
                colNonCornerBlanks++;
            }
        }

        int rowNonCornerBlanks = 0;
        for (auto tile : rowIntersect)
        {
            if (tile != corner && tile->GetID() != Tile::blank)
            {
                rowPenetration = actualRowPenetration;
                break;
            }
            else
            {
                rowNonCornerBlanks++;
            }
        }

        if (corner->GetID() != Tile::blank && colNonCornerBlanks == colIntersect.size() && rowNonCornerBlanks == rowIntersect.size())
        {
            // Only correct in a direction if we weren't already there
            if (curCol != prevCol)
            {
                colPenetration = actualColPenetration;
            }
            if (curRow != prevRow)
            {
                rowPenetration = actualRowPenetration;
            }
        }
    }
    else if (xVel != 0)
    {
        colEdge = xVel > 0 ? Edge::RIGHT : Edge::LEFT;
        int col = colEdge == Edge::RIGHT ? curBounds.rightCol : curBounds.leftCol;
        level->GetTileRange(curBounds.topRow, curBounds.bottomRow + 1, col, col + 1, colIntersect);

        for (auto tile : colIntersect)
        {
            if (tile->GetID() != Tile::blank)
            {
                colPenetration = colEdge == Edge::LEFT
                    ? (int)(tile->GetWorldPosition().GetX() + tileWidth - curBounds.leftBound)
                    : (int)(curBounds.rightBound - tile->GetWorldPosition().GetX());
                break;
            }
        }
    }
    else if (yVel != 0)
    {
        rowEdge = yVel > 0 ? Edge::BOTTOM : Edge::TOP;
        int row = rowEdge == Edge::TOP ? curBounds.topRow : curBounds.bottomRow;
        level->GetTileRange(row, row + 1, curBounds.leftCol, curBounds.rightCol + 1, rowIntersect);

        for (auto tile : rowIntersect)
        {
            if (tile->GetID() != Tile::blank)
            {
                rowPenetration = rowEdge == Edge::TOP
                    ? (int)(tile->GetWorldPosition().GetY() + tileHeight - curBounds.topBound)
                    : (int)(curBounds.bottomBound - tile->GetWorldPosition().GetY());
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
	bounds.rightBound = state.position.x + _sprites[_currentSpriteSheet]->GetFrameWidth();
	bounds.leftBound = state.position.x;
	bounds.topBound = state.position.y;
	bounds.bottomBound = state.position.y + _sprites[_currentSpriteSheet]->GetFrameHeight();

	// Determine which tiles we intersect
	// If the bottom or right is exactly flush, we subtract one so that we don't test against tiles in the next row/column
	bounds.topRow = (int)floor(bounds.topBound / tileHeight);
	bounds.bottomRow = (int)floor(bounds.bottomBound / tileHeight) - ((int)bounds.bottomBound % tileHeight == 0 ? 1 : 0);
	bounds.leftCol = (int)floor(bounds.leftBound / tileWidth);
	bounds.rightCol = (int)floor(bounds.rightBound / tileWidth) - ((int)bounds.rightBound % tileWidth == 0 ? 1 : 0);
}