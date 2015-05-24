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

/**
    @file
    @brief Static linkage of the two maps in orxonox::ScopeManager.
*/

#include "ScopeManager.h"

#include "Scope.h"

namespace orxonox
{
    /* static */ ScopeManager& ScopeManager::getInstance()
    {
        static ScopeManager instance;
        return instance;
    }

    void ScopeManager::addScope(ScopeID::Value scope)
    {
        this->activeScopes_.insert(scope);
    }

    void ScopeManager::removeScope(ScopeID::Value scope)
    {
        this->activeScopes_.erase(scope);
    }

    bool ScopeManager::isActive(ScopeID::Value scope)
    {
        return this->activeScopes_.find(scope) != this->activeScopes_.end();
    }

    void ScopeManager::addListener(ScopeListener* listener)
    {
        this->listeners_[listener->getScope()].insert(listener);
    }

    void ScopeManager::removeListener(ScopeListener* listener)
    {
        this->listeners_[listener->getScope()].erase(listener);
    }
}
