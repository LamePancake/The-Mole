#ifndef SOUND_BANK_H
#define SOUND_BANK_H

#include <unordered_map>
#include <memory>
#include <string>

#include "GameManager.h"

class SoundEffectBank
{
public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Creates a new sound effect bank with a list of sounds to load.</summary>
	///
	///<param name="soundFiles"> A map of sound effect names to their repsective sound effect files.</param>
	///
	///### <author> Shane.</author>
	///### <date> 2/27/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	SoundEffectBank(std::unordered_map<std::string, std::string> & soundFiles);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Actually loads the sound effects specified in the lists.</summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void LoadSounds();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Plays the given sound effect.</summary>
	///
	///<param name="name"> The sound effect to play.</param>
    ///<param name="repeating"> Whether the sound should be repeating (defaults to false).</param>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void PlaySound(std::string && name, bool repeating = false);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Stops a sound. If the sound wasn't playing, this does nothing.</summary>
    ///
    ///<param name="name"> The sound to stop.</param>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void StopSound(std::string && name);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///<summary> Determines whether a given sound is playing.</summary>
    ///
    ///<param name="soundFiles"> The sound effect to check.</param>
    ///<return>Whether the given sound effect is playing.</return>
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    bool IsPlaying(std::string&& name);

private:
	GameManager* _mgr;
    std::unordered_map<std::string, std::string> _soundFiles;
	std::unordered_map<std::string, std::shared_ptr<SDL2pp::Chunk>> _sounds;
    std::vector<std::string> _channelSounds;

    friend void ChannelFinishedHandler(int channel);
};

#endif
