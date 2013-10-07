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

#include <alut.h>
#include <vorbis/vorbisfile.h>

#include "util/Exception.h"
#include "util/StringUtils.h"
#include "sound/SoundManager.h"

namespace orxonox
{
    SoundBuffer::SoundBuffer(const std::string& filename, std::list<shared_ptr<SoundBuffer> >::iterator poolIterator)
        : filename_(filename)
        , audioBuffer_(AL_NONE)
        , poolIterator_(poolIterator)
    {
        if (this->filename_.empty())
            ThrowException(General, "SoundBuffer construction: fileInfo was NULL");

        // Get resource info
        shared_ptr<ResourceInfo> fileInfo = Resource::getInfo(filename);
        if (fileInfo == NULL)
        {
            orxout(internal_error, context::sound) << "Sound file '" << filename << "' not found" << endl;
            return;
        }
        // Open data stream
        DataStreamPtr dataStream = Resource::open(fileInfo);

        const std::string& extension = this->filename_.substr(this->filename_.find_last_of('.') + 1);
        if (getLowercase(extension) == "ogg")
        {
            // Try ogg loader
            this->loadOgg(fileInfo, dataStream);
        }
        else
        {
            // Try standard OpenAL loader
            this->loadStandard(fileInfo, dataStream);
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
        return alGetError() ? 0 : size;
    }

    void SoundBuffer::loadStandard(const shared_ptr<ResourceInfo>& fileInfo, DataStreamPtr dataStream)
    {
        // Read everything into a temporary buffer
        char* buffer = new char[fileInfo->size];
        dataStream->read(buffer, fileInfo->size);
        dataStream->seek(0);

        this->audioBuffer_ = alutCreateBufferFromFileImage(buffer, fileInfo->size);
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
        switch (whence)
        {
        case SEEK_SET:
            stream->seek((size_t)offset);
            break;
        case SEEK_CUR:
            stream->skip((size_t)offset);
            break;
        case SEEK_END:
            stream->seek(stream->size() + (size_t)offset);
            break;
        default:
            return -1;
        }
        return 0;
    }

    long tellVorbis(void* datasource)
    {
        return static_cast<long>(static_cast<Ogre::DataStream*>(datasource)->tell());
    }

    void SoundBuffer::loadOgg(const shared_ptr<ResourceInfo>& fileInfo, DataStreamPtr dataStream)
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
            orxout(internal_error, context::sound) << "libvorbisfile: File does not seem to be an Ogg Vorbis bitstream" << endl;
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
                orxout(internal_error, context::sound) << "libvorbisfile: error reading the file" << endl;
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
