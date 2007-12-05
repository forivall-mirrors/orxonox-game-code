
#include "AudioStream.h"

namespace audio
{
	void AudioStream::open(std::string path)
	{
	    int result;
			loaded = false;
	    

	    path = "audio/ambient/" + path + ".ogg"; 
	    
	    if(!(oggFile = fopen(path.c_str(), "rb")))
			{
	    	orxonox::Error("Could not open Ogg file "+path);
				return;
			}

	    if((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
	    {
        fclose(oggFile);	        
	      orxonox::Error("Could not open Ogg stream. " + errorString(result));
				return;
	    }

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
		if (loaded)
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
	}
	
	
	
	
	void AudioStream::display()
	{
		if (loaded)
		{
	    std::cout
	        << "version         " << vorbisInfo->version         << "\n"
	        << "channels        " << vorbisInfo->channels        << "\n"
	        << "rate (hz)       " << vorbisInfo->rate            << "\n"
	        << "bitrate upper   " << vorbisInfo->bitrate_upper   << "\n"
	        << "bitrate nominal " << vorbisInfo->bitrate_nominal << "\n"
	        << "bitrate lower   " << vorbisInfo->bitrate_lower   << "\n"
	        << "bitrate window  " << vorbisInfo->bitrate_window  << "\n"
	        << "\n"
	        << "vendor " << vorbisComment->vendor << "\n";
	        
	    for(int i = 0; i < vorbisComment->comments; i++)
	        std::cout << "   " << vorbisComment->user_comments[i] << "\n";
	        
	    std::cout << std::endl;	
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
	
	
	
	std::string AudioStream::errorString(int code)
	{
	    switch(code)
	    {
	        case OV_EREAD:
	            return std::string("Read from media.");
	        case OV_ENOTVORBIS:
	            return std::string("Not Vorbis data.");
	        case OV_EVERSION:
	            return std::string("Vorbis version mismatch.");
	        case OV_EBADHEADER:
	            return std::string("Invalid Vorbis header.");
	        case OV_EFAULT:
	            return std::string("Internal logic fault (bug or heap/stack corruption.");
	        default:
	            return std::string("Unknown Ogg error.");
	    }
	}
}

