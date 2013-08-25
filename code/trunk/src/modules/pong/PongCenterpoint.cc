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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @file PongCenterpoint.cc
    @brief Implementation of the PongCenterpoint class.
*/

#include "PongCenterpoint.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Pong.h"

namespace orxonox
{
    RegisterClass(PongCenterpoint);

    /**
    @brief
        Constructor. Registers and initializes the object and checks whether the gametype is actually Pong.
    */
    PongCenterpoint::PongCenterpoint(Context* context) : StaticEntity(context)
    {
        RegisterObject(PongCenterpoint);

        this->width_ = 200;
        this->height_ = 120;
        this->ballspeed_ = 100;
        this->ballaccfactor_ = 1.0;
        this->batspeed_ = 60;
        this->batlength_ = 0.25;

        this->checkGametype();
    }

    /**
    @brief
        Method to create a PongCenterpoint through XML.
    */
    void PongCenterpoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PongCenterpoint, XMLPort, xmlelement, mode);

        XMLPortParam(PongCenterpoint, "dimension", setFieldDimension, getFieldDimension, xmlelement, mode);
        XMLPortParam(PongCenterpoint, "balltemplate", setBalltemplate, getBalltemplate, xmlelement, mode);
        XMLPortParam(PongCenterpoint, "battemplate", setBattemplate, getBattemplate, xmlelement, mode);
        XMLPortParam(PongCenterpoint, "ballspeed", setBallSpeed, getBallSpeed, xmlelement, mode);
        XMLPortParam(PongCenterpoint, "ballaccfactor", setBallAccelerationFactor, getBallAccelerationFactor, xmlelement, mode);
        XMLPortParam(PongCenterpoint, "batspeed", setBatSpeed, getBatSpeed, xmlelement, mode);
        XMLPortParam(PongCenterpoint, "batlength", setBatLength, getBatLength, xmlelement, mode);
    }

    /**
    @brief
        Is called when the gametype has changed.
    */
    void PongCenterpoint::changedGametype()
    {
        SUPER(PongCenterpoint, changedGametype);

        // Check, whether it's still Pong.
        this->checkGametype();
    }

    /**
    @brief
        Checks whether the gametype is Pong and if it is, sets its centerpoint.
    */
    void PongCenterpoint::checkGametype()
    {
        if (this->getGametype() != NULL && this->getGametype()->isA(Class(Pong)))
        {
            Pong* pongGametype = orxonox_cast<Pong*>(this->getGametype().get());
            pongGametype->setCenterpoint(this);
        }
    }
}
