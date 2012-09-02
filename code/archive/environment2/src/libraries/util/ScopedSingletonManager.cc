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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Static linkage of the ScopedSingletonManager maps.
*/

#include "ScopedSingletonManager.h"

namespace orxonox
{
    /*static*/ std::map<std::string, ScopedSingletonManager*>& ScopedSingletonManager::getManagers()
    {
        static std::map<std::string, ScopedSingletonManager*> managers;
        return managers;
    }
    /*static*/ ScopedSingletonManager::ManagerMultiMap& ScopedSingletonManager::getManagersByScope()
    {
        static ManagerMultiMap managers;
        return managers;
    }

    /*static*/ void ScopedSingletonManager::addManager(ScopedSingletonManager* manager)
    {
        getManagers()[manager->className_] = manager;
        getManagersByScope().insert(std::make_pair(manager->scope_, manager));
    }
}
