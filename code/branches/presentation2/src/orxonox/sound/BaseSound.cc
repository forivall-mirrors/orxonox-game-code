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

#include <cassert>
#include <vector>
#include <al.h>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/Resource.h"
#include "core/XMLPort.h"
#include "SoundBuffer.h"
#include "SoundManager.h"

namespace orxonox
{
    BaseSound::BaseSound()
        : audioSource_(0)
        , bPooling_(false)
        , volume_(1.0)
        , bLoop_(false)
        , state_(Stopped)
        , pitch_ (1.0)
    {
        RegisterRootObject(BaseSound);

        if (GameMode::playsSound())
        {
            alGenSources(1, &this->audioSource_);
            if (!alIsSource(this->audioSource_))
                COUT(1) << "Sound: Source generation failed: " << SoundManager::getALErrorString(alGetError()) << std::endl;

            if (alIsSource(this->audioSource_))
            {
                alSourcei(this->audioSource_, AL_REFERENCE_DISTANCE, 20);
                alSourcei(this->audioSource_, AL_MAX_DISTANCE, 300);
            }
        }
    }

    BaseSound::~BaseSound()
    {
        this->setSource(std::string());
        if (GameMode::playsSound() && alIsSource(this->audioSource_))
            alDeleteSources(1, &this->audioSource_);
    }

    void BaseSound::XMLPortExtern(Element& xmlelement, XMLPort::Mode mode)
    {
        XMLPortParam(BaseSound, "volume", setVolume,  getVolume,  xmlelement, mode);
        XMLPortParam(BaseSound, "loop",   setLooping, getLooping, xmlelement, mode);
        XMLPortParam(BaseSound, "play",   setPlaying, isPlaying,  xmlelement, mode);
        XMLPortParam(BaseSound, "source", setSource,  getSource,  xmlelement, mode);
    }

    void BaseSound::play()
    {
        this->state_ = Playing;
        if (GameMode::playsSound() && alIsSource(this->audioSource_) && this->getSourceState() != AL_PLAYING)
        {
            alSourcePlay(this->audioSource_);

            if (int error = alGetError())
                COUT(2) << "Sound: Error playing sound: " << error << std::endl;
        }
    }

    void BaseSound::stop()
    {
        this->state_ = Stopped;
        if (GameMode::playsSound() && alIsSource(this->audioSource_))
            alSourceStop(this->audioSource_);
    }

    void BaseSound::pause()
    {
        if (this->isStopped())
            return;
        this->state_ = Paused;
        if (GameMode::playsSound() && alIsSource(this->audioSource_))
            alSourcePause(this->audioSource_);
    }

    ALint BaseSound::getSourceState() const
    {
        if (GameMode::playsSound() && alIsSource(this->audioSource_))
        {
            ALint state;
            alGetSourcei(this->audioSource_, AL_SOURCE_STATE, &state);
            return state;
        }
        else
            return AL_INITIAL;
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
        
        this->updateVolume();
    }
    
    float BaseSound::getVolumeGain()
    {
        return SoundManager::getInstance().getVolume(SoundType::none);
    }
    
    void BaseSound::updateVolume(void)
    {
        if (alIsSource(this->audioSource_))
        {
            alSourcef(this->audioSource_, AL_GAIN, this->volume_*this->getVolumeGain());
            if (int error = alGetError())
                COUT(2) << "Sound: Error setting volume: " << error << std::endl;
        }
    }

    void BaseSound::setLooping(bool val)
    {
        this->bLoop_ = val;
        if (GameMode::playsSound() && alIsSource(this->audioSource_))
            alSourcei(this->audioSource_, AL_LOOPING, (val ? AL_TRUE : AL_FALSE));
    }

    void BaseSound::setPitch(float pitch)
    {
        if (pitch > 2 || pitch < 0.5)
        {
            COUT(2) << "Sound warning: pitch out of range, cropping value." << std::endl;
            pitch = pitch > 2 ? 2 : pitch;
            pitch = pitch < 0.5 ? 0.5 : pitch;
        }        
        this->pitch_ = pitch;
        if (GameMode::playsSound() && alIsSource(this->audioSource_))
        {
            if (int error = alGetError())
                COUT(2) << "Sound: Error setting pitch: " << error << std::endl;
            alSourcef(this->audioSource_, AL_PITCH, pitch);
        }
    }

    void BaseSound::setSource(const std::string& source)
    {
        if (!GameMode::playsSound() || source == this->source_) 
        {
            this->source_ = source;
            return;
        }

        if (this->soundBuffer_ != NULL)
        {
            if (alIsSource(this->audioSource_))
            {
                alSourceStop(this->audioSource_);
                // Unload old buffer first
                alSourcei(this->audioSource_, AL_BUFFER, 0);
            }
            SoundManager::getInstance().releaseSoundBuffer(this->soundBuffer_, this->bPooling_);
            this->soundBuffer_.reset();
        }

        this->source_ = source;
        if (source_.empty() || !alIsSource(this->audioSource_))
            return;

        this->soundBuffer_ = SoundManager::getInstance().getSoundBuffer(this->source_);
        if (this->soundBuffer_ == NULL)
            return;

        alSourcei(this->audioSource_, AL_BUFFER, this->soundBuffer_->getBuffer());
        if (ALuint error = alGetError())
        {
            COUT(1) << "Sound Error: Could not load file \"" << source << "\": " << SoundManager::getALErrorString << std::endl;
            return;
        }

        alSource3f(this->audioSource_, AL_POSITION,  0, 0, 0);
        this->updateVolume();
        this->setPitch(this->getPitch());
        this->setLooping(getLooping());
        if (this->isPlaying() || this->isPaused())
        {
            alSourcePlay(this->audioSource_);
            if (int error = alGetError())
                COUT(2) << "Sound: Error playing sound: " << error << std::endl;
        }
        if (this->isPaused())
            alSourcePause(this->audioSource_);
    }
}
