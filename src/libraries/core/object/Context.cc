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
    @brief Implementation of Context.
*/

#include "Context.h"
#include "core/class/Identifier.h"

namespace orxonox
{
    Context::Context(Context* context) : Listable(this), parentContext_(context)
    {
        // we have to call Listable(this) to avoid circular initialization when creating a Context because Listable calls Context::getRootContext() by
        // default AND we have to set the context again in the constructor because of other classes inheriting from Context (Listable is a virtual base
        // and each subclass must call its constructor individually, so either all subclasses add Listable(this) to their initialization list or we call
        // setContext(this) here).
        this->setContext(this);
    }

    Context::~Context()
    {
        // unregister context from object lists before object lists are destroyed
        this->unregisterObject();
        for (size_t i = 0; i < this->objectLists_.size(); ++i)
            delete this->objectLists_[i];
    }

    /*static*/ Context* Context::getRootContext()
    {
        static Context rootContext(NULL);
        return &rootContext;
    }

    ObjectListBase* Context::getObjectList(const Identifier* identifier)
    {
        unsigned int classID = identifier->getClassID();
        if (classID >= this->objectLists_.size())
            this->objectLists_.resize(classID + 1);
        if (!this->objectLists_[classID])
            this->objectLists_[classID] = new ObjectListBase();
        return this->objectLists_[classID];
    }
}
