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
    @file JumpCenterpoint.cc
    @brief Implementation of the JumpCenterpoint class.
*/

#include "JumpCenterpoint.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Jump.h"

namespace orxonox
{
    RegisterClass(JumpCenterpoint);

    /**
    @brief
        Constructor. Registers and initializes the object and checks whether the gametype is actually Jump.
    */
    JumpCenterpoint::JumpCenterpoint(Context* context) : StaticEntity(context)
    {
        RegisterObject(JumpCenterpoint);

        // Variablen hier veraendern nuetzt nichts! Diese Variablen koennen in Level-File initialisiert werden.
        width_ = 200;
        height_ = 120;
        sectionLength_ = 120;

        this->checkGametype();
    }

    /**
    @brief
        Method to create a JumpCenterpoint through XML.
    */
    void JumpCenterpoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpCenterpoint, XMLPort, xmlelement, mode);

        XMLPortParam(JumpCenterpoint, "dimension", setFieldDimension, getFieldDimension, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "sectionLength", setSectionLength, getSectionLength, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformLength", setPlatformLength, getPlatformLength, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformStaticTemplate", setPlatformStaticTemplate, getPlatformStaticTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformHMoveTemplate", setPlatformHMoveTemplate, getPlatformHMoveTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformVMoveTemplate", setPlatformVMoveTemplate, getPlatformVMoveTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformDisappearTemplate", setPlatformDisappearTemplate, getPlatformDisappearTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformTimerTemplate", setPlatformTimerTemplate, getPlatformTimerTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformFakeTemplate", setPlatformFakeTemplate, getPlatformFakeTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "figureTemplate", setFigureTemplate, getFigureTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "projectileTemplate", setProjectileTemplate, getProjectileTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "enemy1Template", setEnemy1Template, getEnemy1Template, xmlelement, mode);
    }

    /**
    @brief
        Is called when the gametype has changed.
    */
    void JumpCenterpoint::changedGametype()
    {
        SUPER(JumpCenterpoint, changedGametype);

        // Check, whether it's still Jump.
        this->checkGametype();
    }

    /**
    @brief
        Checks whether the gametype is Jump and if it is, sets its centerpoint.
    */
    void JumpCenterpoint::checkGametype()
    {
        if (this->getGametype() != NULL && this->getGametype()->isA(Class(Jump)))
        {
            Jump* jumpGametype = orxonox_cast<Jump*>(this->getGametype().get());
            jumpGametype->setCenterpoint(this);
        }
    }
}
