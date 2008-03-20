#ifndef _AudioStream_H__
#define _AudioStream_H__

#include "AudioIncludes.h"

#include "AudioPrereqs.h"

namespace audio
{
  #define BUFFER_SIZE (4096 * 4)

  class _AudioExport AudioStream
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

#endif /* _AudioStream_H__ */
