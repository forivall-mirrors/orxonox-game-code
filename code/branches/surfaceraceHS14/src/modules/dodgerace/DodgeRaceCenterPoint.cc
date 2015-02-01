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
    @file DodgeRaceCenterPoint.cc
    @brief Implementation of the DodgeRaceCenterPoint class.
*/

#include "DodgeRaceCenterPoint.h"

#include "core/CoreIncludes.h"

#include "DodgeRace.h"

namespace orxonox
{
    RegisterClass(DodgeRaceCenterPoint);

    DodgeRaceCenterPoint::DodgeRaceCenterPoint(Context* context) : StaticEntity(context)
    {
        RegisterObject(DodgeRaceCenterPoint);

        this->checkGametype();
    }

    void DodgeRaceCenterPoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(DodgeRaceCenterPoint, XMLPort, xmlelement, mode);
    }

    void DodgeRaceCenterPoint::changedGametype()
    {
        SUPER(DodgeRaceCenterPoint, changedGametype);

        // Check, whether it's still DodgeRace.
        this->checkGametype();
    }

    void DodgeRaceCenterPoint::checkGametype()
    {
        if (this->getGametype() != NULL && this->getGametype()->isA(Class(DodgeRace)))
        {
            DodgeRace* DodgeRaceGametype = orxonox_cast<DodgeRace*>(this->getGametype().get());
            DodgeRaceGametype->setCenterpoint(this);
        }
    }
}
