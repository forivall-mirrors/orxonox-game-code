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

To avoid multiple instances of orxonox::Scope<@a scope> in different libraries, each instance of orxonox::Scope
registers in orxonox::ScopeManager, where they are linked statically in the core library.

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
        template <ScopeID::Value scope>
        friend class Scope;

        protected:
            ScopeListener(ScopeID::Value scope) : scope_(scope), bActivated_(false) { }
            virtual ~ScopeListener() { }

            //! Gets called if the scope is activated
            virtual void activated() = 0;
            //! Gets called if the scope is deactivated
            virtual void deactivated() = 0;

        public:
            inline ScopeID::Value getScope() const
                { return this->scope_; }

        private:
            ScopeID::Value scope_; //!< Store the scope to unregister on destruction
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
            //! Constructor: Increases the instance counter and activates the scope if the count went from 0 to 1. Counts >1 don't change anything.
            Scope()
            {
                orxout(internal_status) << "creating scope... (" << scope << ")" << endl;

                try
                {
                    ScopeManager::getInstanceCounts()[scope]++;
                    assert(ScopeManager::getInstanceCounts()[scope] > 0);
                    if (ScopeManager::getInstanceCounts()[scope] == 1)
                    {
                        Loki::ScopeGuard deactivator = Loki::MakeObjGuard(*this, &Scope::deactivateListeners);
                        for (typename std::set<ScopeListener*>::iterator it = ScopeManager::getListeners()[scope].begin(); it != ScopeManager::getListeners()[scope].end(); )
                        {
                            (*it)->activated();
                            (*(it++))->bActivated_ = true;
                        }
                        deactivator.Dismiss();
                    }
                }
                catch (...)
                {
                    ScopeManager::getInstanceCounts()[scope]--;
                    throw;
                }

                orxout(internal_status) << "created scope (" << scope << ")" << endl;
            }

            //! Destructor: Decreases the instance counter and deactivates the scope if the count went from 1 to 0. Counts >0 don't change anything.
            ~Scope()
            {
                orxout(internal_status) << "destroying scope... (" << scope << ")" << endl;

                ScopeManager::getInstanceCounts()[scope]--;

                // This shouldn't happen but just to be sure: check if the count is positive
                assert(ScopeManager::getInstanceCounts()[scope] >= 0);
                if (ScopeManager::getInstanceCounts()[scope] < 0)
                    ScopeManager::getInstanceCounts()[scope] = 0;

                if (ScopeManager::getInstanceCounts()[scope] == 0)
                    this->deactivateListeners();

                orxout(internal_status) << "destroyed scope (" << scope << ")" << endl;
            }

            //! Deactivates the listeners of this scope in case the scope is destroyed or the construction fails.
            void deactivateListeners()
            {
                for (typename std::set<ScopeListener*>::iterator it = ScopeManager::getListeners()[scope].begin(); it != ScopeManager::getListeners()[scope].end(); )
                {
                    if ((*it)->bActivated_)
                    {
                        try
                            { (*it)->deactivated(); }
                        catch (...)
                            { orxout(internal_warning) << "ScopeListener::deactivated() failed! This MUST NOT happen, fix it!" << endl; }
                        (*(it++))->bActivated_ = false;
                    }
                    else
                        ++it;
                }
            }

            //! Returns true if the scope is active.
            static bool isActive()
            {
                return (ScopeManager::getInstanceCounts()[scope] > 0);
            }
    };
}

#endif /* __Core_Scope_H__ */
