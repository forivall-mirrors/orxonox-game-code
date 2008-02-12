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

#ifndef _ClassManager_H__
#define _ClassManager_H__

#include <string>

#include "Identifier.h"
#include "IdentifierDistributor.h"

#include "CorePrereqs.h"

namespace orxonox
{
    template <class T>
    class ClassManager
    {
        public:
            static ClassManager<T>* getSingleton();
            static ClassIdentifier<T>* getIdentifier(const std::string& name);
            static const std::string& getName();

        private:
            ClassManager();
            ClassManager(const ClassIdentifier<T>& identifier) {}    // don't copy
            ~ClassManager() {}                                       // don't delete

            bool bInitialized_;
//            std::string name_;
            ClassIdentifier<T>* identifier_;
    };

    /**
        @brief Constructor: Marks the ClassManager as uninitialized.
    */
    template <class T>
    ClassManager<T>::ClassManager()
    {
        this->bInitialized_ = false;
    }

    template <class T>
    ClassManager<T>* ClassManager<T>::getSingleton()
    {
        static ClassManager<T> theOneAndOnlyInstance = ClassManager<T>();
        return &theOneAndOnlyInstance;
    }

    /**
        @brief Creates the only instance of this class for the template class T and retrieves a unique Identifier.
        @return The unique Identifier
    */
    template <class T>
    ClassIdentifier<T>* ClassManager<T>::getIdentifier(const std::string& name)
    {
        if (!ClassManager<T>::getSingleton()->bInitialized_)
        {

            COUT(4) << "*** Create Identifier Singleton." << std::endl;
            ClassIdentifier<T>* temp = new ClassIdentifier<T>();
            ClassManager<T>::getSingleton()->identifier_ = (ClassIdentifier<T>*)IdentifierDistributor::getIdentifier(name, temp);
            if (temp != ClassManager<T>::getSingleton()->identifier_)
                delete temp;
            ClassManager<T>::getSingleton()->bInitialized_ = true;
        }

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

        if (ClassManager<T>::getSingleton()->bInitialized)
            return ClassManager<T>::getSingleton()->identifier_->getName();
        else
            return unknownClassName;
    }
}

#endif /* _ClassManager_H__ */
