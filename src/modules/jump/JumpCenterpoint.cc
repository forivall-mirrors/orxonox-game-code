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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file JumpCenterpoint.cc
    @brief The JumpCenterpoint is a StaticEntity which represents the level of the minigame. All platforms, enemies and items are attached to the JumpCenterpoint.
*/

#include "JumpCenterpoint.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "Jump.h"

namespace orxonox
{
    RegisterClass(JumpCenterpoint);

    JumpCenterpoint::JumpCenterpoint(Context* context) : StaticEntity(context)
    {
        RegisterObject(JumpCenterpoint);

        width_ = 0.0;
        height_ = 0.0;
        sectionLength_ = 0.0;
        platformSpeed_ = 0.0;

        checkGametype();
    }

    void JumpCenterpoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpCenterpoint, XMLPort, xmlelement, mode);

        XMLPortParam(JumpCenterpoint, "dimension", setFieldDimension, getFieldDimension, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "sectionLength", setSectionLength, getSectionLength, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformSpeed", setPlatformSpeed, getPlatformSpeed, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "cameraOffset", setCameraOffset, getCameraOffset, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformStaticTemplate", setPlatformStaticTemplate, getPlatformStaticTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformHMoveTemplate", setPlatformHMoveTemplate, getPlatformHMoveTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformVMoveTemplate", setPlatformVMoveTemplate, getPlatformVMoveTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformDisappearTemplate", setPlatformDisappearTemplate, getPlatformDisappearTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformTimerTemplate", setPlatformTimerTemplate, getPlatformTimerTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "platformFakeTemplate", setPlatformFakeTemplate, getPlatformFakeTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "figureTemplate", setFigureTemplate, getFigureTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "projectileTemplate", setProjectileTemplate, getProjectileTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "springTemplate", setSpringTemplate, getSpringTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "rocketTemplate", setRocketTemplate, getRocketTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "propellerTemplate", setPropellerTemplate, getPropellerTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "bootsTemplate", setBootsTemplate, getBootsTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "shieldTemplate", setShieldTemplate, getShieldTemplate, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "enemy1Template", setEnemy1Template, getEnemy1Template, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "enemy2Template", setEnemy2Template, getEnemy2Template, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "enemy3Template", setEnemy3Template, getEnemy3Template, xmlelement, mode);
        XMLPortParam(JumpCenterpoint, "enemy4Template", setEnemy4Template, getEnemy4Template, xmlelement, mode);
    }

    void JumpCenterpoint::changedGametype()
    {
        SUPER(JumpCenterpoint, changedGametype);

        checkGametype();
    }

    void JumpCenterpoint::checkGametype()
    {
        if (getGametype() != NULL && this->getGametype()->isA(Class(Jump)))
        {
            Jump* jumpGametype = orxonox_cast<Jump*>(this->getGametype().get());
            jumpGametype->setCenterpoint(this);
        }
    }
}
