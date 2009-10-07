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

#include "core/EventIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(AmbientSound);

    AmbientSound::AmbientSound(BaseObject* creator)
        : BaseObject(creator)
    {
        RegisterObject(AmbientSound);
    }

    AmbientSound::~AmbientSound()
    {
    }

    void AmbientSound::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AmbientSound, XMLPort, xmlelement, mode);
        XMLPortParamExtern(AmbientSound, BaseSound, this, "source", setSource, getSource, xmlelement, mode);
        XMLPortParamExtern(AmbientSound, BaseSound, this, "loop", setLoop, getLoop, xmlelement, mode);
        XMLPortParamExtern(AmbientSound, BaseSound, this, "playOnLoad", setPlayOnLoad, getPlayOnLoad, xmlelement, mode);
    }

    void AmbientSound::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AmbientSound, XMLEventPort, xmlelement, mode);
        XMLPortEventState(AmbientSound, BaseObject, "play", play, xmlelement, mode);
    }
}
