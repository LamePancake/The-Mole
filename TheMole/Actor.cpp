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

	// Only check things that have changed
	if (curBounds.leftCol != prevBounds.leftCol)
	{

	}
	if (curBounds.rightCol != prevBounds.rightCol)
	{

	}

	if (curBounds.topRow != prevBounds.topRow)
	{

	}
	if (curBounds.bottomRow != prevBounds.bottomRow)
	{

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