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
@ingroup SingletonScope
@brief Declaration of orxonox::ScopeManager.
*/

#ifndef __ScopeManager_H__
#define __ScopeManager_H__

#include "core/CorePrereqs.h"

#include <map>
#include <set>

namespace orxonox
{
    /**
        @brief The ScopeManager stores the variables of the Scope templates in a statically linked context.

        If all Scope objects are managed by this class, they are statically linked in the core library.
        Without this, a new instance of Scope<T> for each T would be created in every library of Orxonox,
        which is of course not the desired behavior.

        @see See @ref Scope "this description" for details about the interrelationship of Scope, ScopeListener, and ScopeManager.
    */
    class _CoreExport ScopeManager
    {
        template <ScopeID::Value scope>
        friend class Scope;
        friend class StaticallyInitializedScopedSingletonWrapper;

        private:
            static std::map<ScopeID::Value, int>& getInstanceCounts();                  //!< Counts the number of active instances (>0 means active) for a scope
            static std::map<ScopeID::Value, std::set<ScopeListener*> >& getListeners(); //!< Stores all listeners for a scope
    };
}

#endif /* __ScopeManager_H__ */
