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
	///<summary> Creates a new sound effect bank, which loads the requested sound effects.</summary>
	///
	///<param name="soundFiles"> A map of sound effect names to their repsective sound effect files.</param>
	///
	///### <author> Shane.</author>
	///### <date> 2/27/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	SoundEffectBank(std::unordered_map<std::string, std::string> & soundFiles);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///<summary> Plays the given sound effect.</summary>
	///
	///<param name="soundFiles"> The sound effect to play.</param>
	///
	///### <author> Shane.</author>
	///### <date> 2/27/2016.</date>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void PlaySound(const std::string & name);

private:
	GameManager* _mgr;
	std::unordered_map<std::string, std::shared_ptr<SDL2pp::Chunk>> _sounds;
};

#endif
