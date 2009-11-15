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

#include <AL/alut.h>
#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(WorldSound);

    WorldSound::WorldSound(BaseObject* creator)
        : StaticEntity(creator)
    {
        RegisterObject(WorldSound);
    }

    WorldSound::~WorldSound()
    {
    }

    void WorldSound::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WorldSound, XMLPort, xmlelement, mode);
        XMLPortParamExtern(WorldSound, BaseSound, this, "volume", setVolume, getVolume, xmlelement, mode);
        XMLPortParamExtern(WorldSound, BaseSound, this, "loop", setLoop, getLoop, xmlelement, mode);
        XMLPortParamExtern(WorldSound, BaseSound, this, "playOnLoad", setPlayOnLoad, getPlayOnLoad, xmlelement, mode);
        XMLPortParamExtern(WorldSound, BaseSound, this, "source", setSource, getSource, xmlelement, mode);
    }

    void WorldSound::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WorldSound, XMLEventPort, xmlelement, mode);
        XMLPortEventState(WorldSound, BaseObject, "play", play, xmlelement, mode);
    }

    void WorldSound::tick(float dt)
    {
        if (alIsSource(this->audioSource_))
        {
            const Vector3& pos = this->getWorldPosition();
            alSource3f(this->audioSource_, AL_POSITION, pos.x, pos.y, pos.z);
            ALenum error = alGetError();
            if (error == AL_INVALID_VALUE)
                COUT(2) << "Sound: OpenAL: Invalid sound position" << std::endl;

            const Vector3& vel = this->getVelocity();
            alSource3f(this->audioSource_, AL_VELOCITY, vel.x, vel.y, vel.z);
            error = alGetError();
            if (error == AL_INVALID_VALUE)
                COUT(2) << "Sound: OpenAL: Invalid sound velocity" << std::endl;

            const Quaternion& orient = this->getWorldOrientation();
            Vector3 at = orient.zAxis();
            alSource3f(this->audioSource_, AL_DIRECTION, at.x, at.y, at.z);
            error = alGetError();
            if (error == AL_INVALID_VALUE)
                COUT(2) << "Sound: OpenAL: Invalid sound direction" << std::endl;
        }
    }
}
