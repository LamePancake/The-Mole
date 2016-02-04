#include "Actor.h"

Actor::Actor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath, std::string shadowTexturePath)
	:_position(position), _mgr(&manager), _speed(spd)
{
	_health = 100;
	// This is temporary so we can test whether AABB can be created from texture dimensions
	_sprite = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), texturePath);
	_spriteShadow = std::make_shared<SDL2pp::Texture>(_mgr->GetRenderer(), shadowTexturePath);
	_aabb = AABB(*_sprite, *this);

	SDL_SetTextureColorMod(_spriteShadow->Get(), 127, 127, 127);
	SDL_SetTextureAlphaMod(_spriteShadow->Get(), 127);
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

void Actor::Draw(Camera& camera)
{
	const SDL2pp::Rect& viewport = camera.GetViewport();
	int offsetX = 3;
	int offsetY = 3;
			
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	SDL2pp::Point tempPoint;

	tempPoint = { (int)_position.GetX(), (int)_position.GetY() };

	// Render shadow
	rend.Copy(*_spriteShadow, SDL2pp::NullOpt, tempPoint + SDL2pp::Point(offsetX - viewport.x, offsetY - viewport.y));

	// Render normal
	rend.Copy(*_sprite, SDL2pp::NullOpt, tempPoint + SDL2pp::Point(-viewport.x, -viewport.y));
}

