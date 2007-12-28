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


#include "AudioStream.h"
#include "orxonox/core/Debug.h"

namespace audio
{
	AudioStream::AudioStream(orxonox::String path)
	{
		this->path = path;
		loaded = false;
	}

	void AudioStream::open()
	{
	    //int result;
      errno_t result;


	    if(fopen_s(&oggFile, path.c_str(), "rb"))
			{
	    	orxonox::Error("Could not open Ogg file "+path);
				return;
			}
      else
      {
        COUT(4) << "Opened Ogg file" << path << std::endl;
      }

	    /*if((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
	    {
        fclose(oggFile);
	      orxonox::Error("Could not open Ogg stream. " + errorString(result));
				return;
	    }*/

			loaded = true;

	    vorbisInfo = ov_info(&oggStream, -1);
	    vorbisComment = ov_comment(&oggStream, -1);

	    if(vorbisInfo->channels == 1)
	        format = AL_FORMAT_MONO16;
	    else
	        format = AL_FORMAT_STEREO16;


	    alGenBuffers(2, buffers);
	    check();
	    alGenSources(1, &source);
	    check();

	    alSource3f(source, AL_POSITION,        0.0, 0.0, 0.0);
	    alSource3f(source, AL_VELOCITY,        0.0, 0.0, 0.0);
	    alSource3f(source, AL_DIRECTION,       0.0, 0.0, 0.0);
	    alSourcef (source, AL_ROLLOFF_FACTOR,  0.0          );
	    alSourcei (source, AL_SOURCE_RELATIVE, AL_FALSE      );
	}




	void AudioStream::release()
	{

	    alSourceStop(source);
	    empty();
	    alDeleteSources(1, &source);
	    check();
	    alDeleteBuffers(1, buffers);
	    check();

	    ov_clear(&oggStream);
			loaded = false;

	}




	void AudioStream::display()
	{
		if (loaded)
		{
	    COUT(3)
	        << "version         " << vorbisInfo->version         << std::endl
	        << "channels        " << vorbisInfo->channels        << std::endl
	        << "rate (hz)       " << vorbisInfo->rate            << std::endl
	        << "bitrate upper   " << vorbisInfo->bitrate_upper   << std::endl
	        << "bitrate nominal " << vorbisInfo->bitrate_nominal << std::endl
	        << "bitrate lower   " << vorbisInfo->bitrate_lower   << std::endl
	        << "bitrate window  " << vorbisInfo->bitrate_window  << std::endl
	        << std::endl
	        << "vendor " << vorbisComment->vendor << std::endl;

	    for(int i = 0; i < vorbisComment->comments; i++)
	        COUT(3) << "   " << vorbisComment->user_comments[i] << std::endl;

	    COUT(3) << std::endl;
		}
	}




	bool AudioStream::playback()
	{
		if (!loaded)
		{
			return false;
		}

	    if(playing())
	        return true;

	    if(!stream(buffers[0]))
	        return false;

	    if(!stream(buffers[1]))
	        return false;

	    alSourceQueueBuffers(source, 2, buffers);
	    alSourcePlay(source);

	    return true;
	}




	bool AudioStream::playing()
	{
		if (!loaded)
		{
			return false;
		}

	    ALenum state;
	    alGetSourcei(source, AL_SOURCE_STATE, &state);
	    return (state == AL_PLAYING);
	}




	bool AudioStream::update()
	{
	    int processed;
	    bool active = true;

	    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

	    while(processed--)
	    {
	        ALuint buffer;

	        alSourceUnqueueBuffers(source, 1, &buffer);
	        check();

	        active = stream(buffer);

	        alSourceQueueBuffers(source, 1, &buffer);
	        check();
	    }

			if (active==false)
			{
				loaded = false;
			}
	    return active;
	}




	bool AudioStream::stream(ALuint buffer)
	{
	    char pcm[BUFFER_SIZE];
	    int  size = 0;
	    int  section;
	    int  result;

	    while(size < BUFFER_SIZE)
	    {
	        result = ov_read(&oggStream, pcm + size, BUFFER_SIZE - size, 0, 2, 1, &section);

	        if(result > 0)
	            size += result;
	        else
	            if(result < 0)
	                orxonox::Error(errorString(result));
	            else
	                break;
	    }

	    if(size == 0)
	        return false;

	    alBufferData(buffer, format, pcm, size, vorbisInfo->rate);
	    check();

	    return true;
	}



	void AudioStream::empty()
	{
	    int queued;

	    alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);

	    while(queued--)
	    {
	        ALuint buffer;

	        alSourceUnqueueBuffers(source, 1, &buffer);
	        check();
	    }
	}




	void AudioStream::check()
	{
		int error = alGetError();

		if(error != AL_NO_ERROR)
			orxonox::Error("OpenAL error was raised.");
	}



	orxonox::String AudioStream::errorString(int code)
	{
	    switch(code)
	    {
	        case OV_EREAD:
	            return orxonox::String("Read from media.");
	        case OV_ENOTVORBIS:
	            return orxonox::String("Not Vorbis data.");
	        case OV_EVERSION:
	            return orxonox::String("Vorbis version mismatch.");
	        case OV_EBADHEADER:
	            return orxonox::String("Invalid Vorbis header.");
	        case OV_EFAULT:
	            return orxonox::String("Internal logic fault (bug or heap/stack corruption.");
	        default:
	            return orxonox::String("Unknown Ogg error.");
	    }
	}
}

