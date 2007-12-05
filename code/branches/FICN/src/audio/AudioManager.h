#ifndef AUDIOMANAGER_H_
#define AUDIOMANAGER_H_

#include <iostream>
#include <string>
#include <vector>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "AudioBuffer.h"
#include "AudioSource.h"

namespace audio
{
	class AudioManager
	{
	public:
		// Init audio 
		AudioManager();
		// Kill audio and set buffers, sources and memory free
		~AudioManager();
		// Set listener position
		void setPos(std::vector<float> newPosition);
		// Set listener speed
		void setSpeed(std::vector<float> newSpeed);
		// Set listener orientation (first is direction
		// the listener looks at, the second is the direction
		// upwards the listener)
		void setOri(std::vector<float> at, std::vector<float> up);
		// Parses given xml string
		void loadParams();
	private:

	
		// Vector containing all audio files
		std::vector<AudioBuffer> buffers;
		// Vector containing all audio sources which referer to one buffer
		std::vector<AudioSource> sources;
		// The ambient background sound 	
		AudioSource ambient;
		
		std::vector<float> listenerPosition;
		std::vector<float> listenerSpeed;
		std::vector<float> listenerAt;
		std::vector<float> listenerup;		
	};	
}

#endif /*AUDIOMANAGER_H_*/
