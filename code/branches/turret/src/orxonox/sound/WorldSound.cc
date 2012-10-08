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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "WorldSound.h"

#include <alut.h>

#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/XMLPort.h"
#include "Scene.h"
#include "SoundManager.h"

namespace orxonox
{
    CreateFactory(WorldSound);

    WorldSound::WorldSound(BaseObject* creator)
        : StaticEntity(creator)
    {
        RegisterObject(WorldSound);
        // WorldSound buffers should be pooled when they're not used anymore
        this->bPooling_ = true;
        this->registerVariables();
    }

    void WorldSound::registerVariables()
    {
        registerVariable(volume_,   ObjectDirection::ToClient, new NetworkCallback<WorldSound>(this, &WorldSound::volumeChanged));
        registerVariable(source_,   ObjectDirection::ToClient, new NetworkCallback<WorldSound>(this, &WorldSound::sourceChanged));
        registerVariable(bLooping_, ObjectDirection::ToClient, new NetworkCallback<WorldSound>(this, &WorldSound::loopingChanged));
        registerVariable(pitch_,    ObjectDirection::ToClient, new NetworkCallback<WorldSound>(this, &WorldSound::pitchChanged));
        registerVariable((uint8_t&)(BaseSound::state_), ObjectDirection::ToClient, new NetworkCallback<WorldSound>(this, &WorldSound::stateChanged));
    }

    void WorldSound::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WorldSound, XMLPort, xmlelement, mode);
        BaseSound::XMLPortExtern(xmlelement, mode);
    }

    void WorldSound::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WorldSound, XMLEventPort, xmlelement, mode);
        XMLPortEventState(WorldSound, BaseObject, "play", play, xmlelement, mode);
    }

    void WorldSound::initialiseSource()
    {
        BaseSound::initialiseSource();
        if (this->getScene())
        {
            float refDist = this->getScene()->getSoundReferenceDistance();
            alSourcef(this->audioSource_, AL_REFERENCE_DISTANCE, refDist);
            // TODO: 500 is very magical here. Derive something better
            alSourcef(this->audioSource_, AL_MAX_DISTANCE, refDist * 500);
        }
        this->tick(0); // update position, orientation and velocity
    }

    void WorldSound::tick(float dt)
    {
        if (alIsSource(this->audioSource_))
        {
            const Vector3& pos = this->getWorldPosition();
            alSource3f(this->audioSource_, AL_POSITION, pos.x, pos.y, pos.z);
            ALenum error = alGetError();
            if (error == AL_INVALID_VALUE)
                orxout(internal_error, context::sound) << "OpenAL: Invalid sound position" << endl;

            const Vector3& vel = this->getVelocity();
            alSource3f(this->audioSource_, AL_VELOCITY, vel.x, vel.y, vel.z);
            error = alGetError();
            if (error == AL_INVALID_VALUE)
                orxout(internal_error, context::sound) << "OpenAL: Invalid sound velocity" << endl;

            const Vector3& direction = -this->getWorldOrientation().zAxis();
            alSource3f(this->audioSource_, AL_DIRECTION, direction.x, direction.y, direction.z);
            error = alGetError();
            if (error == AL_INVALID_VALUE)
                orxout(internal_error, context::sound) << "OpenAL: Invalid sound direction" << endl;
        }
    }

    void WorldSound::changedActivity()
    {
        SUPER(WorldSound, changedActivity);
        if (this->isActive())
            this->play();
        else
            this->stop();
    }

    float WorldSound::getRealVolume()
    {
        assert(GameMode::playsSound());
        return SoundManager::getInstance().getRealVolume(SoundType::Effects);
    }
}
