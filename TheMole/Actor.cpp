#include "Actor.h"
#include "GameScreen.h"

using std::vector;
using std::string;
using std::shared_ptr;

Actor::Actor(Vector2 position, GameManager & manager, Vector2 spd, std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
			const std::string&& startSprite, SpriteSheet::XAxisDirection startXDirection, SpriteSheet::YAxisDirection startYDirection, bool active)
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
	_spriteYDir(startYDirection),
    _isActive(active),
    _destroysOnInactive(false),
    _sounds()
{
	SetHealth(100);
	
	// Copy the list of sprites
	_sprites = sprites;
	_currentSpriteSheet = startSprite;

	_aabb = AABB(_sprites[startSprite]->GetFrameWidth(), _sprites[startSprite]->GetFrameHeight(), *this);
	_sprites[_currentSpriteSheet]->SetXAxisDirection(_spriteXDir);
	_sprites[_currentSpriteSheet]->SetYAxisDirection(_spriteYDir);
	_sprites[_currentSpriteSheet]->Start();

    _collisionInfo.shouldCorrectX = false;
    _collisionInfo.shouldCorrectY = false;
}

Actor::Actor(Vector2 position,
    GameManager & manager,
    Vector2 spd,
    std::unordered_map < std::string,
    std::shared_ptr < SpriteSheet >> &sprites,
    const std::string&& startSprite,
    std::unordered_map<std::string, std::pair<std::string, bool>> sounds,
    SpriteSheet::XAxisDirection startXDirection,
    SpriteSheet::YAxisDirection startYDirection,
    bool active)
    : Actor(position, manager, spd, sprites, std::move(startSprite), startXDirection, startYDirection, active)
{
    _sounds = sounds;
}

Actor::Actor(std::string & serialised)
    : _collisionInfo(),
    _mgr(GameManager::GetInstance()),
    _gameScreen(std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen())),
    _isVisible(true),
    _isDestroyed(false),
    _destroysOnInactive(false)
{
    SetHealth(100);

    std::istringstream lineStream(serialised);
    std::string line;
    
    // Get position
    getline(lineStream, line);
    float pos;
    std::istringstream posReader(line);
    posReader >> pos;
    _curKinematic.position.SetX(pos);
    posReader >> pos;
    _curKinematic.position.SetY(pos);

    // Get speed
    getline(lineStream, line);
    float speed;
    std::istringstream speedReader(line);
    speedReader >> speed;
    _curKinematic.velocity.SetX(speed);
    speedReader >> speed;
    _curKinematic.velocity.SetY(speed);

    // Get the list of sprites
    vector<string> splitLine;
    while(true)
    {
        getline(lineStream, line);
        line.erase(std::remove(line.end() - 1, line.end(), '\r'), line.end());

        splitLine = split(line, ' ');
        if (splitLine.size() <= 1) break;

        int numFrames;
        double duration;
        bool isRepeating;
        SpriteSheet::XAxisDirection xDir;
        SpriteSheet::YAxisDirection yDir;
        std::istringstream spriteParamsStream(line);
        spriteParamsStream.ignore(splitLine[0].size() + splitLine[1].size() + 1);

        // Read in spritesheet parameters
        spriteParamsStream >> numFrames;
        spriteParamsStream >> duration;
        spriteParamsStream >> isRepeating;
        xDir = splitLine[5] == "LEFT" ? SpriteSheet::XAxisDirection::LEFT : SpriteSheet::XAxisDirection::RIGHT;
        yDir = splitLine[6] == "UP" ? SpriteSheet::YAxisDirection::UP : SpriteSheet::YAxisDirection::DOWN;

        // Add sprite
        _sprites[splitLine[0]] = shared_ptr<SpriteSheet>(new SpriteSheet(std::move(splitLine[1]), numFrames, duration, isRepeating, xDir, yDir));
        _spriteXDir = xDir;
        _spriteYDir = yDir;
    }

    // splitLine[0] contains the name of the start sheet after the above loop
    _currentSpriteSheet = splitLine[0];

    // Get start x and y directions
    getline(lineStream, line);
    line.erase(std::remove(line.end() - 1, line.end(), '\r'), line.end());
    _startXDir = line == "LEFT" ? SpriteSheet::XAxisDirection::LEFT : SpriteSheet::XAxisDirection::RIGHT;
    getline(lineStream, line);
    line.erase(std::remove(line.end() - 1, line.end(), '\r'), line.end());
    _startYDir = line == "UP" ? SpriteSheet::YAxisDirection::UP : SpriteSheet::YAxisDirection::DOWN;
    
    // Determine whether we're active
    getline(lineStream, line);
    line.erase(std::remove(line.end() - 1, line.end(), '\r'), line.end());
    _isActive = line == "1" ? true : false;

    _aabb = AABB(_sprites[_currentSpriteSheet]->GetFrameWidth(), _sprites[_currentSpriteSheet]->GetFrameHeight(), *this);

    serialised.erase(0, lineStream.tellg());
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

void Actor::SetSprite(std::string & sprite, bool playSoundIfNotVisible)
{
    _sprites[_currentSpriteSheet]->Stop();
    _currentSpriteSheet = sprite;
    _sprites[_currentSpriteSheet]->Start();
    
    if (_sounds.find(sprite) != _sounds.end())
    {
        auto sound = _sounds[sprite];
        if (playSoundIfNotVisible)
        {
            std::string strCopy = sound.first;
            _gameScreen->GetSoundBank().PlaySound(std::move(strCopy), sound.second);
        }
        else
        {
            _gameScreen->PlaySoundIfVisible(sound.first, this, sound.second);
        }
    }
    
}

void Actor::SetSprite(std::string && sprite, bool playSoundIfNotVisible)
{
    SetSprite(sprite, playSoundIfNotVisible);
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

bool Actor::IsActive() const
{
    return _isActive;
}

void Actor::SetActive(bool active)
{
    _isActive = active;
    if(_destroysOnInactive)
        Destroy();
}

void Actor::Destroy()
{
	_isVisible = false;
	_isDestroyed = true;
}

bool Actor::DestroysOnInactive() const
{
    return _destroysOnInactive;
}

void Actor::SetDestroysOnInactive(bool destroyOnInactive)
{
    _destroysOnInactive = destroyOnInactive;
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

    for (auto & sheetKey : _sprites)
    {
        sheetKey.second->Reset();
    }
}

int Actor::GetZIndex()
{
	return ZIndex[GetType()];
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
    colInfo.shouldCorrectX = false;
    colInfo.shouldCorrectY = false;
    colInfo.corner = nullptr;

    float xVel = _curKinematic.velocity.GetX();
    float yVel = _curKinematic.velocity.GetY();

    if (xVel != 0 && yVel != 0)
    {
        colInfo.colEdge = xVel > 0 ? Edge::RIGHT : Edge::LEFT;
        colInfo.rowEdge = yVel > 0 ? Edge::BOTTOM : Edge::TOP;

        int curCol;
        int prevCol;
        if (colInfo.colEdge == Edge::RIGHT)
        {
            curCol = curBounds.rightCol;
            prevCol = prevBounds.rightCol;
            std::shared_ptr<Tile> tile = level->GetTileFromLevel(curCol, 0);
            colInfo.colPenetration = (int)ceil((curBounds.rightBound - tile->GetWorldPosition().GetX()));
        }
        else
        {
            curCol = curBounds.leftCol;
            prevCol = prevBounds.leftCol;
            std::shared_ptr<Tile> tile = level->GetTileFromLevel(curCol, 0);
            colInfo.colPenetration = (int)ceil((tile->GetWorldPosition().GetX() + tileWidth - curBounds.leftBound));
        }

        level->GetTileRange(curBounds.topRow, curBounds.bottomRow + 1, curCol, curCol + 1, colInfo.colIntersect);

        int curRow;
        int prevRow;
        if (colInfo.rowEdge == Edge::TOP)
        {
            curRow = curBounds.topRow;
            prevRow = prevBounds.topRow;
            colInfo.rowPenetration = (int)ceil((level->GetTileFromLevel(0, curRow)->GetWorldPosition().GetY() + tileHeight - curBounds.topBound));
        }
        else
        {
            curRow = curBounds.bottomRow;
            prevRow = prevBounds.bottomRow;
            colInfo.rowPenetration = (int)ceil((curBounds.bottomBound - level->GetTileFromLevel(0, curRow)->GetWorldPosition().GetY()));
        }

        level->GetTileRange(curRow, curRow + 1, curBounds.leftCol, curBounds.rightCol + 1, colInfo.rowIntersect);
        colInfo.corner = level->GetTileFromLevel(curCol, curRow);

        int colNonCornerBlanks = 0;
        for (auto tile : colInfo.colIntersect)
        {
            if (tile != colInfo.corner && tile->GetID() != Tile::blank)
            {
                colInfo.shouldCorrectX = true;
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
            if (tile != colInfo.corner && tile->GetID() != Tile::blank)
            {
                colInfo.shouldCorrectY = true;
                break;
            }
            else
            {
                rowNonCornerBlanks++;
            }
        }

		// Every tile that the actor intersects - except for the for the corner tile - is blank
        if (colInfo.corner->GetID() != Tile::blank &&
            colNonCornerBlanks == colInfo.colIntersect.size() &&
            rowNonCornerBlanks == colInfo.rowIntersect.size())
        {
			bool corrected = false;

            // Only correct in a direction if we weren't already there
            if (curCol != prevCol)
            {
                colInfo.shouldCorrectX = true;
				corrected = true;
            }
            if (curRow != prevRow)
            {
                colInfo.shouldCorrectY = true;
				corrected = true;
            }

			// Due to rounding, we were actually in this column AND row in the last frame
			if (!corrected)
			{
				// Correct in the direction with least penetration
				// We could be completely penetrating a tile in one direction, so the other direction is the one we probably want to correct in
				if (colInfo.colPenetration < colInfo.rowPenetration)
					colInfo.shouldCorrectX = true;
				else
					colInfo.shouldCorrectY = true;
			}
        }
    }
    else if (xVel != 0)
    {
        colInfo.colEdge = xVel > 0 ? Edge::RIGHT : Edge::LEFT;
        GetTilesAlongEdge(colInfo.colEdge, curBounds, colInfo.colIntersect);

        colInfo.colPenetration = colInfo.colEdge == Edge::LEFT
            ? (int)ceil((colInfo.colIntersect[0]->GetWorldPosition().GetX() + tileWidth - curBounds.leftBound))
            : (int)ceil((curBounds.rightBound - colInfo.colIntersect[0]->GetWorldPosition().GetX()));

        for (auto tile : colInfo.colIntersect)
        {
            if (tile->GetID() != Tile::blank)
            {
                colInfo.shouldCorrectX = true;
                break;
            }
        }
    }
    else if (yVel != 0)
    {
        colInfo.rowEdge = yVel > 0 ? Edge::BOTTOM : Edge::TOP;
        GetTilesAlongEdge(colInfo.rowEdge, curBounds, colInfo.rowIntersect);

        colInfo.rowPenetration = colInfo.rowEdge == Edge::TOP
            ? (int)ceil((colInfo.rowIntersect[0]->GetWorldPosition().GetY() + tileHeight - curBounds.topBound))
            : (int)ceil((curBounds.bottomBound - colInfo.rowIntersect[0]->GetWorldPosition().GetY()));

        for (auto tile : colInfo.rowIntersect)
        {
            if (tile->GetID() != Tile::blank)
            {
                colInfo.shouldCorrectY = true;
                break;
            }
        }
    }
}

void Actor::GetTilesAlongEdge(Edge edge, const Bounds& bounds, vector<shared_ptr<Tile>>& tiles)
{
    if (edge == Edge::NONE) return;
    if (edge == Edge::TOP || edge == Edge::BOTTOM)
    {
        int row = edge == Edge::TOP ? bounds.topRow : bounds.bottomRow;
        _gameScreen->GetLevel()->GetTileRange(row, row + 1, bounds.leftCol, bounds.rightCol + 1, tiles);
    }
    else
    {
        int col = edge == Edge::RIGHT ? bounds.rightCol : bounds.leftCol;
        _gameScreen->GetLevel()->GetTileRange(bounds.topRow, bounds.bottomRow + 1, col, col + 1, tiles);
    }
}

void Actor::GetBounds(const KinematicState & state, Bounds & bounds)
{
	int tileWidth = _gameScreen->GetLevel()->GetTileWidth();
	int tileHeight = _gameScreen->GetLevel()->GetTileHeight();

	// Calculate the actor's bounds
	// Note that this should use the AABB, but it's reporting incorrect positions currently
	bounds.rightBound = state.position.GetX() + _aabb.GetWidth();
	bounds.leftBound = state.position.GetX();
	bounds.topBound = state.position.GetY();
	bounds.bottomBound = state.position.GetY() + _aabb.GetHeight();

	// Determine which tiles we intersect
	bounds.topRow = (int)(bounds.topBound / tileHeight);
	bounds.bottomRow = (int)(bounds.bottomBound / tileHeight);
	bounds.leftCol = (int)(bounds.leftBound / tileWidth);
	bounds.rightCol = (int)(bounds.rightBound / tileWidth);
}