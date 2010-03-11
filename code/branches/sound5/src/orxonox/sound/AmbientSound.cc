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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "AmbientSound.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/GameMode.h"
#include "core/Resource.h"
#include "core/XMLPort.h"
#include "SoundManager.h"
#include "SoundStreamer.h"

namespace orxonox
{
    CreateFactory(AmbientSound);

    AmbientSound::AmbientSound(BaseObject* creator)
        : BaseObject(creator)
        , Synchronisable(creator)
        , bPlayOnLoad_(false)
    {
        RegisterObject(AmbientSound);

        // Ambient sounds always fade in
        this->setVolume(0);
        this->registerVariables();
    }

    void AmbientSound::preDestroy()
    {
        if (GameMode::playsSound())
        {
            // Smoothly fade out by keeping a SmartPtr
            SoundManager::getInstance().unregisterAmbientSound(this);
        }
    }

    void AmbientSound::registerVariables()
    {
        registerVariable(ambientSource_, ObjectDirection::ToClient, new NetworkCallback<AmbientSound>(this, &AmbientSound::ambientSourceChanged));
        registerVariable(bLooping_,      ObjectDirection::ToClient, new NetworkCallback<AmbientSound>(this, &AmbientSound::loopingChanged));
        registerVariable(pitch_,         ObjectDirection::ToClient, new NetworkCallback<AmbientSound>(this, &AmbientSound::pitchChanged));
        registerVariable(bPlayOnLoad_,   ObjectDirection::ToClient, new NetworkCallback<AmbientSound>(this, &AmbientSound::playOnLoadChanged));
    }

    void AmbientSound::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AmbientSound, XMLPort, xmlelement, mode);
        BaseSound::XMLPortExtern(xmlelement, mode);
        XMLPortParam(AmbientSound, "ambientSource", setAmbientSource, getAmbientSource, xmlelement, mode);
        XMLPortParam(AmbientSound, "playOnLoad", setPlayOnLoad, getPlayOnLoad, xmlelement, mode);
    }

    void AmbientSound::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AmbientSound, XMLEventPort, xmlelement, mode);
        XMLPortEventState(AmbientSound, BaseObject, "play", play, xmlelement, mode);
    }

    void AmbientSound::play()
    {
        if (GameMode::playsSound())
            SoundManager::getInstance().registerAmbientSound(this);
    }

    void AmbientSound::stop()
    {
        if (GameMode::playsSound())
            SoundManager::getInstance().unregisterAmbientSound(this);
    }

    void AmbientSound::pause()
    {
        if (GameMode::playsSound())
            SoundManager::getInstance().pauseAmbientSound(this);
    }

    float AmbientSound::getRealVolume()
    {
        assert(GameMode::playsSound());
        return SoundManager::getInstance().getRealVolume(SoundType::Music);
    }

    void AmbientSound::setAmbientSource(const std::string& source)
    {
        this->ambientSource_ = source;
        this->moodChanged(this->getMood());
    }

    void AmbientSound::moodChanged(const std::string& mood)
    {
        if (GameMode::playsSound())
        {
            const std::string& path = "ambient/" + MoodManager::getInstance().getMood() + '/' + this->ambientSource_;
            shared_ptr<ResourceInfo> fileInfo = Resource::getInfo(path);
            if (fileInfo != NULL)
                this->setStreamSource(path);
            else
                COUT(3) << "Sound: " << this->ambientSource_ << ": Not a valid name! Ambient sound will not change." << std::endl;
        }
    }

    void AmbientSound::setPlayOnLoad(bool val)
    {
        this->bPlayOnLoad_ = val;
        if (val)
            this->play();
    }

    void AmbientSound::changedActivity()
    {
        SUPER(AmbientSound, changedActivity);
        if (this->isActive())
            this->play();
        else
            this->stop();
    }

    // hacky solution for file streaming
    void AmbientSound::setStreamSource(const std::string& source)
    {
        this->audioSource_ = SoundManager::getInstance().getSoundSource(this);
        if (this->source_ == source)
        {
            return;
        }

        this->source_ = source;
        // Don't load ""
        if (source_.empty())
            return;

        if (this->soundstreamthread_.get_id() != boost::thread::id())
        {
            this->soundstreamthread_.interrupt(); // unhandled interruptions lead to thread terminating ;-)
        }
        // Get resource info
        shared_ptr<ResourceInfo> fileInfo = Resource::getInfo(source);
        if (fileInfo == NULL)
        {
            COUT(2) << "Sound: Warning: Sound file '" << source << "' not found" << std::endl;
            return;
        }
        // Open data stream
        DataStreamPtr dataStream = Resource::open(fileInfo);

        this->soundstreamthread_ = boost::thread(SoundStreamer(), this->audioSource_, dataStream);

        this->updateVolume();
        this->setPitch(this->getPitch());
        this->setLooping(this->getLooping());
        alSource3f(this->audioSource_, AL_POSITION,  0, 0, 0);
        alSource3f(this->audioSource_, AL_VELOCITY,  0, 0, 0);
        alSource3f(this->audioSource_, AL_DIRECTION, 0, 0, 0);
        if (ALint error = alGetError())
            COUT(2) << "Sound: Warning: Setting source parameters to 0 failed: " << getALErrorString(error) << std::endl;
    }

    void AmbientSound::doStop()
    {
        BaseSound::doStop();
        this->soundstreamthread_.interrupt();
    }
}
