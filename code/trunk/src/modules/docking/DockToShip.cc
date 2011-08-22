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
 *      Sven Stucki
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
        orxout(verbose, context::docking) << "DockToShip instance created.." << endl;
    }

    DockToShip::~DockToShip()
    {

    }


    void DockToShip::XMLPort(Element& xmlelement, XMLPort::Mode mode) {
        SUPER(DockToShip, XMLPort, xmlelement, mode);

        XMLPortParam(DockToShip, "target", setTargetId, getTargetId, xmlelement, mode);
    }

    void DockToShip::setTargetId(const std::string& str) {
        this->target_ = str;
    }

    const std::string& DockToShip::getTargetId() const {
        return this->target_;
    }


    bool DockToShip::docking(PlayerInfo* player)
    {
        orxout(verbose, context::docking) << "DockToShip::attach" << endl;

        DockingTarget *target = DockingEffect::findTarget(this->target_);
        if (target == NULL) {
            orxout(internal_warning, context::docking) << "Can't retrieve target for '" << this->target_ << "'.." << endl;
            return false;
        }

        ControllableEntity *dockTo = (ControllableEntity*) target->getParent();
        if (dockTo == NULL) {
            orxout(internal_warning, context::docking) << "Parent is not a ControllableEntity.." << endl;
            return false;
        }

        // Make sure target isn't removed when undocking
        dockTo->setDestroyWhenPlayerLeft(false);
        player->startTemporaryControl(dockTo);

        return true;
    }

    bool DockToShip::release(PlayerInfo* player)
    {
        orxout(verbose, context::docking) << "DockToShip::release" << endl;

        player->stopTemporaryControl();

        return true;
    }
}

