#include "Camera.h"

void Camera::Update(double elapsedSecs) {
	// Use our handy-dandy little reference to the player's position to adjust our position :-)
	// I suspect that there's a reason games don't do this. Not sure why not tho
	// Maybe it's because they don't want to be totally dependent on player position or something. What a weird concept that is
}

void Camera::CentreView(Vector2&& pos) {
	CentreView(SDL2pp::Point((int)pos.GetX(), (int)pos.GetY()));
}

void Camera::CentreView(const SDL2pp::Point& pos) {
	_viewport.x = pos.x - (_viewport.w / 2);
	_viewport.y = pos.y - (_viewport.h / 2);
	
	// Clamp the values to stay within the map
	if (_viewport.x < 0) _viewport.x = 0;
	else if (_viewport.x > _mapSize.x - _viewport.w) _viewport.x = _mapSize.x - _viewport.w;

	if (_viewport.y < 0) _viewport.y = 0;
	else if (_viewport.y >  _mapSize.y - _viewport.h) _viewport.y = _mapSize.y - _viewport.h;
}