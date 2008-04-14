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

#ifndef _AudioStream_H__
#define _AudioStream_H__

#include "AudioPrereqs.h"

#include <string>
#include <iostream>

#include <AL/al.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>

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
