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
    @defgroup ConfigFile Config file
    @ingroup Config
*/

/**
    @file
    @ingroup Config ConfigFile
    @brief Definition of macros and functions for config-values.

    An example of how to use SetConfigValue():

    Definition of a class in the header-file:
    @code
    class MyClass : public BaseObject
    {
        public:
            MyClass();              // Constructor
            void setConfigValues(); // Inherited function

            const std::string& getName()
                { return this->name_; }

            float getVersion()
                { return this->version_; }

        private:
            std::string name_;
            float version_;
    };
    @endcode

    Implementation of the class source-file:
    @code
    MyClass::MyClass()
    {
        // Macro-call to create an Identifier
        RegisterObject(MyClass);

        // Function-call to assign the config-values to the new object
        this->setConfigValues();
    }

    void MyClass::setConfigValues()
    {
        SetConfigValue(name_, "Orxonox").description("The name of the game");
        SetConfigValue(version_, "1.0").description("The version-number");
    }
    @endcode

    Extract of orxonox.ini:
    @code
    [MyClass]
    name_ = "Orxonox"
    version_ = 1.1 // We have changed this value from 1.0 to 1.1
    @endcode

    Some other code:
    @code
    MyObject orxonoxobject;
    orxout() << "Name:    " << orxonoxobject.getName() << endl;
    orxout() << "Version: " << orxonoxobject.getVersion() << endl;
    @endcode

    Output:
    @code
    Name:    Orxonox
    Version: 1.1
    @endcode
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
            orxout(user_warning, context::config) << "Couldn't reset config-value '" << entryName << "' in class '"
                                                  << ClassIdentifier<T>::getIdentifier()->getName() << "', corresponding container doesn't exist." << endl;
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
        orxout(user_warning, context::config) << "Couldn't modify config-value '" << entryName << "' in class '" \
                                              << ClassByObjectType(object)->getName() << "', corresponding container doesn't exist." << endl; \
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

/** Modifies a runtime configurable value by using a modifier and some arguments.
    If the container for the value doesn't yet exist, a warning is displayed.
    Also, the @a variable argument will be modified and set to the current value.
@param variable
    Pointer to the variable where the value should be written to
@param entryName
    Name of the entry in the ini file (e.g. [MySection] myValue)
@param modifier
    On of these functions: set, tset, add, remove, reset, update
@param ...
    Arguments for the modifier function
*/
#define ModifyConfigValueExternal(variable, entryName, modifier, ...) \
    ModifyConfigValueGeneric(this, &variable, entryName, modifier, __VA_ARGS__)

#endif /* _ConfigValueIncludes_H__ */
