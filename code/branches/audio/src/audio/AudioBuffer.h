#ifndef AUDIOBUFFER_H_
#define AUDIOBUFFER_H_

#include <iostream>
#include <string>
#include <vector>

#include "../../include/AL/al.h"
#include "../../include/AL/alc.h"
#include "../../include/AL/alut.h"

namespace audio
{
	class AudioBuffer
	{
	public:
		AudioBuffer(std::string fileName);
		~AudioBuffer();
	private:
		// Buffers hold sound data.
		ALuint buffer;
		// Identifier
		std::string name;
		// True if AL was able to load data
		ALboolean loaded;
		
		ALenum format;
		ALsizei size;
		ALvoid* data;
		ALsizei freq;
		ALboolean loop;
	};
}

#endif /*AUDIOBUFFER_H_*/
