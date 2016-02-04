#include "Actor.h"

Actor::Actor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath)
	:_position(position), _mgr(&manager), _speed(spd)
{
	_health = 100;
	// This is temporary so we can test whether AABB can be created from texture dimensions
	_sprite = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), texturePath);
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

void Actor::Draw(std::shared_ptr<Level>& level, Camera& camera)
{
	const SDL2pp::Rect& viewport = camera.GetViewport();
	float offsetX = 25.0f;
	float offsetY = 25.0f;
			
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	SDL2pp::Window& window = _mgr->GetWindow();
	SDL2pp::Point levelSize = level->GetLevelSize();

	SDL2pp::Rect tempRect;

	//tempPoint = tempTile->GetWorldPosition();
	// Render shadow
	tempRect = SDL2pp::Rect(_position.GetX() + offsetX - viewport.x, _position.GetY() + offsetY - viewport.y, _sprite->GetWidth(), _sprite->GetHeight());
	rend.Copy(*_sprite, SDL2pp::NullOpt, tempRect);

	// Render normal
	tempRect = SDL2pp::Rect(_position.GetX() - viewport.x, _position.GetY() - viewport.y, _sprite->GetWidth(), _sprite->GetHeight());
	rend.Copy(*_sprite, SDL2pp::NullOpt, tempRect);
}

