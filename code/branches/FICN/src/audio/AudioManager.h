#ifndef AUDIOMANAGER_H_
#define AUDIOMANAGER_H_

#include "AudioIncludes.h"

#include "AudioBuffer.h"
#include "AudioSource.h"
#include "AudioStream.h"

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

		// Update
		void update();

		void ambientAdd(std::string file);
		void ambientStart();
		void ambientStop();

	private:

		// Background sound
    std::vector<AudioStream> bgSounds;
		int currentBgSound;



		std::string ambientPath;
	
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
