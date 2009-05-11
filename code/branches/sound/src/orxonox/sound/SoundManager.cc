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

#include <AL/alut.h>

#include "orxonox/CameraManager.h"
#include "orxonox/objects/worldentities/Camera.h"
#include "util/Math.h"
#include "SoundBase.h"
#include "SoundManager.h"

namespace orxonox
{
    ALCdevice* SoundManager::device_s = NULL;

    /**
     * Default constructor
     */
    SoundManager::SoundManager()
    {
        if(!alutInitWithoutContext(NULL,NULL))
        {
            COUT(2) << "OpenAL ALUT: " << alutGetErrorString(alutGetError()) << std::endl;
        }
        else
        {
            COUT(4) << "OpenAL ALUT version:" << alutGetMajorVersion() << "." << alutGetMinorVersion() << std::endl;
            COUT(4) << "OpenAL ALUT supported MIME types:" << alutGetMIMETypes(ALUT_LOADER_BUFFER) << std::endl;
            if(SoundManager::device_s == NULL)
            {
                COUT(3) << "OpenAL: open sound device..." << std::endl;
                SoundManager::device_s = alcOpenDevice(NULL);
            }

            if(SoundManager::device_s == NULL)
            {
                COUT(2) << "OpenAL: Could not open sound device" << std::endl;
            }
            else
            {
                this->context_ = alcCreateContext(SoundManager::device_s, NULL);
                if(this->context_ == NULL)
                {
                    COUT(2) << "OpenAL: Could not create sound context";
                }
                else
                {
                    alcMakeContextCurrent(this->context_);
                }
            }
        }
    }

    SoundManager::~SoundManager()
    {
        alcDestroyContext(this->context_);
        alcCloseDevice(SoundManager::device_s);
        alutExit();
    }

    /**
     * Add a SoundBase object to the list. Every SoundBase object should be in
     * this list.
     *
     * @param sound Pointer to the SoundBase object to add
     */
    void SoundManager::addSound(SoundBase* sound)
    {
        this->soundlist_.push_back(sound);
    }

    /**
     * Remove a SoundBase object from the list and destroy it.
     */
    void SoundManager::removeSound(SoundBase* sound)
    {
        std::list<SoundBase*>::iterator pos = this->soundlist_.end();
        for(std::list<SoundBase*>::iterator i = this->soundlist_.begin(); i != this->soundlist_.end(); i++)
        {
            if((*i) == sound)
                pos = i;
        }

        delete (*pos);
        this->soundlist_.erase(pos);
    }

    /**
     * Tick function, updates listener and registred SoundBase objects
     *
     * @param dt @see Orxonox::Tickable
     */
    void SoundManager::tick(float dt)
    {
        // update listener position
        Camera* camera = CameraManager::getInstance().getActiveCamera();
        if(camera == NULL) return;
        Vector3 pos = camera->getPosition();
        alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
        ALenum error = alGetError();
        if(error == AL_INVALID_VALUE)
            COUT(2) << "OpenAL: Invalid listener position" << std::endl;

        // update listener orientation
        Quaternion orient = camera->getOrientation();
        Vector3 up = orient.xAxis(); // just a wild guess
        Vector3 at = orient.zAxis();

        ALfloat orientation[6] = { at.x, at.y, at.z, 
                                 up.x, up.y, up.z };

        alListenerfv(AL_POSITION, orientation);
        error = alGetError();
        if(error == AL_INVALID_VALUE)
            COUT(2) << "OpenAL: Invalid listener orientation" << std::endl;

        // update sounds
        for(std::list<SoundBase*>::iterator i = this->soundlist_.begin(); i != this->soundlist_.end(); i++)
            (*i)->update();
    }
}
