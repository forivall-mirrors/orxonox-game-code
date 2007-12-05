#include "AudioManager.h"

namespace audio
{
	AudioManager::AudioManager()
	{
 		alutInit(NULL, 0);

		bgSound.open("ambient1");



		bgSound.display();

		if(!bgSound.playback())
		{
    	orxonox::Error("Ogg refused to play.");
		}
	}
		
	AudioManager::~AudioManager()
	{
		bgSound.release();
		alutExit();
	}
	
	void AudioManager::update()
	{
		
		if (bgSound.isLoaded())
		{
			bgSound.update();
	    if(!bgSound.playing())
	    {
	        if(!bgSound.playback())
	            orxonox::Error("Ogg abruptly stopped.");
	        else
	            orxonox::Error("Ogg stream was interrupted.");
	    }
		}
	}
	
	void AudioManager::setPos(std::vector<float> newPosition)
	{
		
	}

	void AudioManager::setSpeed(std::vector<float> newSpeed)
	{
		
	}

	void AudioManager::setOri(std::vector<float> at, std::vector<float> up)
	{
		
	}
}
