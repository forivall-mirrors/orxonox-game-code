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
#include "core/ConfigValueIncludes.h"
#include "core/GameMode.h"
#include "core/ScopedSingletonManager.h"
#include "core/Resource.h"
#include "SoundBuffer.h"
#include "BaseSound.h"
#include "AmbientSound.h"
#include "WorldSound.h"

namespace orxonox
{
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
        
        this->setVolumeInternal(1.0, SoundType::none);
        this->setVolumeInternal(1.0, SoundType::ambient);
        this->setVolumeInternal(1.0, SoundType::effects);
        
        this->mute_[SoundType::none] = false;
        this->mute_[SoundType::ambient] = false;
        this->mute_[SoundType::effects] = false;

        this->setConfigValues();
    }

    SoundManager::~SoundManager()
    {
        GameMode::setPlaysSound(false);
        alcDestroyContext(this->context_);
        alcCloseDevice(this->device_);
        alutExit();
    }

    void SoundManager::preUpdate(const Clock& time)
    {
        this->processCrossFading(time.getDeltaTime());
    }

    void SoundManager::setConfigValues()
    {
        SetConfigValue(crossFadeStep_, 0.2f)
            .description("Determines how fast sounds should fade, per second.")
            .callback(this, &SoundManager::checkFadeStepValidity);
            
        SetConfigValue(soundVolume_, 1.0f)
            .description("Defines the overall volume.")
            .callback(this, &SoundManager::checkSoundVolumeValidity);
            
        SetConfigValue(ambientVolume_, 1.0f)
            .description("Defines the ambient volume.")
            .callback(this, &SoundManager::checkAmbientVolumeValidity);
            
        SetConfigValue(effectsVolume_, 1.0f)
            .description("Defines the effects volume.")
            .callback(this, &SoundManager::checkEffectsVolumeValidity);
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
    
    bool SoundManager::checkVolumeValidity(SoundType::Value type)
    {
        bool valid = true;
        
        if(this->getVolumeInternal(type) < 0.0 || this->getVolumeInternal(type) > 1.0)
        {
            COUT(2) << "Sound warning: Sound volume out of range, ignoring change." << std::endl;
            valid = false;
        }
        
        this->updateVolume(type);
        COUT(3) << "SoundManager: volume set to " << this->getVolumeInternal(type) << std::endl;
        return valid;
    }
    
    void SoundManager::checkSoundVolumeValidity()
    {
        if(!checkVolumeValidity(SoundType::none))
        {
            ResetConfigValue(soundVolume_);
        }
    }
    
    void SoundManager::checkAmbientVolumeValidity()
    {
        if(!checkVolumeValidity(SoundType::ambient))
        {
            ResetConfigValue(ambientVolume_);
        }
    }
    
    void SoundManager::checkEffectsVolumeValidity()
    {
        if(!checkVolumeValidity(SoundType::effects))
        {
            ResetConfigValue(effectsVolume_);
        }
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
    
    
    void SoundManager::setVolume(float vol, SoundType::Value type)
    {
        vol = this->checkVolumeRange(vol);
        
        this->setVolumeInternal(vol, type);
        
        this->updateVolume(type);
    }
    
    float SoundManager::checkVolumeRange(float vol)
    {
        if(vol < 0.0 || vol > 1.0)
        {
            COUT(2) << "Sound warning: volume out of range, cropping value." << std::endl;
            vol = vol > 1 ? 1 : vol;
            vol = vol < 0 ? 0 : vol;
        }
        
        return vol;
    }
    
    void SoundManager::updateVolume(SoundType::Value type)
    {
        switch(type)
        {
            case SoundType::none:
                for (ObjectList<BaseSound>::iterator it = ObjectList<BaseSound>::begin(); it != ObjectList<BaseSound>::end(); ++it)
                {
                    (*it)->updateVolume();
                }
                break;
            case SoundType::ambient:
                for (ObjectList<AmbientSound>::iterator it = ObjectList<AmbientSound>::begin(); it != ObjectList<AmbientSound>::end(); ++it)
                {
                    (*it)->updateVolume();
                }
                break;
            case SoundType::effects:
                for (ObjectList<WorldSound>::iterator it = ObjectList<WorldSound>::begin(); it != ObjectList<WorldSound>::end(); ++it)
                {
                    (*it)->updateVolume();
                }
                break;
            default:
                COUT(2) << "Invalid SoundType in SoundManager::updateVolume() - Not updating!" << std::endl;
        }
    }
    
    void SoundManager::setVolumeInternal(float vol, SoundType::Value type)
    {
        switch(type)
        {
            case SoundType::none:
                this->soundVolume_ = vol;
                break;
            case SoundType::ambient:
                this->ambientVolume_ = vol;
                break;
            case SoundType::effects:
                this->effectsVolume_ = vol;
                break;
            default:
                COUT(2) << "Invalid SoundType in SoundManager::setVolumeInternal() - Not setting any volume!" << std::endl;
        }
    }
    
    float SoundManager::getVolumeInternal(SoundType::Value type)
    {
        switch(type)
        {
            case SoundType::none:
                return this->soundVolume_;
            case SoundType::ambient:
                return this->ambientVolume_;
            case SoundType::effects:
                return this->effectsVolume_;
            default:
                COUT(2) << "Invalid SoundType in SoundManager::setVolumeInternal() - Returning 0.0!" << std::endl;
                return 0.0;
        }
    }
    
    float SoundManager::getVolume(SoundType::Value type) 
    {
        if(this->mute_[SoundType::none] || this->mute_[type])
            return 0.0;
        
        if(type == SoundType::none)
            return this->getVolumeInternal(type);
        
        return this->getVolumeInternal(SoundType::none)*this->getVolumeInternal(type);
    }
    
    void SoundManager::toggleMute(SoundType::Value type)
    {
        bool mute = !this->mute_[type];
        this->mute_[type] = mute;
        
        this->updateVolume(type);
    }
    
    bool SoundManager::getMute(SoundType::Value type)
    {
        return this->mute_[type];
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
        for (std::list<AmbientSound*>::iterator it= this->fadeInList_.begin(); it != this->fadeInList_.end(); )
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
        for (std::list<AmbientSound*>::iterator it = this->fadeOutList_.begin(); it != this->fadeOutList_.end(); )
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

    shared_ptr<SoundBuffer> SoundManager::getSoundBuffer(shared_ptr<ResourceInfo> fileInfo)
    {
        std::map<std::string, weak_ptr<SoundBuffer> >::const_iterator it
            = this->soundBuffers_.find(fileInfo->group + '/' + fileInfo->filename);
        if (it != this->soundBuffers_.end())
            return it->second.lock();
        else
        {
            shared_ptr<SoundBuffer> buffer;
            try
            {
                buffer.reset(new SoundBuffer(fileInfo));
            }
            catch (...)
            {
                COUT(1) << Exception::handleMessage() << std::endl;
                return shared_ptr<SoundBuffer>();
            }
            this->soundBuffers_[fileInfo->group + '/' + fileInfo->filename] = buffer;
            return buffer;
        }
    }

    void SoundManager::removeBuffer(shared_ptr<ResourceInfo> fileInfo)
    {
        std::map<std::string, weak_ptr<SoundBuffer> >::iterator it
            = this->soundBuffers_.find(fileInfo->group + '/' + fileInfo->filename);
        if (it != this->soundBuffers_.end())
            this->soundBuffers_.erase(it);
    }
}
