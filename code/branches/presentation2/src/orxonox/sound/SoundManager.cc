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
 *       Reto Grieder
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
        : effectsPoolSize_(0)
    {
        RegisterRootObject(SoundManager);

        if (!alutInitWithoutContext(NULL, NULL))
            ThrowException(InitialisationFailed, "Sound Error: ALUT initialisation failed: " << alutGetErrorString(alutGetError()));
        Loki::ScopeGuard alutExitGuard = Loki::MakeGuard(&alutExit);

        // Get list of available sound devices and display them
        const char* devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
        std::string renderDevice;
        SetConfigValue(renderDevice, devices).description("Sound device used for rendering");
        COUT(4) << "Sound: Available devices: ";
        while (true)
        {
            this->deviceNames_.push_back(devices);
            COUT(4) << "\"" << devices << "\", ";
            devices += strlen(devices) + 1;
            if (*devices == '\0')
                break;
        }
        COUT(4) << std::endl;

        // Open the selected device
        COUT(3) << "Sound: Opening device \"" << renderDevice << "\"" << std::endl;
        this->device_ = alcOpenDevice(renderDevice.c_str());
        if (this->device_ == NULL)
        {
            COUT(1) << "Sound: Could not open sound device. Have you installed OpenAL?" << std::endl;
#ifdef ORXONOX_PLATFORM_WINDOWS
            COUT(1) << "Sound: Just getting the DLL with the dependencies is not enough for Windows (esp. Windows 7)!" << std::endl;
#endif
            ThrowException(InitialisationFailed, "Sound: OpenAL error: Could not open sound device.");
        }
        Loki::ScopeGuard closeDeviceGuard = Loki::MakeGuard(&alcCloseDevice, this->device_);

        // Create sound context and make it the currently used one
        this->context_ = alcCreateContext(this->device_, NULL);
        if (this->context_ == NULL)
            ThrowException(InitialisationFailed, "Sound Error: Could not create ALC context");
        Loki::ScopeGuard desroyContextGuard = Loki::MakeGuard(&alcDestroyContext, this->context_);
        if (!alcMakeContextCurrent(this->context_))
            ThrowException(InitialisationFailed, "Sound Error: Could not use ALC context");

        GameMode::setPlaysSound(true);

        // Get some information about the sound
        if (const char* version = alGetString(AL_VERSION))
            COUT(4) << "Sound: --- OpenAL Version: " << version << std::endl;
        if (const char* vendor = alGetString(AL_VENDOR))
            COUT(4) << "Sound: --- OpenAL Vendor : " << vendor << std::endl;
        if (const char* types = alutGetMIMETypes(ALUT_LOADER_BUFFER))
            COUT(4) << "Sound: --- Supported MIME Types: " << types << std::endl;
        else
            COUT(2) << "Sound Warning: MIME Type retrieval failed: " << alutGetErrorString(alutGetError()) << std::endl;

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

        COUT(4) << "Sound: Initialisation complete" << std::endl;
    }

    SoundManager::~SoundManager()
    {
        GameMode::setPlaysSound(false);

        // Relieve context to destroy it
        if (!alcMakeContextCurrent(NULL))
            COUT(1) << "Sound Error: Could not unset ALC context" << std::endl;
        alcDestroyContext(this->context_);
        if (ALCenum error = alcGetError(this->device_))
        {
            if (error == AL_INVALID_OPERATION)
                COUT(1) << "Sound Error: Could not destroy ALC context because it is the current one" << std::endl;
            else
                COUT(1) << "Sound Error: Could not destroy ALC context because it is invalid" << std::endl;
        }
#ifdef AL_VERSION_1_1
        if (!alcCloseDevice(this->device_))
            COUT(1) << "Sound Error: Could not destroy ALC device. This might be because there are still buffers in use!" << std::endl;
#else
        alcCloseDevice(this->device_);
#endif
        if (!alutExit())
            COUT(1) << "Sound Error: Closing ALUT failed: " << alutGetErrorString(alutGetError()) << std::endl;
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

    std::string SoundManager::getALErrorString(ALenum code)
    {
        switch (code)
        {
        case AL_NO_ERROR:          return "No error";
        case AL_INVALID_NAME:      return "Invalid AL parameter name";
        case AL_INVALID_ENUM:      return "Invalid AL enum";
        case AL_INVALID_VALUE:     return "Invalid AL value";
        case AL_INVALID_OPERATION: return "Invalid AL operation";
        case AL_OUT_OF_MEMORY:     return "AL reports out of memory";
        default:                   return "Unknown AL error";
        }
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
        COUT(4) << "SoundManager: volume set to " << this->getVolumeInternal(type) << std::endl;
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
        shared_ptr<SoundBuffer> buffer;
        // Check active or pooled buffers
        SoundBufferMap::const_iterator it = this->soundBuffers_.find(fileInfo->group + '/' + fileInfo->filename);
        if (it != this->soundBuffers_.end())
        {
            buffer = it->second;

            // Remove from effects pool if not active used before
            if (buffer->poolIterator_ != this->effectsPool_.end())
            {
                this->effectsPoolSize_ -= buffer->getSize();
                this->effectsPool_.erase(buffer->poolIterator_);
                buffer->poolIterator_ = this->effectsPool_.end();
            }
        }
        else
        {
            try
            {
                buffer.reset(new SoundBuffer(fileInfo));
                buffer->poolIterator_ = this->effectsPool_.end();
            }
            catch (...)
            {
                COUT(1) << Exception::handleMessage() << std::endl;
                return buffer;
            }
            this->soundBuffers_[fileInfo->group + '/' + fileInfo->filename] = buffer;
        }
        return buffer;
    }

    void SoundManager::releaseSoundBuffer(const shared_ptr<SoundBuffer>& buffer, bool bPoolBuffer)
    {
        // Check if others are still using the buffer
        if (buffer.use_count() != 2)
            return;
        SoundBufferMap::iterator it = this->soundBuffers_.find(buffer->fileInfo_->group + '/' + buffer->fileInfo_->filename);
        if (it != this->soundBuffers_.end())
        {
            if (bPoolBuffer)
            {
                // Pool already too large?
                while (this->effectsPoolSize_ + it->second->getSize() > this->maxEffectsPoolSize_s && !this->effectsPool_.empty())
                {
                    shared_ptr<SoundBuffer> bufferDel = this->effectsPool_.back();
                    this->effectsPoolSize_ -= bufferDel->getSize();
                    bufferDel->poolIterator_ = this->effectsPool_.end();
                    this->effectsPool_.pop_back();
                    // Remove from buffer map too
                    SoundBufferMap::iterator itDel = this->soundBuffers_.find(bufferDel->fileInfo_->group + '/' + bufferDel->fileInfo_->filename);
                    if (itDel != this->soundBuffers_.end())
                        this->soundBuffers_.erase(itDel);
                }
                // Put buffer into the pool
                this->effectsPoolSize_ += it->second->getSize();
                this->effectsPool_.push_front(it->second);
                it->second->poolIterator_ = this->effectsPool_.begin();
            }
            else
                this->soundBuffers_.erase(it);
        }
    }
}
