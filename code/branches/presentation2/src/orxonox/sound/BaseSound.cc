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
        , volume_(1.0)
        , bLoop_(false)
        , state_(Stopped)
        , pitch_ (1.0)
    {
        RegisterRootObject(BaseSound);

        if (GameMode::playsSound())
        {
            alGenSources(1, &this->audioSource_);
            assert(this->audioSource_ != 0);
        }
    }

    BaseSound::~BaseSound()
    {
        this->setSource("");
        if (GameMode::playsSound())
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
        if (!this->isPlaying() && GameMode::showsGraphics())
        {
            this->state_ = Playing;
            alSourcePlay(this->audioSource_);

            if (int error = alGetError())
                COUT(2) << "Sound: Error playing sound: " << error << std::endl;
        }
    }

    void BaseSound::stop()
    {
        this->state_ = Stopped;
        if (GameMode::playsSound())
            alSourceStop(this->audioSource_);
    }

    void BaseSound::pause()
    {
        if (this->isStopped())
            return;
        this->state_ = Paused;
        if (GameMode::playsSound())
            alSourcePause(this->audioSource_);
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
        if (GameMode::playsSound())
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
        if (GameMode::playsSound())
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
            alSourceStop(this->audioSource_);
            // Unload old sound first
            alSourcei(this->audioSource_, AL_BUFFER, 0);
            this->soundBuffer_.reset();
        }

        this->source_ = source;
        if (source_.empty()) 
            return;

        // Get DataStream from the resources
        shared_ptr<ResourceInfo> fileInfo = Resource::getInfo(source);
        if (fileInfo == NULL)
        {
            COUT(2) << "Sound: Warning: Sound file '" << source << "' not found" << std::endl;
            return;
        }

        this->soundBuffer_ = SoundManager::getInstance().getSoundBuffer(fileInfo);
        if (this->soundBuffer_ == NULL)
            return;

        alSourcei(this->audioSource_, AL_BUFFER, this->soundBuffer_->getBuffer());
        if (alGetError() != AL_NO_ERROR)
        {
            COUT(2) << "Sound: OpenAL: Error loading sample file: " << source << std::endl;
            return;
        }

        alSource3f(this->audioSource_, AL_POSITION,  0, 0, 0);
        this->updateVolume();
        this->setPitch(this->getPitch());
        this->setLooping(getLooping());
        if (this->isPlaying() || this->isPaused())
            BaseSound::play();
        if (this->isPaused())
            BaseSound::pause();
    }
}
