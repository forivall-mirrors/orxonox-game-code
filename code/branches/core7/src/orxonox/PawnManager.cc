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

#include "PawnManager.h"

#include "util/ScopedSingletonManager.h"
#include "core/CoreIncludes.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    ManageScopedSingleton(PawnManager, ScopeID::Root, false);

    RegisterAbstractClass(PawnManager).inheritsFrom<Tickable>();

    PawnManager::PawnManager()
    {
        RegisterObject(PawnManager);
    }

    PawnManager::~PawnManager()
    {
        // Be sure to delete all the pawns
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); )
            (it++)->destroy();
    }

    void PawnManager::preUpdate(const Clock& time)
    {
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); )
        {
            if (!it->isAlive())
                (it++)->destroy();
            else
                ++it;
        }
    }
}
