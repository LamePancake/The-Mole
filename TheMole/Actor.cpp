#include "Actor.h"

Actor::Actor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath, std::string shadowTexturePath)
	:_position(position), _mgr(&manager), _speed(spd)
{
	_health = 100;
	// This is temporary so we can test whether AABB can be created from texture dimensions
	_sprite = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), texturePath);
	_spriteShadow = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), shadowTexturePath);
	_aabb = AABB(*_sprite, *this);
}

Actor::~Actor()
{
}

AABB Actor::GetAABB()
{
	return _aabb;
}

std::shared_ptr<SDL2pp::Texture> Actor::GetTexture()
{
	return _sprite;
}

std::shared_ptr<SDL2pp::Texture> Actor::GetTextureShadow()
{
	return _spriteShadow;
}

Vector2 Actor::GetPosition()
{
	return _position;
}

Vector2 Actor::GetSpeed()
{
	return _speed;
}

size_t Actor::GetHealth()
{
	return _health;
}

void Actor::SetSpeed(Vector2 spd)
{
	_speed = spd;
}

void Actor::SetHealth(size_t health)
{
	_health = health;
}

void Actor::SetPosition(Vector2 pos)
{
	_position = pos;
}

void Actor::Update(std::shared_ptr<Level>& level)
{
}

void Actor::UpdatePosition()
{
}

void Actor::Draw(std::shared_ptr<Level>& level)
{
	float offsetX = 15.0f;
	float offsetY = 15.0f;
			
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	SDL2pp::Window& window = _mgr->GetWindow();
	SDL2pp::Point levelSize = level->GetLevelSize();

	float xScale = window.GetWidth() / levelSize.x;
	float yScale = window.GetHeight() / levelSize.y;
	offsetX /= xScale;
	offsetY /= yScale;

	SDL2pp::Rect tempRect;

	//tempPoint = tempTile->GetWorldPosition();
	// Render shadow
	tempRect = SDL2pp::Rect((_position.GetX() / (float)level->GetTileWidth() * xScale) + offsetX, (_position.GetY() / (float)level->GetTileHeight() * yScale) + offsetY, xScale, yScale);
	rend.Copy(*_spriteShadow, SDL2pp::NullOpt, tempRect);

	// Render normal
	tempRect = SDL2pp::Rect((_position.GetX() / (float)level->GetTileWidth() * xScale), (_position.GetY() / (float)level->GetTileHeight() * yScale), xScale, yScale);
	rend.Copy(*_sprite, SDL2pp::NullOpt, tempRect);
}

