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
    @brief Definition of macros for Identifier and Factory.

    Every class needs the RegisterObject(class) macro in its constructor. If the class is an interface
    or the BaseObject itself, it needs the macro RegisterRootObject(class) instead.

    To allow the object being created through the factory, use the CreateFactory(class) macro outside
    the of the class implementation, so it gets executed before main().
*/

#ifndef _CoreIncludes_H__
#define _CoreIncludes_H__

#include "CorePrereqs.h"

#include "util/Debug.h"
#include "Identifier.h"
#include "Factory.h"
#include "ClassFactory.h"
#include "ObjectList.h"


/**
    @brief Intern macro, containing the common parts of RegisterObject and RegisterRootObject.
    @param ClassName The name of the class
    @param bRootClass True if the class is directly derived from OrxonoxClass
*/
#define InternRegisterObject(ClassName, bRootClass) \
    if (ClassIdentifier<ClassName>::getIdentifier(#ClassName)->initialiseObject(this, #ClassName, bRootClass)) \
        return; \
    else \
        ((void)0)

/**
    @brief RegisterObject - with and without debug output.
    @param ClassName The name of the class
*/
#define RegisterObject(ClassName) \
    InternRegisterObject(ClassName, false)

/**
    @brief RegisterRootObject - with and without debug output.
    @param ClassName The name of the class
*/
#define RegisterRootObject(ClassName) \
    InternRegisterObject(ClassName, true)

/**
    @brief Creates the entry in the Factory.
    @param ClassName The name of the class
*/
#define CreateFactory(ClassName) \
    bool bCreated##ClassName##Factory = orxonox::ClassFactory<ClassName>::create(#ClassName, true)

/**
    @brief Creates the entry in the Factory for classes which should not be loaded through XML.
    @param ClassName The name of the class
*/
#define CreateUnloadableFactory(ClassName) \
    bool bCreated##ClassName##Factory = orxonox::ClassFactory<ClassName>::create(#ClassName, false)

/**
    @brief Returns the Identifier of the given class.
    @param ClassName The name of the class
*/
#define Class(ClassName) \
    orxonox::ClassIdentifier<ClassName>::getIdentifier()


namespace orxonox
{
    /**
        @brief Returns the Identifier with a given name through the factory.
        @param String The name of the class
    */
    inline Identifier* ClassByString(const std::string& name)
    {
        return Factory::getIdentifier(name);
    }

    /**
        @brief Returns the Identifier with a given network ID through the factory.
        @param networkID The network ID of the class
    */
    inline Identifier* ClassByID(uint32_t id)
    {
        return Factory::getIdentifier(id);
    }
}

#endif /* _CoreIncludes_H__ */
