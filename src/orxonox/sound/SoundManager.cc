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

#include <utility>
#include <alut.h>
#include <loki/ScopeGuard.h>

#include "util/Exception.h"
#include "util/Math.h"
#include "util/Clock.h"
#include "core/singleton/ScopedSingletonIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/Resource.h"
#include "SoundBuffer.h"
#include "BaseSound.h"
#include "AmbientSound.h"
#include "WorldSound.h"

namespace orxonox
{
    ManageScopedSingleton(SoundManager, ScopeID::Graphics, true);

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

    RegisterAbstractClass(SoundManager).inheritsFrom<Configurable>().inheritsFrom<UpdateListener>();

    SoundManager::SoundManager()
        : effectsPoolSize_(0)
    {
        RegisterObject(SoundManager);

        orxout(user_status) << "Loading sound" << endl;

        this->bDestructorCalled_ = false;
         
        // See whether we even want to load
        bool bDisableSound_ = false;
        SetConfigValue(bDisableSound_, false);
        if (bDisableSound_)
            ThrowException(InitialisationAborted, "Sound: Not loading at all");
#if !defined(ORXONOX_PLATFORM_APPLE)
        if (!alutInitWithoutContext(NULL, NULL))
            ThrowException(InitialisationFailed, "Sound Error: ALUT initialisation failed: " << alutGetErrorString(alutGetError()));
        Loki::ScopeGuard alutExitGuard = Loki::MakeGuard(&alutExit);
#endif

/*
        // Get list of available sound devices and display them
        const char* devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
        char* device = new char[strlen(devices)+1];
        strcpy(device, devices);
        std::string renderDevice;
        SetConfigValue(renderDevice, std::string(device)).description("Sound device used for rendering");
        orxout(verbose, context::sound) << "Sound: Available devices: ";
        while (true)
        {
            this->deviceNames_.push_back(devices);
            orxout(verbose, context::sound) << '"' << devices << "\", ";
            devices += strlen(devices) + 1;
            if (*devices == '\0')
                break;
        }
        orxout(verbose, context::sound) << endl;

        // Open the selected device
        orxout(internal_info, context::sound) << "Sound: Opening device \"" << renderDevice << '\' << endl;
        this->device_ = alcOpenDevice(renderDevice.c_str());
*/
        this->device_ = alcOpenDevice(NULL);
        if (this->device_ == NULL)
            ThrowException(InitialisationFailed, "Sound Error: Could not open sound device.");
        Loki::ScopeGuard closeDeviceGuard = Loki::MakeGuard(&alcCloseDevice, this->device_);

        // Create sound context and make it the currently used one
        this->context_ = alcCreateContext(this->device_, NULL);
        if (this->context_ == NULL)
            ThrowException(InitialisationFailed, "Sound Error: Could not create ALC context");
        Loki::ScopeGuard desroyContextGuard = Loki::MakeGuard(&alcDestroyContext, this->context_);
        if (!alcMakeContextCurrent(this->context_))
            ThrowException(InitialisationFailed, "Sound Error: Could not use ALC context");

        GameMode::setPlaysSound(true);
        Loki::ScopeGuard resetPlaysSoundGuard = Loki::MakeGuard(&GameMode::setPlaysSound, false);

        // Get some information about the sound
        if (const char* version = alGetString(AL_VERSION))
            orxout(internal_info, context::sound) << "Sound: --- OpenAL Version: " << version << endl;
        if (const char* vendor = alGetString(AL_VENDOR))
            orxout(internal_info, context::sound) << "Sound: --- OpenAL Vendor : " << vendor << endl;
        if (const char* types = alutGetMIMETypes(ALUT_LOADER_BUFFER))
            orxout(internal_info, context::sound) << "Sound: --- Supported MIME Types: " << types << endl;
        else
            orxout(internal_warning, context::sound) << "MIME Type retrieval failed: " << alutGetErrorString(alutGetError()) << endl;

        this->mute_[SoundType::All]     = 1.0f;
        this->mute_[SoundType::Music]   = 1.0f;
        this->mute_[SoundType::Effects] = 1.0f;

        this->setConfigValues();

        // Try to get exactly one source
        ALuint source;
        alGenSources(1, &source);
        if (!alGetError() && alIsSource(source))
            this->availableSoundSources_.push_back(source);
        else
            ThrowException(InitialisationFailed, "Sound Error: Could not create even a single source");
        // Create a few initial sources
        this->createSoundSources(this->minSources_ - 1);

        // Disarm guards
#if !defined(ORXONOX_PLATFORM_APPLE)
        alutExitGuard.Dismiss();
#endif
        closeDeviceGuard.Dismiss();
        desroyContextGuard.Dismiss();
        resetPlaysSoundGuard.Dismiss();

        orxout(internal_status, context::sound) << "Sound: Initialisation complete" << endl;
    }

    SoundManager::~SoundManager()
    {
        // Erase fade lists because of the smart pointers
        this->bDestructorCalled_ = true;
        this->fadeInList_.clear();
        this->fadeOutList_.clear();

        // If there are still used buffers around, well, that's just very bad...
        if (this->soundBuffers_.size() != this->effectsPool_.size())
            orxout(internal_error, context::sound) << "Some sound buffers are still in use but OpenAL is about to shut down. Fix this!" << endl;
        // Empty buffer pool and buffer list
        this->effectsPool_.clear();
        this->soundBuffers_.clear();

        // There should not be any sources in use anymore
        if (!this->usedSoundSources_.empty())
            orxout(internal_error, context::sound) << "Some sound sources are still in use but OpenAL is about to shut down. Fix this!" << endl;
        while (!this->availableSoundSources_.empty())
        {
            alDeleteSources(1, &this->availableSoundSources_.back());
            this->availableSoundSources_.pop_back();
        }

        GameMode::setPlaysSound(false);

        // Relieve context to destroy it
        if (!alcMakeContextCurrent(NULL))
            orxout(internal_error, context::sound) << "Could not unset ALC context" << endl;
        alcDestroyContext(this->context_);
        if (ALCenum error = alcGetError(this->device_))
        {
            if (error == AL_INVALID_OPERATION)
                orxout(internal_error, context::sound) << "Could not destroy ALC context because it is the current one" << endl;
            else
                orxout(internal_error, context::sound) << "Could not destroy ALC context because it is invalid" << endl;
        }
#ifdef AL_VERSION_1_1
        if (!alcCloseDevice(this->device_))
            orxout(internal_error, context::sound) << "Could not destroy ALC device. This might be because there are still buffers in use!" << endl;
#else
        alcCloseDevice(this->device_);
#endif
#if !defined(ORXONOX_PLATFORM_APPLE)
        if (!alutExit())
            orxout(internal_error, context::sound) << "Closing ALUT failed: " << alutGetErrorString(alutGetError()) << endl;
#endif
    }

    void SoundManager::setConfigValues()
    {
        SetConfigValue(crossFadeStep_, 0.2f)
            .description("Determines how fast sounds should fade, per second.")
            .callback(this, &SoundManager::checkFadeStepValidity);

        SetConfigValueAlias(volume_[SoundType::All], "soundVolume_", 1.0f)
            .description("Defines the overall volume.")
            .callback(this, &SoundManager::checkSoundVolumeValidity);
        SetConfigValueAlias(volume_[SoundType::Music], "ambientVolume_", 0.7f)
            .description("Defines the ambient volume.")
            .callback(this, &SoundManager::checkAmbientVolumeValidity);
        SetConfigValueAlias(volume_[SoundType::Effects], "effectsVolume_", 1.0f)
            .description("Defines the effects volume.")
            .callback(this, &SoundManager::checkEffectsVolumeValidity);

        SetConfigValue(minSources_, 16)
            .description("Minimum number of sources being generated (if possible)");
        SetConfigValue(maxSources_, 1024)
            .description("Maximum number of sources to be made available");
    }

    void SoundManager::preUpdate(const Clock& time)
    {
        this->processCrossFading(time.getDeltaTime());

        // Check whether a sound object has stopped playing
        for (unsigned int i = 0; i < this->usedSoundSources_.size(); ++i)
        {
            ALint state;
            alGetSourcei(this->usedSoundSources_[i].first, AL_SOURCE_STATE, &state);
            if (state == AL_STOPPED)
            {
                if (this->usedSoundSources_[i].second->stop()) // if stop() returns true, the sound source was removed, thus decrement the array index
                    --i;
            }
        }
    }

    void SoundManager::checkFadeStepValidity()
    {
        if (crossFadeStep_ <= 0.0 || crossFadeStep_ >= 1.0 )
        {
            orxout(internal_warning, context::sound) << "Fade step out of range, ignoring change." << endl;
            ResetConfigValue(crossFadeStep_);
        }
    }

    void SoundManager::checkVolumeValidity(SoundType::Value type)
    {
        float clampedVolume = clamp(this->volume_[type], 0.0f, 1.0f);
        if (clampedVolume != this->volume_[type])
            orxout(internal_warning, context::sound) << "Volume setting (" << type << ") out of range, clamping." << endl;
        this->updateVolume(type);
    }

    void SoundManager::setVolume(float vol, SoundType::Value type)
    {
        if (type < 0 || type > SoundType::Effects)
            return;
        this->volume_[type] = vol;
        this->checkVolumeValidity(type);
    }

    float SoundManager::getVolume(SoundType::Value type)
    {
        if (type < 0 || type > SoundType::Effects)
            return 0.0f;
        return this->volume_[type];
    }

    float SoundManager::getRealVolume(SoundType::Value type)
    {
        if (type != SoundType::Music && type != SoundType::Effects)
            return 0.0f;
        return this->volume_[SoundType::All] * this->mute_[SoundType::All] * this->volume_[type] * this->mute_[type];
    }

    void SoundManager::updateVolume(SoundType::Value type)
    {
        switch(type)
        {
        case SoundType::All:
            for (ObjectList<BaseSound>::iterator it = ObjectList<BaseSound>::begin(); it != ObjectList<BaseSound>::end(); ++it)
                (*it)->updateVolume();
            break;
        case SoundType::Music:
            for (ObjectList<AmbientSound>::iterator it = ObjectList<AmbientSound>::begin(); it != ObjectList<AmbientSound>::end(); ++it)
                (*it)->updateVolume();
            break;
        case SoundType::Effects:
            for (ObjectList<WorldSound>::iterator it = ObjectList<WorldSound>::begin(); it != ObjectList<WorldSound>::end(); ++it)
                (*it)->updateVolume();
            break;
        default:
            assert(false);
        }
    }

    void SoundManager::toggleMute(SoundType::Value type)
    {
        if (type < 0 || type > SoundType::Effects)
            return;
        this->mute_[type] = (this->mute_[type] == 0) ? 1.0f : 0.0f;
        this->updateVolume(type);
    }

    bool SoundManager::getMute(SoundType::Value type)
    {
        if (type < 0 || type > SoundType::Effects)
            return true;
        return (this->mute_[type] == 0);
    }

    void SoundManager::setListenerPosition(const Vector3& position)
    {
        alListener3f(AL_POSITION, position.x, position.y, position.z);
        ALenum error = alGetError();
        if (error == AL_INVALID_VALUE)
            // @TODO: Follow this constantly appearing, nerve-racking warning
            orxout(internal_error, context::sound) << "OpenAL: Invalid listener position" << endl;
    }

    void SoundManager::setListenerOrientation(const Quaternion& orientation)
    {
        // update listener orientation
        const Vector3& direction = -orientation.zAxis();
        const Vector3& up = orientation.yAxis();

        ALfloat orient[6] = { direction.x, direction.y, direction.z, up.x, up.y, up.z };

        alListenerfv(AL_ORIENTATION, orient);
        ALenum error = alGetError();
        if (error == AL_INVALID_VALUE)
            orxout(internal_error, context::sound) << "OpenAL: Invalid listener orientation" << endl;
    }

    void SoundManager::registerAmbientSound(AmbientSound* newAmbient)
    {
        if (newAmbient != NULL && !this->bDestructorCalled_)
        {
            for (AmbientList::const_iterator it = this->ambientSounds_.begin(); it != this->ambientSounds_.end(); ++it)
            {
                if (it->first == newAmbient)
                {
                    orxout(internal_warning, context::sound) << "Will not play an AmbientSound twice." << endl;
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
        if (oldAmbient == NULL || ambientSounds_.empty() || this->bDestructorCalled_)
            return;

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

    void SoundManager::fadeIn(const SmartPtr<AmbientSound>& sound)
    {
        // If we're already fading out --> remove that
        for (std::list<SmartPtr<AmbientSound> >::iterator it = this->fadeOutList_.begin(); it != this->fadeOutList_.end(); it++)
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

    void SoundManager::fadeOut(const SmartPtr<AmbientSound>& sound)
    {
        // If we're already fading in --> remove that
        for (std::list<SmartPtr<AmbientSound> >::iterator it = this->fadeInList_.begin(); it != this->fadeInList_.end(); it++)
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
        for (std::list<SmartPtr<AmbientSound> >::iterator it= this->fadeInList_.begin(); it != this->fadeInList_.end(); )
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
        for (std::list<SmartPtr<AmbientSound> >::iterator it = this->fadeOutList_.begin(); it != this->fadeOutList_.end(); )
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

    shared_ptr<SoundBuffer> SoundManager::getSoundBuffer(const std::string& filename)
    {
        shared_ptr<SoundBuffer> buffer;
        // Check active or pooled buffers
        SoundBufferMap::const_iterator it = this->soundBuffers_.find(filename);
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
                buffer.reset(new SoundBuffer(filename, this->effectsPool_.end()));
            }
            catch (const std::exception& ex)
            {
                orxout(internal_error, context::sound) << ex.what() << endl;
                return buffer;
            }
            this->soundBuffers_[filename] = buffer;
        }
        return buffer;
    }

    void SoundManager::releaseSoundBuffer(const shared_ptr<SoundBuffer>& buffer, bool bPoolBuffer)
    {
        // Check if others are still using the buffer
        if (buffer.use_count() != 2)
            return;
        SoundBufferMap::iterator it = this->soundBuffers_.find(buffer->getFilename());
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
                    SoundBufferMap::iterator itDel = this->soundBuffers_.find(bufferDel->getFilename());
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

    ALuint SoundManager::getSoundSource(BaseSound* object)
    {
        if (!this->availableSoundSources_.empty())
        {
            ALuint source = this->availableSoundSources_.back();
            this->availableSoundSources_.pop_back();
            this->usedSoundSources_.push_back(std::make_pair(source, object));
            return source;
        }
        else
        {
            if (this->usedSoundSources_.size() < this->maxSources_)
            {
                ALuint source;
                alGenSources(1, &source);
                // Try to create new sources (50% more, but at least one)
                if (alIsSource(source) && !alGetError())
                {
                    this->usedSoundSources_.push_back(std::make_pair(source, object));
                    return source;
                }
            }
            // Return no source ID
            ALuint source = 123456789;
            while (alIsSource(++source));
            return source;
        }
    }

    void SoundManager::releaseSoundSource(ALuint source)
    {
#ifndef NDEBUG
        for (std::vector<ALuint>::const_iterator it = this->availableSoundSources_.begin(); it != this->availableSoundSources_.end(); ++it)
            assert((*it) != source);
#endif
        this->availableSoundSources_.push_back(source);
        for (std::vector<std::pair<ALuint, BaseSound*> >::iterator it = this->usedSoundSources_.begin();
            it != this->usedSoundSources_.end(); ++it)
        {
            if (it->first == source)
            {
                this->usedSoundSources_.erase(it);
                break;
            }
        }
        int used = std::max((unsigned int)(this->usedSoundSources_.size()), this->minSources_);
        // Subtract those we added in the statement above trough std::max
        int available = (int)this->availableSoundSources_.size() - (used - (int)this->usedSoundSources_.size());
        // Delete sources again to free resources if appropriate (more than 50% more available than used)
        int toDelete = available - used / 2;
        while (toDelete-- > 0)
        {
            alDeleteSources(1, &this->availableSoundSources_.back());
            if (alGetError())
                orxout(internal_error, context::sound) << "Failed to delete a source --> lost forever" << endl;
            this->availableSoundSources_.pop_back();
        }
    }

    unsigned int SoundManager::createSoundSources(unsigned int n)
    {
        unsigned int count = this->availableSoundSources_.size() + this->usedSoundSources_.size();
        while (count < this->maxSources_ && count <= n)
        {
            ALuint source;
            alGenSources(1, &source);
            if (alIsSource(source) && !alGetError())
                this->availableSoundSources_.push_back(source);
            else
                break;
            ++count;
        }
        return count - this->availableSoundSources_.size() - this->usedSoundSources_.size();
    }
}
