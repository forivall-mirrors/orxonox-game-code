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
    @file DockingEffect.cc
    @brief Implementation of the DockingEffect class.
*/

#include "DockingEffect.h"

namespace orxonox
{
    DockingEffect::DockingEffect(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(DockingEffect);
    }

    DockingEffect::~DockingEffect()
    {

    }

    bool DockingEffect::invokeEffect(docking::event event, PlayerInfo* player, std::list<DockingEffect*> & effects)
    {
        bool check = true;

        COUT(4) << "Invoking DockingEffects on player: " << player << " ."  << std::endl;

        for (std::list<DockingEffect*>::iterator effect = effects.begin(); effect != effects.end(); effect++)
        {
            switch(event)
            {
                case docking::DOCKING:
                    check &= (*effect)->docking(player);
                    break;
                case docking::RELEASE:
                    check &= (*effect)->release(player);
                    break;
                default:
                    assert(0);
            }
        }

        return check;
    }

    DockingTarget *DockingEffect::findTarget(std::string name) {
        for (ObjectList<DockingTarget>::iterator it = ObjectList<DockingTarget>::begin();
                                 it != ObjectList<DockingTarget>::end(); ++it)
        {
            if ((*it)->getName().compare(name) == 0)
                return (*it);
        }
        return NULL;
    }
}
