/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Nicolas Perrenoud <nicolape@ee.ethz.ch>
 *   Co-authors:
 *      ...
 *
 */

#ifndef _AudioManager_H__
#define _AudioManager_H__

#include "AudioPrereqs.h"

#include <vector>
#include <string>

#include "core/Tickable.h"
#include "AudioBuffer.h"
#include "AudioSource.h"
#include "AudioStream.h"

namespace audio
{
  class _AudioExport AudioManager : public orxonox::Tickable
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
		void tick(float dt);

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

#endif /* _AudioManager_H__ */
