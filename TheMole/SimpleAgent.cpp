#include "SimpleAgent.h"
#include "Tile.h"

SimpleAgent::SimpleAgent()
{
}


SimpleAgent::~SimpleAgent()
{
}

SimpleAgent::SimpleAgent(Vector2 position, GameManager & manager, Vector2 spd)
{
	_speed = spd;
	_health = 100;
	_position = position;
	_mgr = &manager;
	// This is temporary so we can test whether AABB can be created from texture dimensions
	_sprite = new SDL2pp::Texture(_mgr->GetRenderer(), ".\\Assets\\Textures\\mole.png");
	_aabb = AABB(*_sprite, *this);
}

void SimpleAgent::Unload()
{
	delete _sprite;
}

SDL2pp::Texture* SimpleAgent::GetTexture()
{
	return _sprite;
}

void SimpleAgent::Update(std::shared_ptr<Level> & level)
{
	// While the AI is alive, do stuff.

	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	if (_health <= 0)
	{
		//std::cout << "Dead\n";
	}
	if (_health > 0)
	{
		//std::cout << "Patrolling\n";
	}

	//_aabb.UpdatePosition(*this);
	UpdatePosition();
	ScanNeighbouringTiles(level);
	/*
	switch (keys) {
	case 0:
		_mgr->SetNextScreen("level1");
		return SCREEN_FINISH;
	case 1:
		_mgr->SetNextScreen("settings");
		return SCREEN_OVERLAY;
	case 2:
		// We just tried to quit
		// So for now we're going to do it the dirty way
		exit(0);
	}*/
}

void SimpleAgent::UpdatePosition()
{
	_position.SetX(_position.GetX() + _speed.GetX());
	_position.SetY(_position.GetY() + _speed.GetY());
	std::cout << GetPosition().GetX() << ", " << GetPosition().GetY() << "AI1\n";
}

Vector2 SimpleAgent::GetPosition()
{
	return _position;
}

void SimpleAgent::SetSpeed(Vector2 spd)
{
	_speed = spd;
}

bool SimpleAgent::CollisionCheck(SimpleAgent &otherAI)
{
	return _aabb.CheckCollision(otherAI.GetAABB());
}

AABB SimpleAgent::GetAABB()
{
	return _aabb;
}

void SimpleAgent::ScanNeighbouringTiles(std::shared_ptr<Level> & level)
{
	int tileWidth  = level->GetTileWidth();
	int tileHeight = level->GetTileHeight();

	if ((int)_position.GetX() % tileWidth == 0)
	{
		int xInd = _position.GetX() / tileWidth;
		int yInd = _position.GetY() / tileHeight;
		
		if (xInd + 1 < level->GetLevelSize().GetX() && _speed.GetX() > 0.0f)
		{
			if (level->GetTileFromLevel(xInd + 1, yInd)->GetID() != Tile::blank)
			{
				_speed.SetX(_speed.GetX() * -1.0f);
			}
		}
		else if (xInd - 1 >= 0 && _speed.GetX() < 0.0f)
		{
			if (level->GetTileFromLevel(xInd - 1, yInd)->GetID() != Tile::blank)
			{
				_speed.SetX(_speed.GetX() * -1.0f);
			}
		}
	}
}

void SimpleAgent::Draw(std::shared_ptr<Level> & level)
{
	SDL2pp::Renderer& rend = _mgr->GetRenderer();
	SDL2pp::Window& window = _mgr->GetWindow();
	SDL2pp::Point levelSize = level->GetLevelSize();

	float xScale = window.GetWidth() / levelSize.x;
	float yScale = window.GetHeight() / levelSize.y;

	SDL2pp::Rect tempRect;

	//tempPoint = tempTile->GetWorldPosition();
	tempRect = SDL2pp::Rect(_position.GetX() / (float)level->GetTileWidth() * xScale, _position.GetY() / (float)level->GetTileHeight() * yScale, xScale, yScale);
	rend.Copy(*_sprite, SDL2pp::NullOpt, tempRect);

}
