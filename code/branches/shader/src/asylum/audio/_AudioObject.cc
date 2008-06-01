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
*      ...
*   Co-authors:
*      ...
*
*/


#include <iostream>
#include <string>

#include "core/Debug.h"
#include "AudioObject.h"

namespace audio
{
  AudioObject::AudioObject(std::string audioFile)
  {
    audioFile_ = audioFile;
    SourcePos[0]=0;
    SourcePos[1]=0;
    SourcePos[2]=0;

    SourceVel[0]=0;
    SourceVel[1]=0;
    SourceVel[2]=0;

    ListenerPos[0]=0;
    ListenerPos[1]=0;
    ListenerPos[2]=0;

    ListenerVel[0]=0;
    ListenerVel[1]=0;
    ListenerVel[2]=0;

    ListenerOri[0]=0;
    ListenerOri[1]=0;
    ListenerOri[2]=-1;
    ListenerOri[3]=0;
    ListenerOri[4]=1;
    ListenerOri[5]=0;


    // Initialize OpenAL and clear the error bit.

    alutInit(NULL, 0);
    alGetError();

    // Load the wav data.

    if(LoadALData() == AL_FALSE)
    {
      printf("Error loading sound data.");

    }

    SetListenerValues();
    COUT(3) << "Info: Play sone ambient background sound";
  }

  AudioObject::~AudioObject()
  {
    KillALData();
  }

  ALboolean AudioObject::LoadALData()
  {
    ALenum format;
    ALsizei size;
    ALvoid* data;
    ALsizei freq;
    ALboolean loop;


    alGenBuffers(1, &Buffer);

    if(alGetError() != AL_NO_ERROR)
      return AL_FALSE;

    alutLoadWAVFile((ALbyte*)audioFile_.c_str(), &format, &data, &size, &freq, &loop);
    alBufferData(Buffer, format, data, size, freq);
    alutUnloadWAV(format, data, size, freq);

    alGenSources(1, &Source);

    if(alGetError() != AL_NO_ERROR)
      return AL_FALSE;

    alSourcei (Source, AL_BUFFER,   Buffer   );
    alSourcef (Source, AL_PITCH,    1.0      );
    alSourcef (Source, AL_GAIN,     1.0      );
    alSourcefv(Source, AL_POSITION, SourcePos);
    alSourcefv(Source, AL_VELOCITY, SourceVel);
    alSourcei (Source, AL_LOOPING,  loop     );

    if(alGetError() == AL_NO_ERROR)
      return AL_TRUE;


    return AL_FALSE;
  }

  void AudioObject::SetListenerValues()
  {
    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);
  }

  void AudioObject::KillALData()
  {
    alDeleteBuffers(1, &Buffer);
    alDeleteSources(1, &Source);
    alutExit();
  }

  void AudioObject::play()
  {
    alSourcePlay(Source);
  }

}
