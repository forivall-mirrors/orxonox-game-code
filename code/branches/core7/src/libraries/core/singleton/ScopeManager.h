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
        public:
            static ScopeManager& getInstance();

            /** Adds a scope and activates all listeners which are registered for this scope */
            void addScope(ScopeID::Value scope);
            /** Removes a scope and deactivates all listeners which are registered for this scope */
            void removeScope(ScopeID::Value scope);
            /** Returns true if this scope is active */
            bool isActive(ScopeID::Value scope);

            /** Registers a listener for the given scope. */
            void addListener(ScopeListener* listener, ScopeID::Value scope);
            /** Unregisters a listener for the given scope. */
            void removeListener(ScopeListener* listener, ScopeID::Value scope);

        private:
            void activateListenersForScope(ScopeID::Value scope);
            void deactivateListenersForScope(ScopeID::Value scope);

            void activateListener(ScopeListener* listener);
            void deactivateListener(ScopeListener* listener);

            std::set<ScopeID::Value> activeScopes_;
            std::map<ScopeID::Value, std::set<ScopeListener*> > listeners_; //!< Stores all listeners for a scope
    };
}

#endif /* __ScopeManager_H__ */
