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
		ALuint data;
		// Identifier
		std::string name;
	};
}

#endif /*AUDIOBUFFER_H_*/
