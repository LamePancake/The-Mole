#include "SoundEffectBank.h"
using std::shared_ptr;

static SoundEffectBank* instance = nullptr;

void ChannelFinishedHandler(int channel)
{
    // God damn do I ever hate this...
    // Hopefully we never end up with multiple sound effect banks at the same time
    // Which we completely could since that was the point... Oh well
    instance->_channelSounds[channel] = "";
}

SoundEffectBank::SoundEffectBank(std::unordered_map<std::string, std::string>& soundFiles)
    : _mgr(nullptr), _sounds(), _channelSounds(), _soundFiles(soundFiles)
{
    instance = this;
}

void SoundEffectBank::LoadSounds()
{
    for (auto entry : _soundFiles)
    {
        _sounds[entry.first] = shared_ptr<SDL2pp::Chunk>(new SDL2pp::Chunk(entry.second));
    }
    _soundFiles.clear();

    _mgr = GameManager::GetInstance();
    _channelSounds.resize(_mgr->GetMixer().GetNumChannels(), "");
    
    // Set callback to use when a channel stops playing
    _mgr->GetMixer().SetChannelFinishedHandler(ChannelFinishedHandler);
}

void SoundEffectBank::PlaySound(std::string && name, bool repeat)
{
    try
    {
        int channel = _mgr->GetMixer().PlayChannel(-1, *_sounds[name], repeat ? -1 : 0);
        _channelSounds[channel] = name;
    }
    catch (SDL2pp::Exception & e)
    {
        // We've probably exceeded the number of channels available
        // This is pretty unlikely to happen in actual gameplay (I think) so we're just going to spit
        // out a warning and ignore it for now
        std::cerr << "Warning: no free sound channels!" << std::endl;
    }
}


void SoundEffectBank::StopSound(std::string && name)
{
    auto it = std::find_if(_channelSounds.begin(), _channelSounds.end(), [&](std::string& sound) {return sound == name; });
    if (it != _channelSounds.end())
    {
        _mgr->GetMixer().HaltChannel(it - _channelSounds.begin());
    }
}

bool SoundEffectBank::IsPlaying(std::string&& name)
{
    auto it = std::find_if(_channelSounds.begin(), _channelSounds.end(), [&](std::string& sound) {return sound == name; });
    return it != _channelSounds.end();
}
