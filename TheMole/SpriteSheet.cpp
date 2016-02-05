#include "SpriteSheet.h"
#include "GameManager.h"

using namespace SDL2pp;

SpriteSheet::SpriteSheet(std::string& filename, int numFrames, double duration, XAxisDirection defaultFacingDirection) :
	_sheet(GameManager::GetInstance()->GetRenderer(), filename), _frameTime(duration / numFrames), _currentFrame(0), _numFrames(numFrames),
	_currentFrameElapsed(0), _defaultFacing(defaultFacingDirection) {

	_spriteSize = { (int)_sheet.GetWidth() / _numFrames, (int)_sheet.GetHeight() };
	_mgr = GameManager::GetInstance();
}

void SpriteSheet::Update(double elapsedTime) {
	_currentFrameElapsed += elapsedTime;
	if (_currentFrameElapsed >= _frameTime) {
		_currentFrameElapsed = 0;
		_currentFrame++;
		_currentFrame %= _numFrames;
	}
}

void SpriteSheet::Draw(const SDL2pp::Point&& position, XAxisDirection facingDir) {
	int renderFlags = facingDir == _defaultFacing ? 0 : SDL_FLIP_HORIZONTAL;
	Renderer& rend = _mgr->GetRenderer();
	Rect frameRect((int)(_currentFrame * _spriteSize.x), 0, _spriteSize.x, _spriteSize.x);
	Rect screenRect(position, _spriteSize);
	rend.Copy(_sheet, frameRect, screenRect, 0, NullOpt, renderFlags);
}

void SpriteSheet::Reset() {
	_currentFrame = 0;
	_currentFrameElapsed = 0;
}
