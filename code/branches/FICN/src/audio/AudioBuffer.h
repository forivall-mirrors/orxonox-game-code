#ifndef _AudioBuffer_H__
#define _AudioBuffer_H__

#include "AudioIncludes.h"

namespace audio
{
	class AudioBuffer
	{
	public:
		AudioBuffer(orxonox::String fileName);
		~AudioBuffer();
	private:
		// Buffers hold sound data.
		ALuint buffer;
		// Identifier
		orxonox::String name;
		// True if AL was able to load data
		ALboolean loaded;
		
		ALenum format;
		ALsizei size;
		ALvoid* data;
		ALsizei freq;
		ALboolean loop;
	};
}

#endif /* _AudioBuffer_H__ */
