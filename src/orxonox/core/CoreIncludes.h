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

/**
    @file CoreIncludes.h
    @brief Definition of macros for Identifier and Factory.

    Every class needs the RegisterObject(class) macro in its constructor. If the class is an interface
    or the BaseObject itself, it needs the macro RegisterRootObject(class) instead.

    To allow the object being created through the factory, use the CreateFactory(class) macro outside
    the of the class implementation, so it gets executed before main().
*/

#ifndef _CoreIncludes_H__
#define _CoreIncludes_H__

#include "Identifier.h"
#include "ClassManager.h"
#include "Factory.h"
#include "ClassFactory.h"
#include "Debug.h"


/**
    @brief Intern macro, containing the common parts of RegisterObject and RegisterRootObject.
    @param ClassName The name of the class
    @param bRootClass True if the class is directly derived from OrxonoxClass
*/
#define InternRegisterObject(ClassName, bRootClass) \
    this->setIdentifier(orxonox::ClassManager<ClassName>::getIdentifier()->registerClass(this->getParents(), #ClassName, bRootClass)); \
    if (orxonox::Identifier::isCreatingHierarchy() && this->getParents()) \
        this->getParents()->insert(this->getParents()->end(), this->getIdentifier()); \
    orxonox::ClassManager<ClassName>::getIdentifier()->addObject(this)

/**
    @brief Intern macro, containing the specific part of RegisterRootObject.
    @param ClassName The name of the class
*/
#define InternRegisterRootObject(ClassName) \
    if (orxonox::Identifier::isCreatingHierarchy() && !this->getParents()) \
        this->createParents(); \
    InternRegisterObject(ClassName, true)

/**
    @brief RegisterObject - with and without debug output.
    @param ClassName The name of the class
*/
#define RegisterObject(ClassName) \
    COUT(5) << "*** Register Object: " << #ClassName << std::endl; \
    InternRegisterObject(ClassName, false)

/**
    @brief RegisterRootObject - with and without debug output.
    @param ClassName The name of the class
*/
#define RegisterRootObject(ClassName) \
    COUT(5) << "*** Register Root-Object: " << #ClassName << std::endl; \
    InternRegisterRootObject(ClassName)

/**
    @brief Returns the Identifier of the given class.
    @param ClassName The name of the class
*/
#define Class(ClassName) \
    ClassManager<ClassName>::getIdentifier()

/**
    @brief Creates the entry in the Factory.
    @param ClassName The name of the class
*/
#define CreateFactory(ClassName) \
    bool bCreated##ClassName##Factory = orxonox::ClassFactory<ClassName>::create(#ClassName)

/**
    @brief Returns the Identifier with either a given name or a given network ID through the factory.
    @param StringOrInt The name or the network ID of the class
*/
#define ID(StringOrInt) \
    orxonox::Factory::getIdentifier(StringOrInt)

#endif /* _CoreIncludes_H__ */
