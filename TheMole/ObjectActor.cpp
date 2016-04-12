#include "ObjectActor.h"
#include "PlayerActor.h"
#include "GameScreen.h"

ObjectActor::ObjectActor(Vector2 position,
                         GameManager & manager,
                         Vector2 spd,
                         int id,
                         std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
                         const std::string&& startSprite,
                         std::unordered_map<std::string, std::pair<std::string, bool>> & sounds,
                         SpriteSheet::XAxisDirection startXDirection,
                         SpriteSheet::YAxisDirection startYDirection,
                         std::string dialog,
                         int numericIdentifier)
	: Actor(position, manager, spd, sprites, std::move(startSprite), sounds, startXDirection, startYDirection),
      _id(id),
      _collided(false),
      _dialog(dialog),
      _timer(0),
      _numericIdentifier(numericIdentifier)
{
	if (id == flag)
		_sprites[_currentSpriteSheet]->Pause();

	if (id == dialogTrigger)
	{
		SetVisibility(false);
		_font = new SDL2pp::Font(".\\Assets\\Fonts\\Exo-Regular.otf", 30);
		_dialogTexture = new SDL2pp::Texture(_mgr->GetRenderer(), _font->RenderText_Solid(_dialog, SDL_Color{ 255, 255, 255, 255 }));
	}
}

ObjectActor::ObjectActor(std::string serialised)
    : Actor(serialised),
    _collided(false),
    _timer(0)
{
    std::istringstream lineStream(serialised);
    std::string line;
    getline(lineStream, line);
    line.erase(std::remove(line.end() - 1, line.end(), '\r'), line.end());

    std::istringstream idStream(line);
    idStream >> _id;
    getline(lineStream, line);
    if (line.empty())
        _dialog = " ";
    else
        _dialog = line;

    getline(lineStream, line);
    std::istringstream numericIdStream(line);
    numericIdStream >> _numericIdentifier;
    
    serialised.erase(0, lineStream.tellg());
}

ObjectActor::~ObjectActor()
{
	if(_id == dialogTrigger && _font != nullptr)
		delete _font;

	if (_id == dialogTrigger && _dialogTexture != nullptr)
		delete _dialogTexture;
}

void ObjectActor::Draw(Camera & camera)
{
	Actor::Draw(camera);

	if (_id == dialogTrigger && _collided)
	{
		SDL2pp::Point size = GameManager::GetInstance()->GetWindow().GetSize();
		_mgr->GetRenderer().Copy(*_dialogTexture, SDL2pp::NullOpt, SDL2pp::Rect(size.GetX() * 0.50f - (_dialogTexture->GetWidth() / 2), size.GetY() * 0.9f, _dialogTexture->GetWidth(), _dialogTexture->GetHeight()));
	}
}

void ObjectActor::Update(double elapsedSecs)
{
	Actor::Update(elapsedSecs);
    if (_isDestroyed || !_isActive) return;
    _aabb.UpdatePosition(*this);

    if (_sprites[_currentSpriteSheet]->IsAtFirstFrame())
    {
        auto spriteName(_currentSpriteSheet);
        PlaySpriteSound(spriteName);
    }

	switch (_id)
	{
		case flag:
		{
			FlagUpdate(elapsedSecs);
			break;
		}
		case pancake:
		{
			PancakeUpdate(elapsedSecs);
			break;
		}
		case dialogTrigger:
		{
			DialogUpdate(elapsedSecs);
			break;
		}
		default:
		{
			break;
		}
	}
}

void ObjectActor::Reset(Vector2 pos)
{
	Actor::Reset(pos);
	_timer = 0;
	_dialog = " ";
	
	if(_id == dialogTrigger)
		_collided = false;

	if (_id == flag && _collided)
	{
		_sprites[_currentSpriteSheet]->SetReversed(true);
		_sprites[_currentSpriteSheet]->Stop();
	}
}

ObjectActor * ObjectActor::Clone()
{
    return new ObjectActor(*this);
}

bool ObjectActor::CollisionCheck(Actor & otherAI)
{
	return _aabb.CheckCollision(otherAI.GetAABB());
}

int ObjectActor::GetID()
{
	return _id;
}

void ObjectActor::SetID(int id)
{
	_id = id;
}

void ObjectActor::SetDialog(std::string d)
{
	_dialog = d;
	_dialogTexture = new SDL2pp::Texture(_mgr->GetRenderer(), _font->RenderText_Solid(_dialog, SDL_Color{ 255, 255, 255, 255 }));
}

std::string ObjectActor::GetDialog()
{
	return _dialog;
}

void ObjectActor::SetNumericID(int id)
{
	_numericIdentifier = id;
}

int ObjectActor::GetNumericID()
{
	return _numericIdentifier;
}

void ObjectActor::FlagUpdate(double elapsedSecs)
{
	if (!_collided && CollisionCheck(*(_gameScreen->GetPlayer())))
	{
		_sprites[_currentSpriteSheet]->Start();
		_sprites[_currentSpriteSheet]->SetRepeating(false);

		_gameScreen->GetLevel()->SetSpawnPoint(GetPosition(), _numericIdentifier);
		_gameScreen->GetSoundBank().PlaySound("checkpoint");
		_collided = true;
	}
}
 
void ObjectActor::PancakeUpdate(double elapsedSecs)
{
	if (!_collided && CollisionCheck(*(_gameScreen->GetPlayer())))
	{
		_sprites[_currentSpriteSheet]->Stop();
		SetVisibility(false);
		_gameScreen->GetLevel()->CollectPancake(_numericIdentifier);
        _gameScreen->GetSoundBank().PlaySound("eat");
		_collided = true;
	}
}

void ObjectActor::ProjectileThrowerUpdate(double elapseSecs)
{
	const std::shared_ptr<GameScreen> screen = std::dynamic_pointer_cast<GameScreen>(_mgr->GetCurrentScreen());
	if (CollisionCheck(*(screen->GetPlayer())))
	{
		_sprites[_currentSpriteSheet]->Stop();
		SetVisibility(false);
	}
}

void ObjectActor::DialogUpdate(double elapsedSecs)
{
	if (_collided)
	{
		_timer += elapsedSecs;
		if (_timer > 1.75)
		{
			_timer = 0;
			_collided = false;
		}
	}

	if (CollisionCheck(*(_gameScreen->GetPlayer())))
	{
		_collided = true;
	}
}

