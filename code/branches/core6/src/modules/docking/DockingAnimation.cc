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
    @file DockingAnimation.cc
    @brief Implementation of the DockingAnimation class.
*/

#include "DockingAnimation.h"

#include "DockingEffect.h"  // For DockingEffect::findTarget
#include "core/CoreIncludes.h"

namespace orxonox
{
    DockingAnimation::DockingAnimation(Context* context) : BaseObject(context)
    {
        RegisterObject(DockingAnimation);

        this->parent_ = NULL;
    }

    DockingAnimation::~DockingAnimation()
    {

    }

    bool DockingAnimation::invokeAnimation(bool dock, PlayerInfo* player, std::list<DockingAnimation*> &animations)
    {
        bool check = true;

        for (std::list<DockingAnimation*>::iterator animation = animations.begin(); animation != animations.end(); animation++)
        {
            if(dock)
                check &= (*animation)->docking(player);
            else
                check &= (*animation)->release(player);
        }

        return check;
    }

    DockingTarget *DockingAnimation::findTarget(std::string name) {
        return DockingEffect::findTarget(name);
    }
}

