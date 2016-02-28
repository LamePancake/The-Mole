#include "SoundEffectBank.h"
using std::shared_ptr;

SoundEffectBank::SoundEffectBank(std::unordered_map<std::string, std::string>& soundFiles)
	: _mgr(nullptr), _sounds()
{
	for (auto entry : soundFiles)
	{
		_sounds[entry.first] = shared_ptr<SDL2pp::Chunk>(new SDL2pp::Chunk(entry.second));
	}
}

void SoundEffectBank::PlaySound(const std::string & name)
{
	// Ugly hack since GameManager is actually initialised after this object...
	if (!_mgr) _mgr = GameManager::GetInstance();
	_mgr->GetMixer().PlayChannel(-1, *_sounds[name], 0);
}
