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
    @ingroup Class Identifier
    @brief Definition of SubclassIdentifier.

    @anchor SubclassIdentifierExample

    SubclassIdentifier is a separated class, acting like an Identifier, but has a given class.
    You can only assign Identifiers of exactly the given class or of a derivative to a SubclassIdentifier.

    Example:

    You can assign an Identifier either through the constructor or by using the assignment @c operator=:
    @code
    SubclassIdentifier<BaseClass> identifier = Class(SubClass);
    @endcode

    The @c operator-> is overloaded an returns the assigned Identifier. That way you can just call
    functions of the assigned Identifier by using @c ->function():
    @code
    SubclassIdentifier<BaseClass> identifier = Class(SubClass);
    identifier->getName();      // returns "SubClass"
    @endcode

    There are two possibilities to create an object out of a SubclassIdentifier: Either you just use
    the @c fabricate() function of the assigned Identifier through the overloaded @c operator->, which
    returns a @c BaseObject* pointer, or you use the function of SubclassIdentifier, this time by using
    @c operator., which returns a @c BaseClass* pointer (@a BaseClass is the baseclass specified by the
    template argument):
    @code
    identifier->fabricate();    // calls Identifier::fabricate(), creates a SubClass, returns a BaseObject* pointer

    identifier.fabricate();     // calls SubclassIdentifier::fabricate(), creates a SubClass, returns a BaseClass* pointer
    @endcode
*/

#ifndef _SubclassIdentifier_H__
#define _SubclassIdentifier_H__

#include "CorePrereqs.h"

#include <cstdlib>
#include "util/Output.h"
#include "Identifier.h"

namespace orxonox
{
    // ###############################
    // ###   SubclassIdentifier    ###
    // ###############################
    /**
        @brief The SubclassIdentifier acts almost like an Identifier, but has some prerequisites.

        You can only assign an Identifier that belongs to a class T (or derived) to a SubclassIdentifier<T>.
        If you assign something else, the program prints an error.

        Because we know the base-type, a @c dynamic_cast is done, which makes it easier to create a new object.

        @see See @ref SubclassIdentifierExample "SubclassIdentifier.h" for some examples.
    */
    template <class T>
    class SubclassIdentifier
    {
        public:
            /// Constructor: Automaticaly assigns the Identifier of the given class.
            SubclassIdentifier()
            {
                this->identifier_ = ClassIdentifier<T>::getIdentifier();
            }

            /// Constructor: Assigns the given Identifier.
            SubclassIdentifier(Identifier* identifier)
            {
                this->operator=(identifier);
            }

            /// Copyconstructor: Assigns the identifier of another SubclassIdentifier.
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
            SubclassIdentifier<T>& operator=(Identifier* identifier)
            {
                if (!identifier || !identifier->isA(ClassIdentifier<T>::getIdentifier()))
                {
                    orxout(internal_error) << "An error occurred in SubclassIdentifier (Identifier.h):" << endl;
                    if (identifier)
                    {
                        orxout(internal_error) << "Class " << identifier->getName() << " is not a " << ClassIdentifier<T>::getIdentifier()->getName() << '!' << endl;
                        orxout(internal_error) << "SubclassIdentifier<" << ClassIdentifier<T>::getIdentifier()->getName() << "> = Class(" << identifier->getName() << ") is forbidden." << endl;
                    }
                    else
                    {
                        orxout(internal_error) << "Can't assign NULL identifier" << endl;
                    }
                }
                else
                {
                    this->identifier_ = identifier;
                }
                return *this;
            }

            /// Overloading of the = operator: assigns the identifier of another SubclassIdentifier.
            template <class O>
            SubclassIdentifier<T>& operator=(const SubclassIdentifier<O>& identifier)
            {
                return this->operator=(identifier.getIdentifier());
            }

            /// Overloading of the * operator: returns the assigned identifier.
            inline Identifier* operator*() const
            {
                return this->identifier_;
            }

            /// Overloading of the -> operator: returns the assigned identifier.
            inline Identifier* operator->() const
            {
                return this->identifier_;
            }

            /// Returns the assigned identifier. This allows you to assign a SubclassIdentifier to a normal Identifier*.
            inline operator Identifier*() const
            {
                return this->identifier_;
            }

            /// Creates a new object of the type of the assigned Identifier and dynamic_casts it to the minimal type given by T.
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
                        orxout(user_error) << "An error occurred in SubclassIdentifier (Identifier.h):" << endl;
                        orxout(user_error) << "Class " << this->identifier_->getName() << " is not a " << ClassIdentifier<T>::getIdentifier()->getName() << '!' << endl;
                        orxout(user_error) << "Couldn't fabricate a new Object." << endl;
                    }
                    else
                    {
                        orxout(user_error) << "An error occurred in SubclassIdentifier (Identifier.h):" << endl;
                        orxout(user_error) << "Couldn't fabricate a new Object - Identifier is undefined." << endl;
                    }

                    orxout(user_error) << "Aborting..." << endl;
                    abort();
                    return 0;
                }
            }

            /// Returns the assigned identifier.
            inline Identifier* getIdentifier() const
                { return this->identifier_; }

        private:
            Identifier* identifier_;            //!< The assigned identifier
    };
}

#endif /* _SubclassIdentifier_H__ */
