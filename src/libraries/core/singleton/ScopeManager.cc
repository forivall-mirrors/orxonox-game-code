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
    @brief Implementation of orxonox::ScopeManager.
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
        this->activateListenersForScope(scope);
    }

    void ScopeManager::removeScope(ScopeID::Value scope)
    {
        this->activeScopes_.erase(scope);
        this->deactivateListenersForScope(scope);
    }

    bool ScopeManager::isActive(ScopeID::Value scope)
    {
        return this->activeScopes_.find(scope) != this->activeScopes_.end();
    }

    void ScopeManager::addListener(ScopeListener* listener, ScopeID::Value scope)
    {
        this->listeners_[scope].insert(listener);
    }

    void ScopeManager::removeListener(ScopeListener* listener, ScopeID::Value scope)
    {
        this->listeners_[scope].erase(listener);
    }

    void ScopeManager::activateListenersForScope(ScopeID::Value scope)
    {
        for (std::set<ScopeListener*>::iterator it = this->listeners_[scope].begin(); it != this->listeners_[scope].end(); ++it)
            this->activateListener(*it);
    }

    void ScopeManager::deactivateListenersForScope(ScopeID::Value scope)
    {
        for (std::set<ScopeListener*>::iterator it = this->listeners_[scope].begin(); it != this->listeners_[scope].end(); ++it)
            if ((*it)->bActivated_)
                this->deactivateListener(*it);
    }

    void ScopeManager::activateListener(ScopeListener* listener)
    {
        listener->activated();
        listener->bActivated_ = true;
    }

    void ScopeManager::deactivateListener(ScopeListener* listener)
    {
        try
            { listener->deactivated(); }
        catch (...)
            { orxout(internal_warning) << "ScopeListener::deactivated() failed! This MUST NOT happen, fix it!" << endl; }
        listener->bActivated_ = false;
    }

    void ScopeManager::updateListeners()
    {
        std::map<ScopeID::Value, std::set<ScopeListener*> >::iterator it1;
        for (it1 = this->listeners_.begin(); it1 != this->listeners_.end(); ++it1)
        {
            const ScopeID::Value& scope = it1->first;
            const std::set<ScopeListener*>& listeners = it1->second;

            bool scopeIsActive = this->isActive(scope);
            for (std::set<ScopeListener*>::const_iterator it2 = listeners.begin(); it2 != listeners.end(); ++it2)
            {
                ScopeListener* listener = (*it2);

                if (scopeIsActive && !listener->bActivated_)
                    this->activateListener(listener);
                else if (!scopeIsActive && listener->bActivated_)
                    this->deactivateListener(listener);
            }
        }
    }
}
