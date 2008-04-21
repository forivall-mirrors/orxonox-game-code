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
    @file ClassManager.h
    @brief Definition and Implementation of the ClassManager template.

    The ClassManager is a helper-class for ClassIdentifier. Because ClassIdentifiers must
    be unique, they are created through the IdentifierDistributor-class to assure the
    uniqueness of the ClassIdentifier. But accessing Identifiers through IdentifierDistributor
    is slow, because it uses strings and a map. Thats why we use the ClassManager-template: It's
    a singleton like ClassIdentifier, but it doesn't hurt if there are multiple instances in
    different libraries, because they all store the same pointer to the unique ClassIdentifier
    which they've retrieved through IdentifierDistributor.
*/

#ifndef _ClassManager_H__
#define _ClassManager_H__

#include "CorePrereqs.h"

#include <string>

#include "Identifier.h"
#include "IdentifierDistributor.h"

namespace orxonox
{
    //! ClassManager is a helper class to allow faster access on the ClassIdentifiers.
    /**
        Because accessing the IdentifierDistributor is slow, the ClassManager accesses it once
        and stores the result in a member-variable. IdentifierDistributor assures the uniqueness
        of the ClassIdentifier, even if there are multiple instances of the ClassManager-template
        in different libraries.
    */
    template <class T>
    class ClassManager
    {
        public:
            static ClassManager<T>* getSingleton();
            static ClassIdentifier<T>* getIdentifier();
            static const std::string& getName();

        private:
            ClassManager();
            ClassManager(const ClassIdentifier<T>& identifier) {}    // don't copy
            ~ClassManager() {}                                       // don't delete

            bool bInitialized_;                 //!< This is false until the ClassIdentifier gets assigned
            ClassIdentifier<T>* identifier_;    //!< The unique ClassIdentifier for the class T
    };

    /**
        @brief Constructor: Marks the ClassManager as uninitialized.
    */
    template <class T>
    ClassManager<T>::ClassManager()
    {
        this->bInitialized_ = false;
    }

    /**
        @brief Returns the one and only instance of this class for the template parameter T.
        @return The instance
    */
    template <class T>
    ClassManager<T>* ClassManager<T>::getSingleton()
    {
        static ClassManager<T> theOneAndOnlyInstance = ClassManager<T>();
        return &theOneAndOnlyInstance;
    }

    /**
        @brief Creates the only instance of this class for the template class T and retrieves a unique Identifier for the given name.
        @return The unique Identifier
    */
    template <class T>
    ClassIdentifier<T>* ClassManager<T>::getIdentifier()
    {
        // Check if the ClassManager is already initialized
        if (!ClassManager<T>::getSingleton()->bInitialized_)
        {
            // Get the name of the class
            std::string name = typeid(T).name();

            // It's not -> retrieve the ClassIdentifier through IdentifierDistributor
            COUT(4) << "*** ClassManager: Request Identifier Singleton for " << name << "." << std::endl;

            // First create a ClassIdentifier in case there's no instance existing yet
            ClassIdentifier<T>* temp = new ClassIdentifier<T>();

            // Ask the IdentifierDistributor for the unique ClassIdentifier
            ClassManager<T>::getSingleton()->identifier_ = (ClassIdentifier<T>*)IdentifierDistributor::getIdentifier(name, temp);

            // If the retrieved Identifier differs from our proposal, we don't need the proposal any more
            if (temp != ClassManager<T>::getSingleton()->identifier_)
            {
                COUT(4) << "*** ClassManager: Requested Identifier for " << name << " was already existing and got assigned." << std::endl;

                // Delete the unnecessary proposal
                delete temp;
            }
            else
            {
                COUT(4) << "*** ClassManager: Requested Identifier for " << name << " was not yet existing and got created." << std::endl;
            }

            ClassManager<T>::getSingleton()->bInitialized_ = true;
        }

        // Finally return the unique ClassIdentifier
        return ClassManager<T>::getSingleton()->identifier_;
    }

    /**
        @brief Returns the name of the class the ClassManager belongs to.
        @return The name
    */
    template <class T>
    const std::string& ClassManager<T>::getName()
    {
        static std::string unknownClassName = std::string("unknown");

        if (ClassManager<T>::getSingleton()->bInitialized_)
            return ClassManager<T>::getSingleton()->identifier_->getName();
        else
            return unknownClassName;
    }
}

#endif /* _ClassManager_H__ */
