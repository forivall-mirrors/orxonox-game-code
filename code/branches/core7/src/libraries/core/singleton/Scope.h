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
@brief Declaration of the classes that are needed to use Scopes: orxonox::Scope and orxonox::ScopeListener.

@anchor Scope

A virtual scope can be represented by an instance of class orxonox::Scope. orxonox::Scope<@a scope> is a template
an its template argument defines the name of the virtual scope. See orxonox::ScopeID for an enumeration of the
available values for @a scope. The orxonox::Scope object for a given @a scope can be activated or deactivated.
Instances of orxonox::ScopeListener can register in orxonox::ScopeMAnager for a given @a scope and will get a
notification if the corresponding orxonox::Scope object changes its state.

Scopes are usually used to control the creation and destruction of Singletons.

@see orxonox::Singleton
*/

#ifndef __Core_Scope_H__
#define __Core_Scope_H__

#include "core/CorePrereqs.h"

#include <cassert>
#include <map>
#include <set>
#include <loki/ScopeGuard.h>

#include "util/Output.h"
#include "ScopeManager.h"

namespace orxonox
{
    /**
        @brief ScopeListeners register themselves in the corresponding Scope and wait for notifications.
        Notifications are sent if a Scope is activated or deactivated.

        @see See @ref Scope "this description" for details about the interrelationship of Scope, ScopeListener, and ScopeManager.
    */
    class _CoreExport ScopeListener
    {
        friend class ScopeManager;

        protected:
            ScopeListener() : bActivated_(false) { }
            virtual ~ScopeListener() { }

            //! Gets called if the scope is activated
            virtual void activated() = 0;
            //! Gets called if the scope is deactivated
            virtual void deactivated() = 0;

        private:
            bool bActivated_;
    };

    /**
        @brief A scope for a given template argument is either active or not.

        Objects inheriting from a ScopeListener are registered in a list (different for each scope).
        If the scope gets activated or deactivated, all objects in this list are notified.

        @see See @ref Scope "this description" for details about the interrelationship of Scope, ScopeListener, and ScopeManager.
    */
    template <ScopeID::Value scope>
    class Scope
    {
        public:
            //! Constructor: activate the listeners.
            Scope()
            {
                orxout(internal_status) << "creating scope... (" << scope << ")" << endl;

                Loki::ScopeGuard deactivator = Loki::MakeObjGuard(*this, &Scope::deactivateScope);
                ScopeManager::getInstance().addScope(scope);
                deactivator.Dismiss();

                orxout(internal_status) << "created scope (" << scope << ")" << endl;
            }

            //! Destructor: deactivate the listeners.
            ~Scope()
            {
                orxout(internal_status) << "destroying scope... (" << scope << ")" << endl;

                this->deactivateScope();

                orxout(internal_status) << "destroyed scope (" << scope << ")" << endl;
            }

            //! Deactivates the listeners of this scope in case the scope is destroyed or the construction fails.
            void deactivateScope()
            {
                ScopeManager::getInstance().removeScope(scope);
            }

            //! Returns true if the scope is active.
            static bool isActive()
            {
                return ScopeManager::getInstance().isActive(scope);
            }
    };
}

#endif /* __Core_Scope_H__ */
