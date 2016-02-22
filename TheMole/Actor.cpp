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

	// Only check things that have changed and in the correct direction (for now...)
	if (_curKinematic.velocity.GetX() < 0 && curBounds.leftCol != prevBounds.leftCol)
	{
        level->GetTileRange(curBounds.topRow, curBounds.bottomRow + 1, curBounds.leftCol, curBounds.leftCol + 1, colIntersect);
        colEdge = Edge::LEFT;
	}
	else if (_curKinematic.velocity.GetX() > 0 && curBounds.rightCol != prevBounds.rightCol)
	{
        level->GetTileRange(curBounds.topRow, curBounds.bottomRow + 1, curBounds.rightCol, curBounds.rightCol + 1, colIntersect);
        colEdge = Edge::RIGHT;
	}

	if (_curKinematic.velocity.GetY() > 0 && curBounds.topRow != prevBounds.topRow)
	{
        level->GetTileRange(curBounds.topRow, curBounds.topRow + 1, curBounds.leftCol, curBounds.rightCol + 1, rowIntersect);
        rowEdge = Edge::TOP;
	}
	else if (_curKinematic.velocity.GetY() < 0 &&  curBounds.bottomRow != prevBounds.bottomRow)
	{
        level->GetTileRange(curBounds.bottomRow, curBounds.bottomRow + 1, curBounds.leftCol, curBounds.rightCol + 1, rowIntersect);
        rowEdge = Edge::BOTTOM;
	}

    if (!rowIntersect.empty() && !colIntersect.empty())
    {
        std::shared_ptr<Tile> corner = level->GetTileFromLevel(rowIntersect[0]->GetIndices().y, colIntersect[0]->GetIndices().x);

        // Determine the number of blank tiles in each set of tiles
        int blankRowTiles = 0;
        for (auto tile : rowIntersect)
        {
            if (tile != corner && tile->GetID() != Tile::blank)
                break;
            else
                blankRowTiles++;
        }

        int blankColTiles = 0;
        for (auto tile : colIntersect)
        {
            if (tile != corner && tile->GetID() != Tile::blank)
                break;
            else
                blankColTiles++;
        }

        Vector2 cornerPos = corner->GetWorldPosition();

        // We've found a non-corner tile in both sets that pushes us out, so correct both
        if (blankRowTiles < rowIntersect.size() && blankColTiles < colIntersect.size())
        {
            rowPenetration = _curKinematic.velocity.GetY() < 0 
                ? (int)(curBounds.bottomBound - cornerPos.GetY())
                : (int)(cornerPos.GetY() + tileHeight - curBounds.topBound);

            colPenetration = _curKinematic.velocity.GetX() < 0
                ? (int)(curBounds.rightBound - cornerPos.GetX())
                : (int)(cornerPos.GetX() + tileWidth - curBounds.leftBound);
        }
        else if (blankColTiles < colIntersect.size())
        {
            colPenetration = _curKinematic.velocity.GetX() < 0
                ? (int)(curBounds.rightBound - cornerPos.GetX())
                : (int)(cornerPos.GetX() + tileWidth - curBounds.leftBound);
        }
        else if (blankRowTiles < rowIntersect.size())
        {
            rowPenetration = _curKinematic.velocity.GetY() < 0
                ? (int)(curBounds.bottomBound - cornerPos.GetY())
                : (int)(cornerPos.GetY() + tileHeight - curBounds.topBound);
        }
        else if (corner->GetID() != Tile::blank)
        {
            rowPenetration = _curKinematic.velocity.GetY() < 0
                ? (int)(curBounds.bottomBound - cornerPos.GetY())
                : (int)(cornerPos.GetY() + tileHeight - curBounds.topBound);

            colPenetration = _curKinematic.velocity.GetX() < 0
                ? (int)(curBounds.rightBound - cornerPos.GetX())
                : (int)(cornerPos.GetX() + tileWidth - curBounds.leftBound);
        }
    }
    else if (!rowIntersect.empty())
    {
        // Correct y
    }
    else if (!colIntersect.empty())
    {
        // Correct x
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