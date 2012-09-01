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
    @defgroup Factory RegisterObject() and CreateFactory()
    @ingroup Object
*/

/**
    @file
    @ingroup Object Factory Class Identifier
    @brief Defines several very important macros used to register objects, create factories, and to work with identifiers.

    Every class needs the @c RegisterObject(class) macro in its constructor. If the class is an interface
    or the @c BaseObject itself, it needs the macro @c RegisterRootObject(class) instead.

    To allow the object being created through the factory, use the @c CreateFactory(class) macro outside
    of the class implementation, so it gets executed statically before @c main(). This will at the same time
    register @a class in the class-hierarchy. If you don't want @a class to be loadable, but still
    register it, call @c CreateUnloadableFactory(class).

    Example:
    @code
    // Create the factory for MyClass
    CreateFactory(MyClass);

    // Constructor:
    MyClass::MyClass()
    {
        // Register the object in the Identifier of MyClass
        RegisterObject(MyClass);
    }
    @endcode

    This file also defines a number of other useful macros, like, for example, @c Class(class) which
    returns the @ref orxonox::Identifier "Identifier" of @a class, or @c ClassByString("class") which
    returns the Identifier of a class with name @a "class".

    Example:
    @code
    // Assigns the Identifier of MyClass
    Identifier* identifier = Class(MyClass);
    @endcode
    @code
    // Assigns the Identifier of a class named "MyClass"
    Identifier* identifier = ClassByString("MyClass");
    @endcode
*/

#ifndef _CoreIncludes_H__
#define _CoreIncludes_H__

#include "CorePrereqs.h"

#include "util/Output.h"
#include "Identifier.h"
#include "ClassFactory.h"
#include "ObjectList.h"


/**
    @brief Intern macro, containing the common parts of @c RegisterObject and @c RegisterRootObject.
    @param ClassName The name of the class
    @param bRootClass True if the class is directly derived from orxonox::OrxonoxClass
*/
#define InternRegisterObject(ClassName, bRootClass) \
    if (ClassIdentifier<ClassName>::getIdentifier(#ClassName)->initialiseObject(this, #ClassName, bRootClass)) \
        return; \
    else \
        ((void)0)

/**
    @brief Registers a newly created object in the core. Has to be called at the beginning of the constructor of @a ClassName.
    @param ClassName The name of the class
*/
#define RegisterObject(ClassName) \
    InternRegisterObject(ClassName, false)

/**
    @brief Registers a newly created object in the core. Has to be called at the beginning of the constructor of @a ClassName.
    @param ClassName The name of the class

    In contrast to RegisterObject, this is used for classes that inherit directly from
    orxonox::OrxonoxClass, namely all interfaces and orxonox::BaseObject.
*/
#define RegisterRootObject(ClassName) \
    InternRegisterObject(ClassName, true)

/**
    @brief Creates the Factory.
    @param ClassName The name of the class
*/
#define CreateFactory(ClassName) \
    Factory* _##ClassName##Factory = new orxonox::ClassFactory<ClassName>(#ClassName, true)

/**
    @brief Creates the Factory for classes which should not be loaded through XML.
    @param ClassName The name of the class
*/
#define CreateUnloadableFactory(ClassName) \
    Factory* _##ClassName##Factory = new orxonox::ClassFactory<ClassName>(#ClassName, false)

/**
    @brief Returns the Identifier of the given class.
    @param ClassName The name of the class
*/
#define Class(ClassName) \
    orxonox::ClassIdentifier<ClassName>::getIdentifier()


namespace orxonox
{
    /**
        @brief Returns the Identifier with a given name.
        @param name The name of the class
    */
    inline Identifier* ClassByString(const std::string& name)
    {
        return Identifier::getIdentifierByString(name);
    }

    /**
        @brief Returns the Identifier with a given lowercase name.
        @param name The lowercase name of the class
    */
    inline Identifier* ClassByLowercaseString(const std::string& name)
    {
        return Identifier::getIdentifierByLowercaseString(name);
    }

    /**
        @brief Returns the Identifier with a given network ID.
        @param id The network ID of the class
    */
    inline Identifier* ClassByID(uint32_t id)
    {
        return Identifier::getIdentifierByID(id);
    }

    /**
        @brief Returns the Identifier with a given 'this' pointer.
        @note This of course only works with OrxonoxClasses.
              The only use is in conjunction with macros that don't know the class type.
        @param object Pointer to an OrxonoxClass
    */
    template <class T>
    inline Identifier* ClassByObjectType(const T*)
    {
        return ClassIdentifier<T>::getIdentifier();
    }
}

#endif /* _CoreIncludes_H__ */
