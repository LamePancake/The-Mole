#include "SpriteSheet.h"
#include "GameManager.h"

using namespace SDL2pp;

SpriteSheet::SpriteSheet(std::string& filename, int numFrames, double duration, XAxisDirection defaultFacingDirection) :
	_sheet(GameManager::GetInstance()->GetRenderer(), filename), _frameTime(duration / numFrames), _currentFrame(0), _numFrames(numFrames),
	_currentFrameElapsed(0), _defaultFacing(defaultFacingDirection), _isRunning(true), _isRepeating(true), _toDraw(true) {

	_spriteSize = { (int)_sheet.GetWidth() / _numFrames, (int)_sheet.GetHeight() };
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
			_isRunning = false;
			_currentFrame--;
			return;
		}

		_currentFrame %= _numFrames;
	}
}

void SpriteSheet::Draw(const SDL2pp::Point&& position, XAxisDirection facingDir) {
	if (!_toDraw) return;

	int renderFlags = facingDir == _defaultFacing ? 0 : SDL_FLIP_HORIZONTAL;
	Renderer& rend = _mgr->GetRenderer();
	Rect frameRect((int)(_currentFrame * _spriteSize.x), 0, _spriteSize.x, _spriteSize.x);
	Rect screenRect(position, _spriteSize);
	rend.Copy(_sheet, frameRect, screenRect, 0, NullOpt, renderFlags);
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
	return _sheet;
}

void SpriteSheet::SetDraw(bool draw)
{
	_toDraw = draw;
}

bool SpriteSheet::CanDraw()
{
	return _toDraw;
}

bool SpriteSheet::IsRepeating()
{
	return _isRepeating;
}

void SpriteSheet::SetRepeating(bool repeating)
{
	_isRepeating = repeating;
}
