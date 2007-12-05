#ifndef __ogg_h__
#define __ogg_h__

#include "AudioIncludes.h"

namespace audio
{
	#define BUFFER_SIZE (4096 * 4)
	
	class AudioStream
	{
	    public:
					AudioStream(std::string path);
	        void open();
	        void release();
	        void display();
	        bool playback();
	        bool playing();
	        bool update();
					inline bool isLoaded() { return loaded; }	

	    protected:
	
	        bool stream(ALuint buffer);
	        void empty();
	        void check();
	        std::string errorString(int code);
	
	    private:
	
					std::string path;

	        FILE*           oggFile;
	        OggVorbis_File  oggStream;
	        vorbis_info*    vorbisInfo;
	        vorbis_comment* vorbisComment;
					bool loaded;	

	        ALuint buffers[2];
	        ALuint source;
	        ALenum format;
	};
}

#endif // __ogg_h__
