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
        : bPooling_(false)
        , volume_(1.0)
        , bLooping_(false)
        , state_(Stopped)
        , pitch_ (1.0)
    {
        RegisterRootObject(BaseSound);

        // Initialise audioSource_ to a value that is not a source
        // 0 is unfortunately not guaranteed to be no source ID.
        this->audioSource_ = 123456789;
        while (alIsSource(++this->audioSource_));
    }

    BaseSound::~BaseSound()
    {
        this->stop();
        // Release buffer
        if (this->soundBuffer_ != NULL)
        {
            assert(GameMode::playsSound());
            SoundManager::getInstance().releaseSoundBuffer(this->soundBuffer_, this->bPooling_);
        }
    }

    void BaseSound::XMLPortExtern(Element& xmlelement, XMLPort::Mode mode)
    {
        XMLPortParam(BaseSound, "volume",  setVolume,  getVolume,  xmlelement, mode);
        XMLPortParam(BaseSound, "looping", setLooping, getLooping, xmlelement, mode);
        XMLPortParam(BaseSound, "pitch",   setPitch,   getPitch,   xmlelement, mode);
        XMLPortParam(BaseSound, "source",  setSource,  getSource,  xmlelement, mode);
    }

    void BaseSound::doPlay()
    {
        this->state_ = Playing;
        if (GameMode::playsSound() && this->getSourceState() != AL_PLAYING && this->soundBuffer_ != NULL)
        {
            if (!alIsSource(this->audioSource_))
            {
                this->audioSource_ = SoundManager::getInstance().getSoundSource(this);
                if (!alIsSource(this->audioSource_))
                    return;
                this->initialiseSource();
            }

            alSourcePlay(this->audioSource_);
            if (int error = alGetError())
                COUT(2) << "Sound: Error playing sound: " << getALErrorString(error) << std::endl;
        }
    }

    void BaseSound::doStop()
    {
        this->state_ = Stopped;
        if (alIsSource(this->audioSource_))
        {
            alSourceStop(this->audioSource_);
            // Release buffer
            alSourcei(this->audioSource_, AL_BUFFER, AL_NONE);
            // Release source again
            SoundManager::getInstance().releaseSoundSource(this->audioSource_);
            // Get a no source ID
            this->audioSource_ += 123455;
            while (alIsSource(++this->audioSource_));
        }
    }

    void BaseSound::doPause()
    {
        if (this->isStopped())
            return;
        this->state_ = Paused;
        if (alIsSource(this->audioSource_))
            alSourcePause(this->audioSource_);
    }

    ALint BaseSound::getSourceState() const
    {
        if (alIsSource(this->audioSource_))
        {
            ALint state;
            alGetSourcei(this->audioSource_, AL_SOURCE_STATE, &state);
            return state;
        }
        else
            return AL_INITIAL;
    }

    void BaseSound::initialiseSource()
    {
        this->updateVolume();
        this->setPitch(this->getPitch());
        this->setLooping(this->getLooping());
        alSource3f(this->audioSource_, AL_POSITION,  0, 0, 0);
        alSource3f(this->audioSource_, AL_VELOCITY,  0, 0, 0);
        alSource3f(this->audioSource_, AL_DIRECTION, 0, 0, 0);
        if (ALint error = alGetError())
            COUT(2) << "Sound: Warning: Setting source parameters to 0 failed: " << getALErrorString(error) << std::endl;
        if(this->soundBuffer_ != NULL) {
            alSourcei(this->audioSource_, AL_BUFFER, this->soundBuffer_->getBuffer());
        }
        if (ALuint error = alGetError())
            COUT(1) << "Sound: Error: Could not set buffer \"" << this->source_ << "\": " << getALErrorString(error) << std::endl;
    }

    void BaseSound::setVolume(float vol)
    {
        this->volume_ = clamp(vol, 0.0f, 1.0f);
        if (this->volume_ != vol)
            COUT(2) << "Sound: Warning: volume out of range, clamping value." << std::endl;
        this->updateVolume();
    }

    void BaseSound::updateVolume()
    {
        if (alIsSource(this->audioSource_))
        {
            float volume = this->volume_ * this->getRealVolume();
            alSourcef(this->audioSource_, AL_GAIN, volume);
            if (int error = alGetError())
                COUT(2) << "Sound: Error setting volume to " << volume << ": " << getALErrorString(error) << std::endl;
        }
    }

    void BaseSound::setLooping(bool val)
    {
        this->bLooping_ = val;
        if (alIsSource(this->audioSource_))
            alSourcei(this->audioSource_, AL_LOOPING, (val ? AL_TRUE : AL_FALSE));
    }

    void BaseSound::setPitch(float pitch)
    {
        if (pitch > 2 || pitch < 0.5f)
        {
            COUT(2) << "Sound: Warning: pitch out of range, cropping value." << std::endl;
            pitch = pitch > 2.0f ? 2.0f : pitch;
            pitch = pitch < 0.5f ? 0.5f : pitch;
        }
        this->pitch_ = pitch;
        if (alIsSource(this->audioSource_))
        {
            alSourcef(this->audioSource_, AL_PITCH, pitch);
            if (int error = alGetError())
                COUT(2) << "Sound: Error setting pitch: " << getALErrorString(error) << std::endl;
        }
    }

    void BaseSound::setSource(const std::string& source)
    {
        if (!GameMode::playsSound())
        {
            this->source_ = source;
            return;
        }

        if (this->soundBuffer_ != NULL)
        {
            if (this->soundBuffer_->getFilename() == source)
            {
                assert(this->source_ == source_);
                return;
            }
            // Stopping is imperative here!
            if (alIsSource(this->audioSource_))
            {
                alSourceStop(this->audioSource_);
                alSourcei(this->audioSource_, AL_BUFFER, AL_NONE);
            }
            SoundManager::getInstance().releaseSoundBuffer(this->soundBuffer_, this->bPooling_);
            this->soundBuffer_.reset();
        }

        this->source_ = source;
        // Don't load ""
        if (source_.empty())
            return;

        // Get new sound buffer
        this->soundBuffer_ = SoundManager::getInstance().getSoundBuffer(this->source_);
        if (this->soundBuffer_ == NULL)
            return;

        if (alIsSource(this->audioSource_)) // already playing or paused
        {
            // Set new buffer
            alSourcei(this->audioSource_, AL_BUFFER, this->soundBuffer_->getBuffer());
            if (ALuint error = alGetError())
            {
                COUT(1) << "Sound: Error: Could not set buffer \"" << source << "\": " << getALErrorString(error) << std::endl;
                return;
            }

            // Sound was already playing or paused because there was a source acquired
            assert(this->isPlaying() || this->isPaused());
            alSourcePlay(this->audioSource_);
            if (int error = alGetError())
                COUT(2) << "Sound: Error playing sound: " << getALErrorString(error) << std::endl;
            if (this->isPaused())
                alSourcePause(this->audioSource_);
        }
        else // No source acquired so far, but might be set to playing or paused
        {
            State state = this->state_; // save
            if (this->isPlaying() || this->isPaused())
                doPlay();
            if (state == Paused)
            {
                this->state_ = Paused;
                doPause();
            }
        }
    }

    void BaseSound::stateChanged()
    {
        switch (this->state_)
        {
            case Playing:
                this->play();
                break;
            case Paused:
                this->pause();
                break;
            case Stopped:
            default:
                this->stop();
                break;
        }
    }
}
