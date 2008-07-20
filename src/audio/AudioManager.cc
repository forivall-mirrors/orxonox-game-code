/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Nicolas Perrenoud <nicolape_at_ee.ethz.ch>
 *   Co-authors:
 *      ...
 *
 */

#include "AudioManager.h"

#include <cstdlib>

#include <AL/alut.h>

#include "AudioBuffer.h"
#include "AudioSource.h"
#include "AudioStream.h"
#include "core/Error.h"
#include "core/Debug.h"

namespace audio
{
	AudioManager::AudioManager()
	{
    ambientPath = "audio/ambient";

    alutInit(NULL, 0);


	}

	AudioManager::~AudioManager()
	{
		for (unsigned int i=0;i<bgSounds.size();i++)
		{
			bgSounds[i]->release();
		}
		alutExit();
	}

	void AudioManager::ambientStart()
	{
// 		currentBgSound = 0;
		if (bgSounds.size() > 0)
		{
      currentBgSound = rand() % bgSounds.size();
			if(!bgSounds[currentBgSound]->playback())
			{
    		orxonox::Error("Ogg refused to play.");
			}
			else
			{
				COUT(3) << "Info: Started playing background sound" << std::endl;
			}
		}
	}

	void AudioManager::ambientStop()
	{
		COUT(3) << "Info: Stopped playing background sound" << std::endl;
	}

	void AudioManager::ambientAdd(std::string file)
	{
    std::string path = ambientPath + "/" + file + ".ogg";
		AudioStream* tmp = new AudioStream(path);
		tmp->open();
		if (tmp->isLoaded())
		{
			bgSounds.push_back(tmp);
			COUT(3) << "Info: Added background sound " << file << std::endl;
		}
	}

	void AudioManager::tick(float dt)
	{
		if (bgSounds.size() > 0)
		{
			if (bgSounds[currentBgSound]->isLoaded())
			{
				bool playing = bgSounds[currentBgSound]->update();
		    if(!bgSounds[currentBgSound]->playing() && playing)
		    {
		        if(!bgSounds[currentBgSound]->playback())
		            orxonox::Error("Ogg abruptly stopped.");
		        else
		            orxonox::Error("Ogg stream was interrupted.");

		    }
				if (!playing)
				{
// 					if (currentBgSound < bgSounds.size()-1)
// 					{
// 						currentBgSound++;
// 					}
// 					else
// 					{
// 						currentBgSound=0;
// 					}
          // switch to next sound in list/array
          currentBgSound = ++currentBgSound % bgSounds.size();

					if (!bgSounds[currentBgSound]->isLoaded())
					{
						bgSounds[currentBgSound]->release();
						bgSounds[currentBgSound]->open();
					}
					bgSounds[currentBgSound]->playback();
					COUT(3) << "Info: Playing next background sound" << std::endl;
				}
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
