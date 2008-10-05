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
    @file ConfigValueIncludes.h
    @brief Definition of macros for config-values.
*/

#ifndef _ConfigValueIncludes_H__
#define _ConfigValueIncludes_H__

#include "CorePrereqs.h"

#include "Identifier.h"
#include "ConfigValueContainer.h"
#include "ConfigFileManager.h"


/**
    @brief Assigns the value, defined in the config-file, to the variable (or the default-value, if there is no entry in the file).
    @param varname The name of the variable
    @param defvalue The default-value of the variable
*/
#define SetConfigValueGeneric(type, varname, defvalue) \
    static orxonox::Identifier* identifier##varname = this->getIdentifier(); \
    orxonox::ConfigValueContainer* container##varname = identifier##varname->getConfigValueContainer(#varname); \
    if (!container##varname) \
    { \
        container##varname = new orxonox::ConfigValueContainer(CFT_Settings, identifier##varname, identifier##varname->getName(), #varname, defvalue, varname); \
        identifier##varname->addConfigValueContainer(#varname, container##varname); \
    } \
    container##varname->getValue(&varname, this)

#define SetConfigValue(varname, defvalue) SetConfigValueGeneric(CFT_Settings, varname, defvalue)
#define SetKeybindingValue(varname, defvalue) SetConfigValueGeneric(CFT_Keybindings, varname, defvalue)


/**
    @brief Assigns the vector-values, defined in the config-file, to the vector (or the default-value, if there are no entries in the file).
    @param varname The name of the std::vector
    @param defvalue The default-value
*/
#define SetConfigValueVectorGeneric(type, varname, defvalue) \
    static orxonox::Identifier* identifier##varname = this->getIdentifier(); \
    orxonox::ConfigValueContainer* container##varname = identifier##varname->getConfigValueContainer(#varname); \
    if (!container##varname) \
    { \
        container##varname = new orxonox::ConfigValueContainer(CFT_Settings, identifier##varname, identifier##varname->getName(), #varname, defvalue); \
        identifier##varname->addConfigValueContainer(#varname, container##varname); \
    } \
    container##varname->getValue(&varname, this)

#define SetConfigValueVector(varname, defvalue) SetConfigValueVectorGeneric(CFT_Settings, varname, defvalue)
#define SetKeybindingValueVector(varname, defvalue) SetConfigValueVectorGeneric(CFT_Keybindings, varname, defvalue)


/**
    @brief Sets the variable and the config-file entry back to the previously defined default-value.
    @param varname The name of the variable
*/
#define ResetConfigValue(varname) \
    orxonox::ConfigValueContainer* container##varname##reset = this->getIdentifier()->getConfigValueContainer(#varname); \
    if (container##varname##reset) \
    { \
        container##varname##reset->reset(); \
        container##varname##reset->getValue(&varname, this); \
    } \
    else \
    { \
        COUT(2) << "Warning: Couldn't reset config-value '" << #varname << "', corresponding container doesn't exist." << std::endl; \
    }


/**
    @brief Modifies a config-value by using a modifier and some arguments.
    @param varname The name of the config-value
    @param modifier The name of the modifier: set, tset, add, remove, reset, update
*/
#define ModifyConfigValue(varname, modifier, ...) \
    orxonox::ConfigValueContainer* container##varname##modify##modifier = this->getIdentifier()->getConfigValueContainer(#varname); \
    if (container##varname##modify##modifier) \
    { \
        container##varname##modify##modifier->modifier(__VA_ARGS__); \
        container##varname##modify##modifier->getValue(&varname, this); \
    } \
    else \
    { \
        COUT(2) << "Warning: Couln't modify config-value '" << #varname << "', corresponding container doesn't exist." << std::endl; \
    }

#endif /* _ConfigValueIncludes_H__ */
