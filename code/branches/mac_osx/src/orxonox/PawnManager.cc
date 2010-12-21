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

#include "core/CoreIncludes.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    PawnManager* PawnManager::singletonPtr_s = 0;

    PawnManager::PawnManager()
    {
        RegisterRootObject(PawnManager);
    }

    PawnManager::~PawnManager()
    {
    }

    void PawnManager::touch()
    {
        if (!PawnManager::singletonPtr_s)
            new PawnManager();
    }

    void PawnManager::tick(float dt)
    {
        unsigned int count = 0;
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++count)
        {
            if (!it->isAlive())
                (it++)->destroy();
            else
                ++it;
        }

        if (count == 0)
            this->destroy();
    }
}
