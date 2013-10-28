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
    @file InvaderCenterPoint.cc
    @brief Implementation of the InvaderCenterPoint class.
*/

#include "InvaderCenterPoint.h"

#include "core/CoreIncludes.h"

#include "Invader.h"

namespace orxonox
{
    RegisterClass(InvaderCenterPoint);

    /**
    @brief
        Constructor. Registers and initializes the object and checks whether the gametype is actually Invader.
    */
    InvaderCenterPoint::InvaderCenterPoint(Context* context) : StaticEntity(context)
    {
        RegisterObject(InvaderCenterPoint);

        this->checkGametype();
    }

    /**
    @brief
        Method to create a InvaderCenterPoint through XML.
    */
    void InvaderCenterPoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(InvaderCenterPoint, XMLPort, xmlelement, mode);

//        XMLPortParam(InvaderCenterPoint, "dimension", setFieldDimension, getFieldDimension, xmlelement, mode);

    }

    /**
    @brief
        Is called when the gametype has changed.
    */
    void InvaderCenterPoint::changedGametype()
    {
        SUPER(InvaderCenterPoint, changedGametype);

        // Check, whether it's still Invader.
        this->checkGametype();
    }

    /**
    @brief
        Checks whether the gametype is Invader and if it is, sets its centerpoint.
    */
    void InvaderCenterPoint::checkGametype()
    {
        if (this->getGametype() != NULL && this->getGametype()->isA(Class(Invader)))
        {
            Invader* InvaderGametype = orxonox_cast<Invader*>(this->getGametype().get());
            InvaderGametype->setCenterpoint(this);
        }
    }
}
