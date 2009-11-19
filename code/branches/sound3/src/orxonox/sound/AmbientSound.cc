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
#include "MoodManager.h"

namespace orxonox
{
    CreateFactory(AmbientSound);

    AmbientSound::AmbientSound(BaseObject* creator)
        : BaseObject(creator)
    {
        RegisterObject(AmbientSound);

        // Ambient sounds always fade in
        this->setVolume(0);
    }

    AmbientSound::~AmbientSound()
    {
    }

    void AmbientSound::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AmbientSound, XMLPort, xmlelement, mode);
        XMLPortParamExtern(AmbientSound, BaseSound, this, "volume", setVolume, getVolume, xmlelement, mode);
        XMLPortParamExtern(AmbientSound, BaseSound, this, "loop", setLooping, getLooping, xmlelement, mode);
        XMLPortParamExtern(AmbientSound, BaseSound, this, "play", play, isPlaying, xmlelement, mode);
        XMLPortParam(AmbientSound, "source", setAmbientSource, getAmbientSource, xmlelement, mode);
    }

    void AmbientSound::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AmbientSound, XMLEventPort, xmlelement, mode);
        XMLPortEventState(AmbientSound, BaseObject, "play", play, xmlelement, mode);
    }

    void AmbientSound::play()
    {
        if (GameMode::playsSound())
        {
            COUT(3) << "Sound: " << this->getSource() << ": Playing" << std::endl;
            SoundManager::getInstance().registerAmbientSound(this);
        }
    }

    void AmbientSound::doPlay()
    {
        BaseSound::play();
    }

    void AmbientSound::stop()
    {
        if (GameMode::playsSound())
        {
            SoundManager::getInstance().unregisterAmbientSound(this);
        }
    }

    void AmbientSound::doStop()
    {
        BaseSound::stop();
    }

    void AmbientSound::pause()
    {
        if (GameMode::playsSound())
        {
            SoundManager::getInstance().pauseAmbientSound(this);
        }
    }

    void AmbientSound::doPause()
    {
        BaseSound::pause();
    }

    void AmbientSound::setAmbientSource(const std::string& source)
    {
        this->ambientSource_ = source;
        if (GameMode::playsSound())
        {
            std::string path = "ambient/" + MoodManager::getInstance().getMood() + "/" + source;
            shared_ptr<ResourceInfo> fileInfo = Resource::getInfo(path);
            if (fileInfo != NULL)
                this->setSource(path);
            else
                COUT(3) << "Sound: " << source << ": Not a valid name! Ambient sound will not change." << std::endl;       
        }
    }

    void AmbientSound::changedActivity() 
    {
        SUPER(AmbientSound, changedActivity);
        if (this->isActive())
            this->play();
        else 
            this->stop();
    }
}
