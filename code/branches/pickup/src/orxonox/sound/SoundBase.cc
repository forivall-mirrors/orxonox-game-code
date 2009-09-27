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
 *       Erwin 'vaiursch' Herrsche
 *   Co-authors:
 *      ...
 *
 */

#include "SoundBase.h"

#include <string>
#include <vector>
#include <AL/alut.h>
#include <vorbis/vorbisfile.h>

#include "util/Math.h"
#include "core/Core.h"
#include "core/Resource.h"
#include "worldentities/WorldEntity.h"
#include "SoundManager.h"

namespace orxonox
{
    SoundBase::SoundBase(WorldEntity* entity)
    {
        this->source_ = 0;
        this->buffer_ = 0;
        this->entity_ = entity;

        SoundManager::getInstance().addSound(this);
    }

    SoundBase::~SoundBase()
    {
        alSourcei(this->source_, AL_BUFFER, 0);
        alDeleteSources(1, &this->source_);
        alDeleteBuffers(1, &this->buffer_);
    }

    void SoundBase::attachToEntity(WorldEntity* entity)
    {
        this->entity_ = entity;
        this->update();
    }

    void SoundBase::update() {
        if(this->entity_ != NULL && alIsSource(this->source_)) {
            const Vector3& pos = this->entity_->getPosition();
            alSource3f(this->source_, AL_POSITION, pos.x, pos.y, pos.z);
            ALenum error = alGetError();
            if(error == AL_INVALID_VALUE)
                COUT(2) << "Sound: OpenAL: Invalid sound position" << std::endl;

            const Vector3& vel = this->entity_->getVelocity();
            alSource3f(this->source_, AL_VELOCITY, vel.x, vel.y, vel.z);
            error = alGetError();
            if(error == AL_INVALID_VALUE)
                COUT(2) << "Sound: OpenAL: Invalid sound velocity" << std::endl;

            const Quaternion& orient = this->entity_->getOrientation();
            Vector3 at = orient.zAxis();
            alSource3f(this->source_, AL_DIRECTION, at.x, at.y, at.z);
            error = alGetError();
            if(error == AL_INVALID_VALUE)
                COUT(2) << "Sound: OpenAL: Invalid sound direction" << std::endl;
        }
    }

    void SoundBase::play(bool loop) {
        if(alIsSource(this->source_)) {
            if(loop)
                alSourcei(this->source_, AL_LOOPING, AL_TRUE);
            else
                alSourcei(this->source_, AL_LOOPING, AL_FALSE);
            alSourcePlay(this->source_);

            if(alGetError() != AL_NO_ERROR)
            {
                 COUT(2) << "Sound: OpenAL: Error playing sound " << this->source_ << std::endl;
            }
        }
    }

    void SoundBase::stop() {
        if(alIsSource(this->source_)) {
            alSourceStop(this->source_);
        }
    }

    void SoundBase::pause() {
        if(alIsSource(this->source_)) {
            alSourcePause(this->source_);
        }
    }

    bool SoundBase::isPlaying() {
        if(alIsSource(this->source_)) {
            return getSourceState() == AL_PLAYING;
        }
        return false;
    }

    bool SoundBase::isPaused() {
        if(alIsSource(this->source_)) {
            return getSourceState() == AL_PAUSED;
        }
        return true;
    }

    bool SoundBase::isStopped() {
        if(alIsSource(this->source_)) {
            return getSourceState() == AL_INITIAL || getSourceState() == AL_STOPPED;
        }
        return true;
    }

    bool SoundBase::loadFile(const std::string& filename) {
        if(!SoundManager::getInstance().isSoundAvailable())
        {
            COUT(3) << "Sound: not available, skipping " << filename << std::endl;
            return false;
        }

        COUT(3) << "Sound: OpenAL ALUT: loading file " << filename << std::endl;
        // Get DataStream from the resources
        shared_ptr<ResourceInfo> fileInfo = Resource::getInfo(filename);
        if (fileInfo == NULL) {
            COUT(2) << "Warning: Sound file '" << filename << "' not found" << std::endl;
            return false;
        }
        DataStreamPtr stream = Resource::open(filename);
        // Read everything into a temporary buffer
        char* buffer = new char[fileInfo->size];
        stream->read(buffer, fileInfo->size);

        this->buffer_ = alutCreateBufferFromFileImage(buffer, fileInfo->size);
        delete[] buffer;

        if(this->buffer_ == AL_NONE) {
            COUT(2) << "Sound: OpenAL ALUT: " << alutGetErrorString(alutGetError()) << std::endl;
            if(filename.find("ogg", 0) != std::string::npos)
            {
                COUT(2) << "Sound: Trying fallback ogg loader" << std::endl;
                this->buffer_ = loadOggFile(filename);
            }

            if(this->buffer_ == AL_NONE)
            {
                COUT(2) << "Sound: fallback ogg loader failed: " << alutGetErrorString(alutGetError()) << std::endl;
                return false;
            }
        }

        alGenSources(1, &this->source_);
        alSourcei(this->source_, AL_BUFFER, this->buffer_);
        if(alGetError() != AL_NO_ERROR) {
            COUT(2) << "Sound: OpenAL: Error loading sample file: " << filename << std::endl;
            return false;
        }
        return true;
    }

    ALint SoundBase::getSourceState() {
        ALint state;
        alGetSourcei(this->source_, AL_SOURCE_STATE, &state);
        return state;
    }

    ALuint SoundBase::loadOggFile(const std::string& filename)
    {
        char inbuffer[4096];
        std::vector<char> outbuffer;
        OggVorbis_File vf;
        vorbis_info* vorbisInfo;
        int eof = false;
        int current_section;
        ALuint buffer;
        ALenum format;

        FILE* f = fopen(filename.c_str(), "rb");

        if(ov_open(f, &vf, NULL, 0) < 0)
        {
            COUT(2) << "Sound: libvorbisfile: File does not seem to be an Ogg Vorbis bitstream" << std::endl;
            ov_clear(&vf);
            return AL_NONE;
        }

        while(!eof)
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
        if(vorbisInfo->channels == 1)
            format = AL_FORMAT_MONO16;
        else
            format = AL_FORMAT_STEREO16;

        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, &outbuffer[0], outbuffer.size(), vorbisInfo->rate);
        ov_clear(&vf);

        return buffer;
    }
} // namespace: orxonox
