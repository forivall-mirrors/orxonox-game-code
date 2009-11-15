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
 *      Erwin 'vaiursch' Herrsche
 *   Co-authors:
 *      Reto Grieder
 *
 */

#include "BaseSound.h"

#include <vector>
#include <AL/alut.h>
#include <vorbis/vorbisfile.h>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/Resource.h"

namespace orxonox
{
    BaseSound::BaseSound()
        : audioSource_(0)
        , audioBuffer_(0)
        , bPlayOnLoad_(false)
        , bLoop_(false)
    {
        RegisterRootObject(BaseSound);

        if (GameMode::playsSound())
            alGenSources(1, &this->audioSource_);
    }

    BaseSound::~BaseSound()
    {
        this->setSource("");
        if (this->audioSource_)
            alDeleteSources(1, &this->audioSource_);
    }

    void BaseSound::play()
    {
        if (alIsSource(this->audioSource_))
        {
            if (this->bLoop_)
                alSourcei(this->audioSource_, AL_LOOPING, AL_TRUE);
            else
                alSourcei(this->audioSource_, AL_LOOPING, AL_FALSE);
            alSourcePlay(this->audioSource_);

            if (alGetError() != AL_NO_ERROR)
            {
                 COUT(2) << "Sound: OpenAL: Error playing sound " << this->audioSource_ << std::endl;
            }
        }
    }

    void BaseSound::stop()
    {
        if (alIsSource(this->audioSource_))
            alSourceStop(this->audioSource_);
    }

    void BaseSound::pause()
    {
        if (alIsSource(this->audioSource_))
            alSourcePause(this->audioSource_);
    }

    bool BaseSound::isPlaying()
    {
        if (alIsSource(this->audioSource_))
            return getSourceState() == AL_PLAYING;
        return false;
    }

    bool BaseSound::isPaused()
    {
        if (alIsSource(this->audioSource_))
            return getSourceState() == AL_PAUSED;
        return false;
    }

    bool BaseSound::isStopped()
    {
        if (alIsSource(this->audioSource_))
            return getSourceState() == AL_INITIAL || getSourceState() == AL_STOPPED;
        return true;
    }

    void BaseSound::setVolume(float vol)
    {
        if (vol > 1 || vol < 0)
        {
            COUT(2) << "Sound warning: volume out of range, cropping value." << std::endl;
            vol = vol > 1 ? 1 : vol;
            vol = vol < 0 ? 0 : vol;
        }
        this->volume_ = vol;
        if (alIsSource(this->audioSource_))
            alSourcef(this->audioSource_, AL_GAIN, vol);
    }

    void BaseSound::setSource(const std::string& source)
    {
        if (!GameMode::playsSound() || source == this->source_) 
        {
            this->source_ = source;
            return;
        }
        
        if (alIsSource(this->audioBuffer_))
        {
            this->stop();
            // Unload old sound first
            alSourcei(this->audioSource_, AL_BUFFER, 0);
            alDeleteBuffers(1, &this->audioBuffer_);
            this->audioBuffer_ = 0;
        }

        this->source_ = source;
        if (source_.empty()) 
            return;

        COUT(3) << "Sound: OpenAL ALUT: loading file " << source << std::endl;
        // Get DataStream from the resources
        shared_ptr<ResourceInfo> fileInfo = Resource::getInfo(source);
        if (fileInfo == NULL)
        {
            COUT(2) << "Sound: Warning: Sound file '" << source << "' not found" << std::endl;
            return;
        }
        dataStream_ = Resource::open(source);
        // Read everything into a temporary buffer
        char* buffer = new char[fileInfo->size];
        dataStream_->read(buffer, fileInfo->size);
        dataStream_->seek(0);

        this->audioBuffer_ = alutCreateBufferFromFileImage(buffer, fileInfo->size);
        delete[] buffer;

        if (this->audioBuffer_ == AL_NONE)
        {
            COUT(2) << "Sound: OpenAL ALUT: " << alutGetErrorString(alutGetError()) << std::endl;
            if (source.find("ogg", 0) != std::string::npos)
            {
                COUT(2) << "Sound: Trying fallback ogg loader" << std::endl;
                this->audioBuffer_ = this->loadOggFile();
            }

            if (this->audioBuffer_ == AL_NONE)
            {
                COUT(2) << "Sound: fallback ogg loader failed: " << alutGetErrorString(alutGetError()) << std::endl;
                return;
            }
        }

        alSourcei(this->audioSource_, AL_BUFFER, this->audioBuffer_);
        if (alGetError() != AL_NO_ERROR)
        {
            COUT(2) << "Sound: OpenAL: Error loading sample file: " << source << std::endl;
            return;
        }

        alSource3f(this->audioSource_, AL_POSITION,  0, 0, 0);

        this->setVolume(this->volume_);

        if (this->bPlayOnLoad_)
            this->play();
    }

    ALint BaseSound::getSourceState()
    {
        ALint state;
        alGetSourcei(this->audioSource_, AL_SOURCE_STATE, &state);
        return state;
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

    ALuint BaseSound::loadOggFile()
    {
        char inbuffer[4096];
        std::vector<char> outbuffer;
        OggVorbis_File vf;
        vorbis_info* vorbisInfo;
        int eof = false;
        int current_section;
        ALuint buffer;
        ALenum format;

        // Open file with custom streaming
        ov_callbacks vorbisCallbacks;
        vorbisCallbacks.read_func  = &readVorbis;
        vorbisCallbacks.seek_func  = &seekVorbis;
        vorbisCallbacks.tell_func  = &tellVorbis;
        vorbisCallbacks.close_func = NULL;

        int ret = ov_open_callbacks(dataStream_.get(), &vf, NULL, 0, vorbisCallbacks);
        if (ret < 0)
        {
            COUT(2) << "Sound: libvorbisfile: File does not seem to be an Ogg Vorbis bitstream" << std::endl;
            ov_clear(&vf);
            return AL_NONE;
        }

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
                return AL_NONE;
            }
            else
            {
                outbuffer.insert(outbuffer.end(), inbuffer, inbuffer + sizeof(inbuffer));
            }
        }

        vorbisInfo = ov_info(&vf, -1);
        if (vorbisInfo->channels == 1)
            format = AL_FORMAT_MONO16;
        else
            format = AL_FORMAT_STEREO16;

        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, &outbuffer[0], outbuffer.size(), vorbisInfo->rate);
        ov_clear(&vf);

        return buffer;
    }
}
