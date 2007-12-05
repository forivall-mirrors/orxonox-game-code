#include "AudioManager.h"

namespace audio
{
	AudioManager::AudioManager()
	{
 		alutInit(NULL, 0);

		bgSound.open("ambient3");
		bgSound.display();
		
		if(!bgSound.playback())
		{
    	std::cout << "Ogg refused to play.";
		}
	}
		
	AudioManager::~AudioManager()
	{
		bgSound.release();
		alutExit();
	}
	
	void AudioManager::update()
	{
		bgSound.update();
    if(!bgSound.playing())
    {
        if(!bgSound.playback())
            std::cout << "Ogg abruptly stopped.";
        else
            std::cout << "Ogg stream was interrupted.\n";
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
