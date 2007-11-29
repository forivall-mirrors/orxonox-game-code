#include "AudioBuffer.h"

namespace audio
{
	AudioBuffer::AudioBuffer(std::string fileName)
	{
		// Load wav data into buffers.
		alGenBuffers(1, &buffer);

		if(alGetError() != AL_NO_ERROR)
			loaded=AL_FALSE;

		alutLoadWAVFile((ALbyte*)fileName.c_str(), &format, &data, &size, &freq, &loop);
		alBufferData(buffer, format, data, size, freq);
		alutUnloadWAV(format, data, size, freq);

		// Do another error check and return.
		if(alGetError() != AL_NO_ERROR)
			loaded=AL_FALSE;
	
		loaded=AL_TRUE;
	}
	
	AudioBuffer::~AudioBuffer()
	{
		
	}
}
