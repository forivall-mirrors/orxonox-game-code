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
 *       Kevin Young
 *   Co-authors:
 *      ...
 *
 */

#include "SoundManager.h"

#include <AL/alut.h>
#include <utility>

#include "util/Exception.h"
#include "util/Math.h"
#include "util/ScopeGuard.h"
#include "util/StringUtils.h"
#include "util/Clock.h"
#include "core/GameMode.h"
#include "core/ScopedSingletonManager.h"
#include "core/ConfigValueIncludes.h"
#include "BaseSound.h"
#include "AmbientSound.h"

namespace orxonox
{
    SoundManager* SoundManager::singletonPtr_s = NULL;
    ManageScopedSingleton(SoundManager, ScopeID::Graphics, true);

    SoundManager::SoundManager()
    {
        RegisterRootObject(SoundManager);

        if (!alutInitWithoutContext(NULL, NULL))
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

    void SoundManager::update(const Clock& time)
    {
        this->processCrossFading(time.getDeltaTime());
    }

    void SoundManager::setConfigValues()
    {
        SetConfigValue(crossFadeStep_, 0.2f)
            .description("Determines how fast sounds should fade, per second.")
            .callback(this, &SoundManager::checkFadeStepValidity);
    }

    void SoundManager::checkFadeStepValidity()
    {
        if (crossFadeStep_ <= 0.0 || crossFadeStep_ >= 1.0 )
        {
            COUT(2) << "Sound warning: Sound step out of range, ignoring change." << std::endl;
            ResetConfigValue(crossFadeStep_);
        }
        COUT(3) << "SoundManager: fade step set to " << crossFadeStep_ << std::endl;
        return;
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
        if (newAmbient != NULL)
        {
            for (AmbientList::const_iterator it = this->ambientSounds_.begin(); it != this->ambientSounds_.end(); ++it)
            {
                if (it->first == newAmbient)
                {
                    COUT(2) << "Sound warning: Will not play an AmbientSound twice." << std::endl;
                    return;
                }
            }

            if (!this->ambientSounds_.empty()) 
            {
                this->fadeOut(ambientSounds_.front().first);
            }
            this->ambientSounds_.push_front(std::make_pair(newAmbient, false));
            newAmbient->doPlay();
            this->fadeIn(newAmbient);
        }
    }

    void SoundManager::unregisterAmbientSound(AmbientSound* oldAmbient)
    {
        if (oldAmbient == NULL || ambientSounds_.empty())
        {
            return;
        }
        if (this->ambientSounds_.front().first == oldAmbient) 
        {
            this->fadeOut(oldAmbient);
            this->ambientSounds_.pop_front();
            if (!this->ambientSounds_.empty())
            {
                if (!this->ambientSounds_.front().second) // Not paused before
                {
                    this->ambientSounds_.front().first->doPlay();
                }
                this->fadeIn(this->ambientSounds_.front().first);
            }
        }
        else
        {
            for (AmbientList::iterator it = this->ambientSounds_.begin(); it != this->ambientSounds_.end(); ++it)
            {
                if (it->first == oldAmbient)
                {
                    this->fadeOut(oldAmbient);
                    this->ambientSounds_.erase(it);
                    break;
                }
            }
        }
    }

    void SoundManager::pauseAmbientSound(AmbientSound* ambient)
    {
        if (ambient != NULL)
        {
            for (AmbientList::iterator it = this->ambientSounds_.begin(); it != this->ambientSounds_.end(); ++it)
            {
                if (it->first == ambient)
                {
                    it->second = true;
                    this->fadeOut(it->first);
                    return;
                }
            }
        }
    }

    void SoundManager::fadeIn(AmbientSound* sound)
    {
        // If we're already fading out --> remove that
        for (std::list<AmbientSound*>::iterator it = this->fadeOutList_.begin(); it != this->fadeOutList_.end(); it++)
        {
            if (*it == sound)
            {
                this->fadeOutList_.erase(it);
                break;
            }
        }
        // No duplicate entries
        if (std::find(this->fadeInList_.begin(), this->fadeInList_.end(), sound) == this->fadeInList_.end())
            this->fadeInList_.push_back(sound);
    }

    void SoundManager::fadeOut(AmbientSound* sound)
    {
        // If we're already fading in --> remove that
        for (std::list<AmbientSound*>::iterator it = this->fadeInList_.begin(); it != this->fadeInList_.end(); it++)
        {
            if (*it == sound)
            {
                this->fadeInList_.erase(it);
                break;
            }
        }
        // No duplicate entries
        if (std::find(this->fadeOutList_.begin(), this->fadeOutList_.end(), sound) == this->fadeOutList_.end())
            this->fadeOutList_.push_back(sound);
    }

    void SoundManager::processCrossFading(float dt)
    {
        
        // Hacky solution to the fade delay while loading a level.
        if(dt > 0.2)
        {
            return;
        }
        
        // FADE IN
        for (std::list<AmbientSound*>::iterator it= this->fadeInList_.begin(); it != this->fadeInList_.end(); it)
        {
            if ((*it)->getVolume() + this->crossFadeStep_*dt > 1.0f)
            {
                (*it)->setVolume(1.0f);
                this->fadeInList_.erase(it++);
            }
            else
            {
                (*it)->setVolume((*it)->getVolume() + this->crossFadeStep_*dt);
                ++it;
            }
        }

        // FADE OUT
        for (std::list<AmbientSound*>::iterator it = this->fadeOutList_.begin(); it != this->fadeOutList_.end(); it)
        {
            if ((*it)->getVolume() - this->crossFadeStep_*dt < 0.0f)
            {
                (*it)->setVolume(0.0f);

                // If sound is in the ambient list --> pause
                for (AmbientList::const_iterator it2 = this->ambientSounds_.begin(); it2 != this->ambientSounds_.end(); ++it2)
                {
                    if (it2->first == *it)
                    {
                        (*it)->doPause();
                        break;
                    }
                }
                // If not pause (by loop above for instance) --> stop
                if (!(*it)->isPaused())
                    (*it)->doStop();

                this->fadeOutList_.erase(it++);
            }
            else
            {
                (*it)->setVolume((*it)->getVolume() - this->crossFadeStep_*dt);
                ++it;
            }
        }
    }
}
