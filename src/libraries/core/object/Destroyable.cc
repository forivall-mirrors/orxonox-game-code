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
    @brief Implementation of Destroyable.
*/

#include "Destroyable.h"
#include "DestroyLaterManager.h"

#include <cassert>

namespace orxonox
{
    /**
        @brief Constructor: Sets the default values.
    */
    Destroyable::Destroyable()
    {
        this->referenceCount_ = 0;
        this->requestedDestruction_ = false;
    }

    /**
        @brief Destructor: Notifies all DestructionListener (for example @ref WeakPtr "weak pointers") that this object is being deleted.
    */
    Destroyable::~Destroyable()
    {
        assert(this->referenceCount_ <= 0);

        // notify all destruction listeners
        for (std::set<DestructionListener*>::iterator it = this->destructionListeners_.begin(); it != this->destructionListeners_.end(); )
            (*(it++))->objectDeleted();
    }

    /**
        @brief Deletes the object if no @ref orxonox::SmartPtr "smart pointers" point to this object. Otherwise schedules the object to be deleted as soon as possible.
        Always call destroy() instead of using 'delete' directly, otherwise smart pointers won't work.
    */
    void Destroyable::destroy()
    {
        assert(this); // Just in case someone tries to delete a NULL pointer
        this->requestedDestruction_ = true;
        if (this->referenceCount_ == 0)
        {
            this->preDestroy();
            if (this->referenceCount_ == 0)
                delete this;
        }
    }

    /**
     * Works like @ref destroy() but doesn't destroy the object until the current tick has ended.
     */
    void Destroyable::destroyLater()
    {
        // register in DestroyLaterManager - this ensures that a smartPtr points to this object and keeps it alive for a while
        DestroyLaterManager::getInstance().retain(this);

        // request destruction -> object will be deleted after all smartPtrs (including the one in DestroyLaterManager) were destroyed.
        this->destroy();
    }
}
