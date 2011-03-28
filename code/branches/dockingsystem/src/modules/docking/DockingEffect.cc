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
#include "core/CoreIncludes.h"

namespace orxonox
{
    DockingEffect::DockingEffect(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(DockingEffect);
    }

    DockingEffect::~DockingEffect()
    {

    }

    bool DockingEffect::invokeEffects(PlayerInfo* player, std::list<DockingEffect*> & effects)
    {
        bool check = true;

        COUT(4) << "Invoking DockingEffects on player: " << player << " ."  << std::endl;

        for (std::list<DockingEffect*>::iterator effect = effects.begin(); effect != effects.end(); effect++)
            check = check	;// && (*effect)->invoke(player);  TODO

        return check;
    }
}
