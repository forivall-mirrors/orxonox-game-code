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
    @brief Implementation of Listable.
*/

#include "Listable.h"
#include "core/CoreIncludes.h"
#include "ObjectListBase.h"
#include "Context.h"

namespace orxonox
{
    RegisterClass(Listable);

    /**
        @brief Constructor: Allocates space in the element list.
    */
    Listable::Listable()
    {
        this->context_ = Context::getRootContext();
        this->elements_.reserve(6);

        RegisterObject(Listable);
    }

    /**
        @brief Constructor: Allocates space in the element list and assigns the context
    */
    Listable::Listable(Context* context)
    {
        this->context_ = context;
        this->elements_.reserve(6);

        RegisterObject(Listable);
    }

    /**
        @brief Destructor: Removes the object from the object-lists
    */
    Listable::~Listable()
    {
        this->unregisterObject();
    }

    /**
        @brief Removes this object from the object-lists.
    */
    void Listable::unregisterObject()
    {
        for (size_t i = 0; i < this->elements_.size(); ++i)
            delete this->elements_[i];
        this->elements_.clear();
    }

    /**
     * @brief Changes the context.
     * The object is removed from the current context and added to the new one. This also applies to all object lists the object is registered in.
     */
    void Listable::setContext(Context* context)
    {
        assert(context);
        std::vector<ObjectListBaseElement*> copy = this->elements_;
        this->elements_.clear();

        for (size_t i = 0; i < copy.size(); ++i)
        {
            copy[i]->changeContext(this->context_, context);
            delete copy[i];
        }

        this->context_ = context;
    }

}
