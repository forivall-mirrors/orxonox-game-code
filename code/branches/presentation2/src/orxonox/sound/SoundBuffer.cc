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
 *      Reto Grieder
 *      Erwin 'vaiursch' Herrsche
 *   Co-authors:
 *      ...
 *
 */

#include "SoundBuffer.h"

#include <AL/alut.h>
#include <vorbis/vorbisfile.h>

#include "util/Exception.h"
#include "util/StringUtils.h"
#include "core/Resource.h"
#include "sound/SoundManager.h"

namespace orxonox
{
    SoundBuffer::SoundBuffer(shared_ptr<ResourceInfo> fileInfo)
        : fileInfo_(fileInfo)
        , audioBuffer_(AL_NONE)
    {
        if (this->fileInfo_ == NULL)
            ThrowException(General, "SoundBuffer construction: fileInfo was NULL");
        DataStreamPtr dataStream = Resource::open(this->fileInfo_);

        std::string extension(this->fileInfo_->basename.substr(this->fileInfo_->basename.find_last_of('.') + 1));
        if (getLowercase(extension) == "ogg")
        {
            // Try ogg loader
            this->loadOgg(dataStream);
        }
        else
        {
            // Try standard OpenAL loader
            this->loadStandard(dataStream);
        }
    }

    SoundBuffer::~SoundBuffer()
    {
        // Destroy buffer
        alDeleteBuffers(1, &this->audioBuffer_);
    }

    unsigned int SoundBuffer::getSize() const
    {
        ALint size;
        alGetBufferi(this->audioBuffer_, AL_SIZE, &size);
        if (!alGetError())
            return size;
        else
            return 0;
    }

    void SoundBuffer::loadStandard(DataStreamPtr dataStream)
    {
        // Read everything into a temporary buffer
        char* buffer = new char[this->fileInfo_->size];
        dataStream->read(buffer, this->fileInfo_->size);
        dataStream->seek(0);

        this->audioBuffer_ = alutCreateBufferFromFileImage(buffer, this->fileInfo_->size);
        delete[] buffer;

        if (!alIsBuffer(this->audioBuffer_))
            ThrowException(General, "Sound Error: Standard file loader failed: " << alutGetErrorString(alutGetError()));
    }

    size_t readVorbis(void* ptr, size_t size, size_t nmemb, void* datasource)
    {
        return static_cast<Ogre::DataStream*>(datasource)->read(ptr, size * nmemb);
    }

    int seekVorbis(void* datasource, ogg_int64_t offset, int whence)
    {
        Ogre::DataStream* stream = static_cast<Ogre::DataStream*>(datasource);
        int offset_beg = offset;
        if (whence == SEEK_CUR)
            offset_beg = stream->tell() + offset;
        else if (whence == SEEK_END)
            offset_beg = stream->size() + offset;
        else if (whence != SEEK_SET)
            return -1;
        stream->seek(offset_beg);
        return 0;
    }

    long tellVorbis(void* datasource)
    {
        return static_cast<long>(static_cast<Ogre::DataStream*>(datasource)->tell());
    }

    void SoundBuffer::loadOgg(DataStreamPtr dataStream)
    {
        char inbuffer[256*1024];
        std::vector<char> outbuffer;
        outbuffer.reserve(80*1024*1024);

        // Open file with custom streaming
        ov_callbacks vorbisCallbacks;
        vorbisCallbacks.read_func  = &readVorbis;
        vorbisCallbacks.seek_func  = &seekVorbis;
        vorbisCallbacks.tell_func  = &tellVorbis;
        vorbisCallbacks.close_func = NULL;

        OggVorbis_File vf;
        int ret = ov_open_callbacks(dataStream.get(), &vf, NULL, 0, vorbisCallbacks);
        if (ret < 0)
        {
            COUT(2) << "Sound: libvorbisfile: File does not seem to be an Ogg Vorbis bitstream" << std::endl;
            ov_clear(&vf);
            ThrowException(General, "Sound Error: Ogg file loader failed when opening the bitstream");
        }

        int current_section;
        int eof = false;
        while (!eof)
        {
            long ret = ov_read(&vf, inbuffer, sizeof(inbuffer), 0, 2, 1, &current_section);
            if (ret == 0)
            {
                eof = true;
            }
            else if (ret < 0)
            {
                COUT(2) << "Sound: libvorbisfile: error reading the file" << std::endl;
                ov_clear(&vf);
                ThrowException(General, "Sound Error: Ogg file loader failed when decoding the file");
            }
            else
            {
                outbuffer.insert(outbuffer.end(), inbuffer, inbuffer + ret);
            }
        }

        vorbis_info* vorbisInfo;
        vorbisInfo = ov_info(&vf, -1);
        ALenum format;
        if (vorbisInfo->channels == 1)
            format = AL_FORMAT_MONO16;
        else
            format = AL_FORMAT_STEREO16;

        alGenBuffers(1, &this->audioBuffer_);
        alBufferData(this->audioBuffer_, format, &outbuffer[0], outbuffer.size(), vorbisInfo->rate);
        ov_clear(&vf);

        if (!alIsBuffer(this->audioBuffer_))
            ThrowException(General, "Sound: Ogg file loader failed when creating the buffer.");
    }
}
