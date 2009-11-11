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

#include "SoundManager.h"

#include <AL/alut.h>

#include "util/Exception.h"
#include "util/Math.h"
#include "util/ScopeGuard.h"
#include "util/StringUtils.h"
#include "util/Clock.h"
#include "core/GameMode.h"
#include "core/ScopedSingletonManager.h"
#include "core/Resource.h"
#include "core/ConfigValueIncludes.h"
#include "BaseSound.h"
#include "MoodManager.h"
#include "AmbientSound.h"

namespace orxonox
{
    SoundManager* SoundManager::singletonPtr_s = NULL;
    ManageScopedSingleton(SoundManager, ScopeID::Graphics, true);

    SoundManager::SoundManager()
    {
        RegisterRootObject(SoundManager);

        if (!alutInitWithoutContext(NULL,NULL))
            ThrowException(InitialisationFailed, "Sound: OpenAL ALUT error: " << alutGetErrorString(alutGetError()));
        Loki::ScopeGuard alutExitGuard = Loki::MakeGuard(&alutExit);

        COUT(3) << "Sound: OpenAL: Opening sound device..." << std::endl;
        this->device_ = alcOpenDevice(NULL);
        if (this->device_ == NULL)
        {
            COUT(0) << "Sound: OpenaAL: Could not open sound device. Have you installed OpenAL?" << std::endl;
#ifdef ORXONOX_PLATFORM_WINDOWS
            COUT(0) << "Sound: Just getting the DLL with the dependencies is not enough for Windows (esp. Windows 7)!" << std::endl;
#endif
            ThrowException(InitialisationFailed, "Sound: OpenAL error: Could not open sound device.");
        }
        Loki::ScopeGuard closeDeviceGuard = Loki::MakeGuard(&alcCloseDevice, this->device_);

        COUT(3) << "Sound: OpenAL: Sound device opened" << std::endl;
        this->context_ = alcCreateContext(this->device_, NULL);
        if (this->context_ == NULL)
            ThrowException(InitialisationFailed, "Sound: OpenAL error: Could not create sound context");
        Loki::ScopeGuard desroyContextGuard = Loki::MakeGuard(&alcDestroyContext, this->context_);

        if (alcMakeContextCurrent(this->context_) == AL_TRUE)
            COUT(3) << "Sound: OpenAL: Context " << this->context_ << " loaded" << std::endl;

        COUT(4) << "Sound: OpenAL ALUT version: " << alutGetMajorVersion() << "." << alutGetMinorVersion() << std::endl;

        const char* str = alutGetMIMETypes(ALUT_LOADER_BUFFER);
        if (str == NULL)
            COUT(2) << "Sound: OpenAL ALUT error: " << alutGetErrorString(alutGetError()) << std::endl;
        else
            COUT(4) << "Sound: OpenAL ALUT supported MIME types: " << str << std::endl;

        GameMode::setPlaysSound(true);
        // Disarm guards
        alutExitGuard.Dismiss();
        closeDeviceGuard.Dismiss();
        desroyContextGuard.Dismiss();

        this->setConfigValues();
    }

    SoundManager::~SoundManager()
    {
        GameMode::setPlaysSound(false);
        alcDestroyContext(this->context_);
        alcCloseDevice(this->device_);
        alutExit();
    }

    void SoundManager::update(const Clock &time)
    {
        this->fadeInAmbientSound(time.getDeltaTime());
        this->fadeOutAmbientSound(time.getDeltaTime());
    }

    void SoundManager::setConfigValues()
    {
        SetConfigValue(fadeStep_, 0.2f)
            .description("Determines how fast sounds should fade, per second.")
            .callback(this, &SoundManager::checkFadeStepValidity);
    }

    void SoundManager::setListenerPosition(const Vector3& position)
    {
        alListener3f(AL_POSITION, position.x, position.y, position.z);
        ALenum error = alGetError();
        if (error == AL_INVALID_VALUE)
            COUT(2) << "Sound: OpenAL: Invalid listener position" << std::endl;
    }

    void SoundManager::setListenerOrientation(const Quaternion& orientation)
    {
        // update listener orientation
        Vector3 up = orientation.xAxis(); // just a wild guess
        Vector3 at = orientation.zAxis();

        ALfloat orient[6] = { at.x, at.y, at.z,
                              up.x, up.y, up.z };

        alListenerfv(AL_POSITION, orient);
        ALenum error = alGetError();
        if (error == AL_INVALID_VALUE)
            COUT(2) << "Sound: OpenAL: Invalid listener orientation" << std::endl;
    }

    void SoundManager::registerAmbientSound(AmbientSound* newAmbient)
    {
        if(newAmbient != NULL)
        {
            if (!(this->ambientSounds_.empty())) 
            {
                this->fadeOutList_.push_front(std::make_pair(this->ambientSounds_.front(), 1.0));
            }
            this->fadeInList_.push_front(std::make_pair(newAmbient, 0.0));
            this->ambientSounds_.push_front(newAmbient);
        }
    }

    void SoundManager::unregisterAmbientSound(AmbientSound* currentAmbient)
    {
        if(currentAmbient == NULL || ambientSounds_.empty())
        {
            return;
        }
        if(this->ambientSounds_.front() == currentAmbient) 
        {
            this->fadeOutList_.push_front(std::make_pair(this->ambientSounds_.front(), 1.0));
            this->ambientSounds_.pop_front();
            if(!(this->ambientSounds_.empty()))
            {
                this->fadeInList_.push_front(std::make_pair(this->ambientSounds_.front(), 0.0));
            }
        }
        else
        {
            for(std::list<AmbientSound*>::iterator it= this->ambientSounds_.begin(); it != this->ambientSounds_.end(); it++)
            {
                if(*it == currentAmbient)
                {
                    currentAmbient->doStop();
                    this->ambientSounds_.erase(it);
                    break;
                }
            }
        }
    }

    // Get the current mood and return the full path string to the requested sound.
    const std::string& SoundManager::getAmbientPath(const std::string& source)
    {
        lastReqPath_ = "ambient/" + MoodManager::getInstance().getMood() + "/" + source;
        shared_ptr<ResourceInfo> fileInfo = Resource::getInfo(lastReqPath_);
        if(fileInfo == NULL)
        {
            return BLANKSTRING;
        }
        return lastReqPath_;
    }

    void SoundManager::fadeInAmbientSound(float dt)
    {
        if(!(this->fadeInList_.empty()))
        {
            for(std::list<std::pair<AmbientSound*, float> >::iterator it= this->fadeInList_.begin(); it != this->fadeInList_.end(); it++)
            {
                it->second += fadeStep_ * dt;
                alSourcef(it->first->getALAudioSource(), AL_GAIN, it->second);
            }
            if(this->fadeInList_.back().second >= 1)
            {
                this->fadeInList_.pop_back();
            }
        }
    }

    void SoundManager::fadeOutAmbientSound(float dt)
    {
        if(!(this->fadeInList_.empty()))
        {
            for(std::list<std::pair<AmbientSound*, float> >::iterator it= this->fadeOutList_.begin(); it != this->fadeOutList_.end(); it++)
            {
                it->second -= fadeStep_ * dt;
                alSourcef(it->first->getALAudioSource(), AL_GAIN, it->second);
            }
            if(this->fadeOutList_.back().second <= 0)
            {
                bool pauseTest = false;
            
                for(std::list<AmbientSound*>::iterator it= this->ambientSounds_.begin(); it != this->ambientSounds_.end(); it++)
                {
                    if(*it == this->fadeOutList_.back().first)
                    {
                        pauseTest = true;
                        break;
                    }
                }
                if(pauseTest)
                {
                    this->fadeOutList_.back().first->pause();
                }
                else
                {
                    this->fadeOutList_.back().first->doStop();
                }
                this->fadeOutList_.pop_back();
            }
        }
    }

    void SoundManager::checkFadeStepValidity()
    {
        if(fadeStep_ <= 0.0 || fadeStep_ >= 1.0 )
        {
            ResetConfigValue(fadeStep_);
        }
        COUT(0) << "SoundManager: fade step now set to " << fadeStep_ << std::endl;
        return;
    }
}
