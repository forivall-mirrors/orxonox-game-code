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
    @brief Definition of SubclassIdentifier.

    SubclassIdentifier is a separated class, acting like an Identifier, but has a given class.
    You can only assign Identifiers of exactly the given class or of a derivative to a SubclassIdentifier.
*/

#ifndef _SubclassIdentifier_H__
#define _SubclassIdentifier_H__

#include "CorePrereqs.h"

#include <cstdlib>
#include "util/Debug.h"
#include "Identifier.h"

namespace orxonox
{
    // ###############################
    // ###   SubclassIdentifier    ###
    // ###############################
    //! The SubclassIdentifier acts almost like an Identifier, but has some prerequisites.
    /**
        You can only assign an Identifier that belongs to a class T (or derived) to a SubclassIdentifier<T>.
        If you assign something else, the program aborts.
        Because we know the minimum type, a dynamic_cast is done, which makes it easier to create a new object.
    */
    template <class T>
    class SubclassIdentifier
    {
        public:
            /**
                @brief Constructor: Automaticaly assigns the Identifier of the given class.
            */
            SubclassIdentifier()
            {
                this->identifier_ = ClassIdentifier<T>::getIdentifier();
            }

            /**
                @brief Constructor: Assigns the given Identifier.
                @param identifier The Identifier
            */
            SubclassIdentifier(Identifier* identifier)
            {
                this->operator=(identifier);
            }

            /**
                @brief Copyconstructor: Assigns the identifier of the other SubclassIdentifier.
                @param identifier The other SublcassIdentifier
            */
            template <class O>
            SubclassIdentifier(const SubclassIdentifier<O>& identifier)
            {
                this->operator=(identifier.getIdentifier());
            }

            /**
                @brief Overloading of the = operator: assigns the identifier and checks its type.
                @param identifier The Identifier to assign
                @return The SubclassIdentifier itself
            */
            const SubclassIdentifier<T>& operator=(Identifier* identifier)
            {
                if (!identifier || !identifier->isA(ClassIdentifier<T>::getIdentifier()))
                {
                    COUT(1) << "An error occurred in SubclassIdentifier (Identifier.h):" << std::endl;
                    if (identifier)
                    {
                        COUT(1) << "Error: Class " << identifier->getName() << " is not a " << ClassIdentifier<T>::getIdentifier()->getName() << '!' << std::endl;
                        COUT(1) << "Error: SubclassIdentifier<" << ClassIdentifier<T>::getIdentifier()->getName() << "> = Class(" << identifier->getName() << ") is forbidden." << std::endl;
                    }
                    else
                    {
                        COUT(1) << "Error: Can't assign NULL identifier" << std::endl;
                    }
                }
                else
                {
                    this->identifier_ = identifier;
                }
                return *this;
            }

            /**
                @brief Overloading of the = operator: assigns the identifier of the other SubclassIdentifier.
                @param identifier The other SublcassIdentifier
            */
            template <class O>
            const SubclassIdentifier<T>& operator=(const SubclassIdentifier<O>& identifier)
            {
                return this->operator=(identifier.getIdentifier());
            }

            /**
                @brief Overloading of the * operator: returns the assigned identifier.
            */
            inline Identifier* operator*() const
            {
                return this->identifier_;
            }

            /**
                @brief Overloading of the -> operator: returns the assigned identifier.
            */
            inline Identifier* operator->() const
            {
                return this->identifier_;
            }

            /**
                @brief Returns the assigned identifier. This allows you to assign a SubclassIdentifier to a normal Identifier*.
            */
            inline operator Identifier*() const
            {
                return this->identifier_;
            }

            /**
                @brief Creates a new object of the type of the assigned Identifier and dynamic_casts it to the minimal type given by T.
                @return The new object
            */
            T* fabricate(BaseObject* creator) const
            {
                BaseObject* newObject = this->identifier_->fabricate(creator);

                // Check if the creation was successful
                if (newObject)
                {
                    return orxonox_cast<T*>(newObject);
                }
                else
                {
                    // Something went terribly wrong
                    if (this->identifier_)
                    {
                        COUT(1) << "An error occurred in SubclassIdentifier (Identifier.h):" << std::endl;
                        COUT(1) << "Error: Class " << this->identifier_->getName() << " is not a " << ClassIdentifier<T>::getIdentifier()->getName() << '!' << std::endl;
                        COUT(1) << "Error: Couldn't fabricate a new Object." << std::endl;
                    }
                    else
                    {
                        COUT(1) << "An error occurred in SubclassIdentifier (Identifier.h):" << std::endl;
                        COUT(1) << "Error: Couldn't fabricate a new Object - Identifier is undefined." << std::endl;
                    }

                    COUT(1) << "Aborting..." << std::endl;
                    abort();
                    return 0;
                }
            }

            /** @brief Returns the assigned identifier. @return The identifier */
            inline Identifier* getIdentifier() const
                { return this->identifier_; }

        private:
            Identifier* identifier_;            //!< The assigned identifier
    };
}

#endif /* _SubclassIdentifier_H__ */
