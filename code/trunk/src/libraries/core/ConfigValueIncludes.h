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
 *      Reto Grieder (functions)
 *
 */

/**
@file
@brief
    Definition of macros and functions for config-values.
*/

#ifndef _ConfigValueIncludes_H__
#define _ConfigValueIncludes_H__

#include "CorePrereqs.h"

#include "Identifier.h"
#include "ConfigValueContainer.h"

namespace orxonox
{
    /** Sets a runtime configurable value.
        If the container for the value doesn't yet exist, a new one is created.
        Also, the @a variable argument will be modified and set to the new value (default or from ini file).
    @param object
        Class instance that the config value should belong to (usually just 'this')
    @param variable
        Pointer to the variable where the value should be written to
    @param type
        Type of the config file, usually ConfigFileType::Settings
    @param sectionName
        Name of the section in the ini file (e.g. [MySection])
    @param entryName
        Name of the entry in the ini file (e.g. [MySection] myValue)
    @param defaultValue
        Value to be used if it cannot be read from the ini file
    */
    template <class T, class D, class V>
    inline ConfigValueContainer& setConfigValueGeneric(T* object, V* variable, ConfigFileType::Value type, const std::string& sectionName, const std::string& entryName, const D& defaultValue)
    {
        ConfigValueContainer* container = ClassIdentifier<T>::getIdentifier()->getConfigValueContainer(entryName);
        if (!container)
        {
            container = new ConfigValueContainer(type, ClassIdentifier<T>::getIdentifier(), sectionName, entryName, defaultValue, *variable);
            ClassIdentifier<T>::getIdentifier()->addConfigValueContainer(entryName, container);
        }
        return container->getValue(variable, object);
    }
}

/** Sets a runtime configurable value (simplified macro version of setConfigValueGeneric)
    If the container for the value doesn't yet exist, a new one is created.
    Also, the @a varname argument will be modified and set to the new value (default or from ini file).
@param varname
    Variable name as C++ identifier. It will be used as entry name and as variable pointer
@param defaultValue
    Value to be used if it cannot be read from the ini file
*/
#define SetConfigValue(varname, defaultValue) \
    orxonox::setConfigValueGeneric(this, &varname, ConfigFileType::Settings, this->getIdentifier()->getName(), #varname, defaultValue)

/** Sets a runtime configurable value (simplified macro version of setConfigValueGeneric)
    If the container for the value doesn't yet exist, a new one is created.
    Also, the @a varname argument will be modified and set to the new value (default or from ini file).
@param variable
    Variable name as C++ identifier.
@param entryName
    Name of the entry in the ini file (e.g. [MySection] myValue)
@param defaultValue
    Value to be used if it cannot be read from the ini file
*/
#define SetConfigValueAlias(variable, entryName, defaultValue) \
    orxonox::setConfigValueGeneric(this, &variable, ConfigFileType::Settings, this->getIdentifier()->getName(), entryName, defaultValue)

/** Sets a runtime configurable value (simplified macro version of setConfigValueGeneric)
    If the container for the value doesn't yet exist, a new one is created.
    Also, the @a varname argument will be modified and set to the new value (default or from ini file).
@param variable
    Variable name as C++ identifier.
@param sectionName
    Name of the section in the ini file (e.g. [MySection])
@param entryName
    Name of the entry in the ini file (e.g. [MySection] myValue)
@param defaultValue
    Value to be used if it cannot be read from the ini file
*/
#define SetConfigValueExternal(variable, sectionName, entryName, defaultValue) \
    orxonox::setConfigValueGeneric(this, &variable, ConfigFileType::Settings, sectionName, entryName, defaultValue)


namespace orxonox
{
    /** Resets a runtime configurable value to its default.
        If the container for the value doesn't yet exist, a warning is displayed.
        Also, the @a variable argument will be modified and set to the default value.
    @param object
        Class instance that the config value should belong to (usually just 'this')
    @param variable
        Pointer to the variable where the value should be written to
    @param entryName
        Name of the entry in the ini file (e.g. [MySection] myValue)
    */
    template <class T, class V>
    inline void resetConfigValueGeneric(T* object, V* variable, const std::string& entryName)
    {
        ConfigValueContainer* container = ClassIdentifier<T>::getIdentifier()->getConfigValueContainer(entryName);
        if (container)
        {
            container->reset();
            container->getValue(variable, object);
        }
        else
        {
            COUT(2) << "Warning: Couldn't reset config-value '" << entryName << "' in class '"
                    << ClassIdentifier<T>::getIdentifier()->getName() << "', corresponding container doesn't exist." << std::endl;
        }
    }
}

/** Resets a runtime configurable value to its default (simplified macro version of modifyConfigValueGeneric)
    If the container for the value doesn't yet exist, a warning is displayed.
    Also, the @a varname argument will be modified and set to the default value.
@param varname
    Variable name as C++ identifier. It will be used as entry name and as variable pointer
*/
#define ResetConfigValue(varname) \
    orxonox::resetConfigValueGeneric(this, &varname, #varname)


/** Modifies a runtime configurable value by using a modifier and some arguments.
    If the container for the value doesn't yet exist, a warning is displayed.
    Also, the @a variable argument will be modified and set to the current value.
@param object
    Class instance that the config value should belong to (usually just 'this')
@param variable
    Pointer to the variable where the value should be written to
@param entryName
    Name of the entry in the ini file (e.g. [MySection] myValue)
@param modifier
    On of these functions: set, tset, add, remove, reset, update
@param ...
    Arguments for the modifier function
*/
#define ModifyConfigValueGeneric(object, variable, entryName, modifier, ...) \
    if (orxonox::ConfigValueContainer* container = ClassByObjectType(object)->getConfigValueContainer(entryName)) \
    { \
        container->modifier(__VA_ARGS__); \
        container->getValue(variable, object); \
    } \
    else \
    { \
        COUT(2) << "Warning: Couln't modify config-value '" << entryName << "' in class '" \
                << ClassByObjectType(object)->getName() << "', corresponding container doesn't exist." << std::endl; \
    }

/** Modifies a runtime configurable value by using a modifier and some arguments.
    If the container for the value doesn't yet exist, a warning is displayed.
    Also, the @a varname argument will be modified and set to the current value.
@param varname
    Variable name as C++ identifier. It will be used as entry name and as variable pointer
@param modifier
    On of these functions: set, tset, add, remove, reset, update
@param ...
    Arguments for the modifier function
*/
#define ModifyConfigValue(varname, modifier, ...) \
    ModifyConfigValueGeneric(this, &varname, #varname, modifier, __VA_ARGS__)

#endif /* _ConfigValueIncludes_H__ */
