#include "Actor.h"

Actor::Actor(Vector2 position, GameManager & manager, Vector2 spd, std::string texturePath, std::string shadowTexturePath)
	:_position(position), _mgr(&manager), _speed(spd)
{
	_health = 100;
	
	_sprite = std::make_shared<SpriteSheet>(texturePath, 8, 1.0, SpriteSheet::XAxisDirection::RIGHT);
	_spriteShadow = std::make_shared<SpriteSheet>(shadowTexturePath, 8, 1.0, SpriteSheet::XAxisDirection::RIGHT);

	_aabb = AABB(64, 64, *this);

	SDL_SetTextureColorMod(_spriteShadow->GetTexture().Get(), 127, 127, 127);
	SDL_SetTextureAlphaMod(_spriteShadow->GetTexture().Get() , 127);
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
	return _sprite;
}

std::shared_ptr<SpriteSheet> Actor::GetTextureShadow()
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

void Actor::Update(double elapsedSecs, std::shared_ptr<Level>& level)
{
	_sprite->Update(elapsedSecs);
	_spriteShadow->Update(elapsedSecs);
}

void Actor::UpdatePosition()
{
}

void Actor::Draw(Camera& camera)
{
	const SDL2pp::Rect& viewport = camera.GetViewport();
	int offsetX = 4;
	int offsetY = 0;
			
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	SDL2pp::Point tempPoint;

	tempPoint = { (int)_position.GetX(), (int)_position.GetY() };

	_spriteShadow->Draw(tempPoint + SDL2pp::Point(offsetX - viewport.x, offsetY - viewport.y), SpriteSheet::XAxisDirection::RIGHT);
	_sprite->Draw(tempPoint + SDL2pp::Point(-viewport.x, -viewport.y), SpriteSheet::XAxisDirection::RIGHT);
}

