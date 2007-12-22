/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

/*!
    @file Identifier.cc
    @brief Implementation of the Identifier class.
*/

#include "Identifier.h"

namespace orxonox
{
    // ###############################
    // ###       Identifier        ###
    // ###############################
    int Identifier::hierarchyCreatingCounter_s = 0; // Set the static member variable hierarchyCreatingCounter_s to zero
    unsigned int Identifier::classIDcounter_s = 0;  // Set the static member variable classIDcounter_s to zero

    /**
        @brief Constructor: No factory, no object created, new ObjectList and a unique networkID.
    */
    Identifier::Identifier()
    {
        this->bCreatedOneObject_ = false;
        this->factory_ = 0;

        this->children_ = new IdentifierList;
        this->classID_ = Identifier::classIDcounter_s++;
    }

    /**
        @brief Destructor: Deletes the name and the IdentifierList containing the children.
    */
    Identifier::~Identifier()
    {
        delete &this->name_;
        delete this->children_;
    }

    /**
        @brief Initializes the Identifier with an IdentifierList containing all parents of the class the Identifier belongs to.
        @param parents The IdentifierList containing all parents
    */
    void Identifier::initialize(const IdentifierList* parents)
    {
        COUT(4) << "*** Initialize " << this->name_ << "-Singleton.\n";
        this->bCreatedOneObject_ = true;

        if (parents)
        {
            IdentifierListElement* temp1 = parents->first_;
            while (temp1)
            {
                this->parents_.add(temp1->identifier_);
                temp1->identifier_->getChildren().add(this); // We're a child of our parents

                temp1 = temp1->next_;
            }
        }
    }

    /**
        @brief Creates an object of the type the Identifier belongs to.
        @return The new object
    */
    BaseObject* Identifier::fabricate()
    {
        if (this->factory_)
        {
            return this->factory_->fabricate(); // We have to return a BaseObject, because we don't know the exact type.
        }
        else
        {
            // Abstract classes don't have a factory and therefore can't create new objects
            COUT(1) << "Error: Cannot create an object of type '" << this->name_ << "'. Class is abstract.\n";
            COUT(1) << "Aborting...";
            abort();
            return NULL;
        }
    }

    /**
        @brief Sets the network ID to a new value and changes the entry in the Factory.
        @param id The new network ID
    */
    void Identifier::setNetworkID(unsigned int id)
    {
        Factory::changeNetworkID(this, this->classID_, id);
        this->classID_ = id;
    }

    /**
        @returns true, if the Identifier is at least of the given type.
        @param identifier The identifier to compare with
    */
    bool Identifier::isA(const Identifier* identifier) const
    {
        return (identifier == this || this->parents_.isInList(identifier));
    }

    /**
        @returns true, if the Identifier is exactly of the given type.
        @param identifier The identifier to compare with
    */
    bool Identifier::isDirectlyA(const Identifier* identifier) const
    {
        return (identifier == this);
    }

    /**
        @returns true, if the assigned identifier is a child of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isChildOf(const Identifier* identifier) const
    {
        return this->parents_.isInList(identifier);
    }

    /**
        @returns true, if the assigned identifier is a parent of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isParentOf(const Identifier* identifier) const
    {
        return this->children_->isInList(identifier);
    }
}
