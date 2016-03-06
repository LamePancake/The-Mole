#include "SpriteSheet.h"
#include "GameManager.h"
#include "Vector2.h"

#define RAD_TO_DEG(RAD) (((RAD) * 360) / (2 * M_PI))
#define DEG_TO_RAD(DEG) (((DEG) * (2 * M_PI)) / 360)

using namespace SDL2pp;

SpriteSheet::SpriteSheet(std::string&& filename, int numFrames, double duration, bool isRepeating, XAxisDirection defaultXAxisDirection, YAxisDirection defaultYAxisDirection) 
	: SpriteSheet(std::make_shared<SDL2pp::Texture>(GameManager::GetInstance()->GetRenderer(), filename), numFrames, duration, isRepeating, defaultXAxisDirection, defaultYAxisDirection)
{}

SpriteSheet::SpriteSheet(std::shared_ptr<SDL2pp::Texture>& texture, int numFrames, double duration, bool isRepeating, XAxisDirection defaultXAxisDirection, YAxisDirection defaultYAxisDirection)
	: _sheet(texture),
	_numFrames(numFrames),
	_frameTime(duration / numFrames),
	_isRepeating(isRepeating),
	_defaultXDir(defaultXAxisDirection),
	_currentXDir(_defaultXDir),
	_defaultYDir(defaultYAxisDirection),
	_currentYDir(_defaultYDir),
	_isRunning(true),
	_isReversed(false),
	_currentFrame(0),
	_currentFrameElapsed(0),
	_colourMod({ 255, 255, 255, 255 }),
	_rotation(0),
	_scale(1.0),
	_mgr(GameManager::GetInstance())
{
	_spriteSize = { (int)_sheet->GetWidth() / _numFrames, (int)_sheet->GetHeight() };
}

void SpriteSheet::Update(double elapsedTime) {
	if (!_isRunning) return;

	_currentFrameElapsed += elapsedTime;
	if (_currentFrameElapsed >= _frameTime) {
		_currentFrameElapsed = 0;

		int sentinelFrame; // One past the end of the animation in either direction
		int finalFrame; // The actual valid last frame

		if (_isReversed)
		{
			sentinelFrame = -1;
			finalFrame = 0;
			_currentFrame--;
		}
		else
		{
			sentinelFrame = _numFrames;
			finalFrame = _numFrames - 1;
			_currentFrame++;
		}

		// Stop the animation at the last frame, if it is not repeating
		if (!_isRepeating && _currentFrame == finalFrame)
		{
			Pause();
			_currentFrame = finalFrame;
			return;
		}

		_currentFrame %= _numFrames;
	}
}

void SpriteSheet::Draw(const SDL2pp::Point& position)
{
	// Determine which axes to flip the sprite on, if any
	int renderFlags = 0;
	renderFlags |= (_currentXDir == _defaultXDir ? 0 : SDL_FLIP_HORIZONTAL);
	renderFlags |= (_currentYDir == _defaultYDir ? 0 : SDL_FLIP_VERTICAL);

	Renderer& rend = _mgr->GetRenderer();

	Vector2 newSize(_spriteSize.x * _scale, _spriteSize.y * _scale);
	Vector2 newPos(position.x - (newSize.GetX() - _spriteSize.x), position.y - (newSize.GetY() - _spriteSize.y));
	
	// Determine the destination frame rect
	Rect frameRect((int)(_currentFrame * _spriteSize.x), 0, _spriteSize.x, _spriteSize.y);
	Rect screenRect(newPos.GetX(), newPos.GetY(), newSize.GetX(), newSize.GetY());

	SDL_SetTextureColorMod(_sheet->Get(), _colourMod.r, _colourMod.g, _colourMod.b);
	SDL_SetTextureAlphaMod(_sheet->Get(), _colourMod.a);

	rend.Copy(*_sheet, frameRect, screenRect, _rotation, NullOpt, renderFlags);
}

void SpriteSheet::Draw(const SDL2pp::Point&& position)
{
	// Convert to lvalue reference, lol
	Draw(position);
}

bool SpriteSheet::IsAnimating() const
{
	return _isRunning;
}

bool SpriteSheet::IsFinished() const
{
	if (_isRepeating) return false;

	return (_isReversed && _currentFrame == 0) || (_currentFrame == _numFrames - 1);
}

void SpriteSheet::Start()
{
	_isRunning = true;
}

void SpriteSheet::Pause()
{
	_isRunning = false;
}

void SpriteSheet::Stop()
{
	Pause();
	Reset();
}

void SpriteSheet::Reset() {
	_currentFrame = _isReversed ? _numFrames - 1 : 0;
	_currentFrameElapsed = 0;
}

int SpriteSheet::GetFrameWidth()
{
	return _spriteSize.x;
}

int SpriteSheet::GetFrameHeight()
{
	return _spriteSize.y;
}

SDL2pp::Texture & SpriteSheet::GetTexture()
{
	return *_sheet;
}

bool SpriteSheet::IsRepeating()
{
	return _isRepeating;
}

void SpriteSheet::SetRepeating(bool repeating)
{
	_isRepeating = repeating;
}

bool SpriteSheet::IsReversed() const
{
	return _isReversed;
}

void SpriteSheet::SetReversed(bool reversed)
{
	_isReversed = reversed;
}

void SpriteSheet::SetXAxisDirection(XAxisDirection direction)
{
	_currentXDir = direction;
}

SpriteSheet::XAxisDirection SpriteSheet::GetXAxisDirection() const
{
	return _currentXDir;
}

void SpriteSheet::SetYAxisDirection(YAxisDirection direction)
{
	_currentYDir = direction;
}

SpriteSheet::YAxisDirection SpriteSheet::GetYAxisDirection() const
{
	return _currentYDir;
}

void SpriteSheet::SetRotation(double angle)
{
	_rotation = angle;
}

double SpriteSheet::GetRotation() const
{
	return _rotation;
}

void SpriteSheet::SetColourMod(SDL_Color colourMod)
{
	_colourMod = colourMod;
}

SDL_Color SpriteSheet::GetColourMod() const
{
	return _colourMod;
}

void SpriteSheet::SetScale(double scale)
{
	_scale = scale;
}

double SpriteSheet::GetScale() const
{
	return _scale;
}
