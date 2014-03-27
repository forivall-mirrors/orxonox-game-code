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
 *      Florian Zinggeler
 *   Co-authors:
 *      ...
 *
 */

/**
    @file JumpCenterPoint.cc
    @brief Implementation of the JumpCenterPoint class.
*/

#include "JumpCenterPoint.h"

#include "core/CoreIncludes.h"

#include "Jump.h"

namespace orxonox
{
    RegisterClass(JumpCenterPoint);

    JumpCenterPoint::JumpCenterPoint(Context* context) : StaticEntity(context)
    {
        RegisterObject(JumpCenterPoint);

        this->checkGametype();
    }

    void JumpCenterPoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpCenterPoint, XMLPort, xmlelement, mode);
    }

    void JumpCenterPoint::changedGametype()
    {
        SUPER(JumpCenterPoint, changedGametype);

        // Check, whether it's still Jump.
        this->checkGametype();
    }

    void JumpCenterPoint::checkGametype()
    {
        if (this->getGametype() != NULL && this->getGametype()->isA(Class(Jump)))
        {
            Jump* jumpGametype = orxonox_cast<Jump*>(this->getGametype().get());
            jumpGametype->setCenterpoint(this);
        }
    }
}
