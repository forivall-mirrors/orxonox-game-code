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

#ifndef __Util_Scope_H__
#define __Util_Scope_H__

#include "UtilPrereqs.h"

#include <cassert>
#include <map>
#include <set>
#include "Debug.h"

namespace orxonox
{
    /**
        @brief The ScopeManager stores the variables of the scope templates in a statically linked context.
    */
    class _UtilExport ScopeManager
    {
        template <ScopeID::Value scope>
        friend class Scope;
        friend class ScopeListener;

        private:
            static std::map<ScopeID::Value, int> instanceCounts_s;                  //!< Counts the number of active instances (>0 means active) for a scope
            static std::map<ScopeID::Value, std::set<ScopeListener*> > listeners_s; //!< Stores all listeners for a scope
    };

    /**
        @brief ScopeListeners register themselves in the corresponding scope and wait for notifications.
    */
    class _UtilExport ScopeListener
    {
        template <ScopeID::Value scope>
        friend class Scope;

        protected:
            //! Constructor: Registers the instance.
            ScopeListener(ScopeID::Value scope) : scope_(scope)
                { ScopeManager::listeners_s[this->scope_].insert(this); }
            //! Destructor: Unregisters the instance.
            virtual ~ScopeListener()
                { ScopeManager::listeners_s[this->scope_].erase(this); }

            //! Gets called if the scope is activated
            virtual void activated() = 0;
            //! Gets called if the scope is deactivated
            virtual void deactivated() = 0;
            //! Gets called if the scope is updated
            virtual void updated(const Clock& time) = 0;

        private:
            ScopeID::Value scope_; //!< Store the scope to unregister on destruction
    };

    /**
        @brief A scope for a given template argument is either active or not.

        Objects inheriting from a ScopeListener are registered in a list (different for each scope).
        If the scope gets activated or deactivated, all objects in this list are notified.
    */
    template <ScopeID::Value scope>
    class Scope
    {
        public:
            //! Constructor: Increases the instance counter and activates the scope if the count went from 0 to 1. Counts >1 don't change anything.
            Scope()
            {
                ScopeManager::instanceCounts_s[scope]++;
                assert(ScopeManager::instanceCounts_s[scope] > 0);
                if (ScopeManager::instanceCounts_s[scope] == 1)
                {
                    for (typename std::set<ScopeListener*>::iterator it = ScopeManager::listeners_s[scope].begin(); it != ScopeManager::listeners_s[scope].end(); )
                        (*(it++))->activated();
                }
            }

            //! Destructor: Decreases the instance counter and deactivates the scope if the count went from 1 to 0. Counts >0 don't change anything.
            ~Scope()
            {
                ScopeManager::instanceCounts_s[scope]--;

                // This shouldn't happen but just to be sure: check if the count is positive
                assert(ScopeManager::instanceCounts_s[scope] >= 0);
                if (ScopeManager::instanceCounts_s[scope] < 0)
                    ScopeManager::instanceCounts_s[scope] = 0;

                if (ScopeManager::instanceCounts_s[scope] == 0)
                {
                    for (typename std::set<ScopeListener*>::iterator it = ScopeManager::listeners_s[scope].begin(); it != ScopeManager::listeners_s[scope].end(); )
                        (*(it++))->deactivated();
                }
            }

            //! Returns true if the scope is active.
            static bool isActive()
            {
                return (ScopeManager::instanceCounts_s[scope] > 0);
            }

            //! Update method for the ScopeListeners (to implement singleton updates)
            static void update(const Clock& time)
            {
                if (isActive())
                {
                    for (typename std::set<ScopeListener*>::iterator it = ScopeManager::listeners_s[scope].begin(); it != ScopeManager::listeners_s[scope].end(); )
                        (*(it++))->updated(time);
                }
            }
    };
}

#endif /* __Util_Scope_H__ */
