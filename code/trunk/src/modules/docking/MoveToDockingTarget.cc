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
    @file MoveToDockingTarget.h
    @brief Definition of the MoveToDockingTarget class.
    @ingroup Docking
*/

#include "MoveToDockingTarget.h"

#include "core/XMLPort.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    RegisterClass(MoveToDockingTarget);

    MoveToDockingTarget::MoveToDockingTarget(Context* context) : DockingAnimation(context)
    {
        RegisterObject(MoveToDockingTarget);
    }

    MoveToDockingTarget::~MoveToDockingTarget() {

    }

    bool MoveToDockingTarget::docking(PlayerInfo *player)
    {
        assert(this->parent_);

        DockingController *dockingController = new DockingController(this->getContext());
        dockingController->setDock(this->parent_);
        dockingController->setPlayer(player);
        dockingController->takeControl(true);

        return true;
    }

    bool MoveToDockingTarget::release(PlayerInfo *player)
    {
        //TODO: Investigate strange things...
        //this->parent_->detach((WorldEntity*)player->getControllableEntity());

        //TODO: Check the issue with this detach call.
        //I have removed the line because the detach call only caused a warning and terminated. And because I didn't find a attach call either.
        //Didn't find the need for the line.
        this->parent_->undockingAnimationFinished(player);
        return true;
    }

}

