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
#include "core/GameMode.h"
#include "core/Resource.h"
#include "SoundManager.h"

namespace orxonox
{
    AmbientSound::AmbientSound()
        : bPlayOnLoad_(false)
    {
        RegisterObject(AmbientSound);

        // Ambient sounds always fade in
        this->setVolume(0.0f);
    }

    void AmbientSound::preDestroy()
    {
        if (GameMode::playsSound())
        {
            // Smoothly fade out by keeping a SmartPtr
            SoundManager::getInstance().unregisterAmbientSound(this);
        }
    }

    void AmbientSound::play()
    {
        if (GameMode::playsSound())
            SoundManager::getInstance().registerAmbientSound(this);
    }

    bool AmbientSound::stop()
    {
        if (GameMode::playsSound())
            SoundManager::getInstance().unregisterAmbientSound(this);
        return false; // sound source not (yet) destroyed - return false
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
                this->setSource(path);
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
}
