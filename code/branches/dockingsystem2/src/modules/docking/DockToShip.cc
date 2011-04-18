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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

/**
    @file DockToShip.cc
    @brief Implementation of the DockToShip class.
*/

#include "DockingEffect.h"
#include "DockToShip.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    CreateFactory(DockToShip);

    DockToShip::DockToShip(BaseObject* creator) : DockingEffect(creator)
    {
        RegisterObject(DockToShip);
        COUT(0) << "DockToShip instance created.." << endl;
    }

    DockToShip::~DockToShip()
    {

    }


    void DockToShip::XMLPort(Element& xmlelement, XMLPort::Mode mode) {
        SUPER(DockToShip, XMLPort, xmlelement, mode);

        XMLPortParam(DockToShip, "target", setTargetId, getTargetId, xmlelement, mode);
    }

    void DockToShip::setTargetId(std::string str) {
        this->target = str;
    }

    std::string DockToShip::getTargetId() {
        return this->target;
    }


    bool DockToShip::docking(PlayerInfo* player)
    {
        COUT(0) << "DockToShip::docking" << endl;
        return true;
    }

    bool DockToShip::attach(PlayerInfo* player)
    {
        COUT(0) << "DockToShip::attach" << endl;

        DockingTarget *target = DockingEffect::findTarget(this->target);
        if (target == NULL) {
            COUT(0) << "Can't retrieve target for '" << this->target << "'.." << std::endl;
            return false;
        }

        ControllableEntity *dockTo = (ControllableEntity*) target->getParent();
        if (dockTo == NULL) {
            COUT(0) << "Parent is not a ControllableEntity.." << std::endl;
            return false;
        }

        player->startTemporaryControl(dockTo);

        return true;
    }

    bool DockToShip::release(PlayerInfo* player)
    {
        COUT(0) << "DockToShip::release" << endl;
        return true;
    }
}
