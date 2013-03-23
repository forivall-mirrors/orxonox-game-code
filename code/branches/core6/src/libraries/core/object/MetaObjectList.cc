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
    @brief Implementation of the MetaObjectList class.
*/

#include "MetaObjectList.h"

#include "util/Output.h"
#include "core/Identifier.h"
#include "ObjectListBase.h"

namespace orxonox
{
    // ###############################
    // ###  MetaObjectListElement  ###
    // ###############################
    /**
        @brief Destructor: Removes the ObjectListBaseElement from the ObjectListBase by linking next_ and prev_ of the ObjectListBaseElement.
    */
    MetaObjectListElement::~MetaObjectListElement()
    {
        orxout(verbose, context::object_list) << "Removing Object from " << this->list_->getIdentifier()->getName() << "-list." << endl;
        this->list_->notifyIterators(this->element_->objectBase_);

        if (this->element_->next_)
            this->element_->next_->prev_ = this->element_->prev_;
        else
            this->list_->last_ = this->element_->prev_; // If there is no next_, we deleted the last object and have to update the last_ pointer of the list

        if (this->element_->prev_)
            this->element_->prev_->next_ = this->element_->next_;
        else
            this->list_->first_ = this->element_->next_; // If there is no prev_, we deleted the first object and have to update the first_ pointer of the list

        delete this->element_;
    }


    // ###############################
    // ###     MetaObjectList      ###
    // ###############################
    /**
        @brief Constructor: Sets first_ to zero.
    */
    MetaObjectList::MetaObjectList()
    {
        this->first_ = 0;
    }

    /**
        @brief Destructor: Removes all elements from the list, causing them to remove the stored ObjectListElement from the ObjectList.
    */
    MetaObjectList::~MetaObjectList()
    {
        MetaObjectListElement* temp;
        while (this->first_)
        {
            temp = this->first_->next_;
            delete this->first_;
            this->first_ = temp;
        }
    }

    /**
        @brief Adds an ObjectList and an element of that list to the MetaObjectList.
        @param list The ObjectList wherein the element is
        @param element The element wherein the object is
    */
    void MetaObjectList::add(ObjectListBase* list, ObjectListBaseElement* element)
    {
        MetaObjectListElement* temp = this->first_;
        this->first_ = new MetaObjectListElement(list, element);
        this->first_->next_ = temp;
    }
}
