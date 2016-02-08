#include "SpriteSheet.h"
#include "GameManager.h"

using namespace SDL2pp;

SpriteSheet::SpriteSheet(std::string&& filename, int numFrames, double duration, bool isRepeating, XAxisDirection defaultXAxisDirection, YAxisDirection defaultYAxisDirection) 
	: SpriteSheet(std::make_shared<SDL2pp::Texture>(GameManager::GetInstance()->GetRenderer(), filename), numFrames, duration, isRepeating, defaultXAxisDirection, defaultYAxisDirection)
{}

SpriteSheet::SpriteSheet(std::shared_ptr<SDL2pp::Texture>& texture, int numFrames, double duration, bool isRepeating, XAxisDirection defaultXAxisDirection, YAxisDirection defaultYAxisDirection)
	: _sheet(texture), _numFrames(numFrames), _frameTime(duration / numFrames), _isRepeating(isRepeating), _defaultXDir(defaultXAxisDirection), _defaultYDir(defaultYAxisDirection), _isRunning(true),
	_currentFrame(0), _currentFrameElapsed(0)
{
	_spriteSize = { (int)_sheet->GetWidth() / _numFrames, (int)_sheet->GetHeight() };
	_mgr = GameManager::GetInstance();
}

void SpriteSheet::Update(double elapsedTime) {
	if (!_isRunning) return;

	_currentFrameElapsed += elapsedTime;
	if (_currentFrameElapsed >= _frameTime) {
		_currentFrameElapsed = 0;
		_currentFrame++;

		// Stop the animation at the last frame, if it is not repeating
		if (!_isRepeating && _currentFrame == _numFrames)
		{
			Stop();
			return;
		}

		_currentFrame %= _numFrames;
	}
}

void SpriteSheet::Draw(const SDL2pp::Point&& position, XAxisDirection xAxisDir, YAxisDirection yAxisDir) {
	// Determine which axes to flip the sprite on, if any
	int renderFlags = 0;
	renderFlags |= (xAxisDir == _defaultXDir ? 0 : SDL_FLIP_HORIZONTAL);
	renderFlags |= (yAxisDir == _defaultYDir ? 0 : SDL_FLIP_VERTICAL);

	Renderer& rend = _mgr->GetRenderer();
	Rect frameRect((int)(_currentFrame * _spriteSize.x), 0, _spriteSize.x, _spriteSize.x);
	Rect screenRect(position, _spriteSize);
	rend.Copy(*_sheet, frameRect, screenRect, 0, NullOpt, renderFlags);
}

bool SpriteSheet::IsAnimating() const
{
	return _isRunning;
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
	_currentFrame = 0;
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
