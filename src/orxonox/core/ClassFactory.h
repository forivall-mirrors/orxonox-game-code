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
    @file ClassFactory.h
    @brief Definition and implementation of the ClassFactory class

    The ClassFactory is able to create new objects of a specific class.
*/

#ifndef _ClassFactory_H__
#define _ClassFactory_H__

#include <string>

#include "CorePrereqs.h"

#include "Factory.h"
#include "Identifier.h"
#include "Debug.h"

namespace orxonox
{
    // ###############################
    // ###      ClassFactory       ###
    // ###############################
    //! The ClassFactory is able to create new objects of a specific class.
    template <class T>
    class ClassFactory : public BaseFactory
    {
        public:
            static bool create(const std::string& name);
            BaseObject* fabricate();

        private:
            ClassFactory() {}                               // Don't create
            ClassFactory(const ClassFactory& factory) {}    // Don't copy
            virtual ~ClassFactory() {}                      // Don't delete

            static T* createNewObject();
    };

    /**
        @brief Adds the ClassFactory to the Identifier of the same type and the Identifier to the Factory.
        @return Always true (this is needed because the compiler only allows assignments before main())
    */
    template <class T>
    bool ClassFactory<T>::create(const std::string& name)
    {
        COUT(4) << "*** Create entry for " << name << " in Factory." << std::endl;
        ClassIdentifier<T>::getIdentifier()->addFactory(new ClassFactory<T>);
        Factory::add(name, ClassIdentifier<T>::getIdentifier());

        return true;
    }

    /**
        @brief Creates and returns a new object of class T.
        @return The new object
    */
    template <class T>
    BaseObject* ClassFactory<T>::fabricate()
    {
        return ClassFactory<T>::createNewObject();
    }

    /**
        @brief Creates and returns a new object of class T; this is a wrapper for the new operator.
        @return The new object
    */
    template <class T>
    T* ClassFactory<T>::createNewObject()
    {
        return new T;
    }
}

#endif /* _ClassFactory_H__ */
