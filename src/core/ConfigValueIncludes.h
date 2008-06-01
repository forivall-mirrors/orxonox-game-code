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
#define SetConfigValue(varname, defvalue) \
    orxonox::ConfigValueContainer* container##varname = this->getIdentifier()->getConfigValueContainer(#varname); \
    if (!container##varname) \
    { \
        container##varname = new orxonox::ConfigValueContainer(CFT_Settings, this->getIdentifier(), #varname, varname = defvalue); \
        this->getIdentifier()->addConfigValueContainer(#varname, container##varname); \
    } \
    container##varname->getValue(&varname)

/**
    @brief Assigns the value, defined in the config-file, to the variable (or the default-value, if there is no entry in the file).
    @param classname name in which the config value should be stored
    @param varname The name of the variable
    @param defvalue The default-value of the variable
*/
#define SetConfigValueGeneric(classname, varname, defvalue) \
    orxonox::ConfigValueContainer* container##varname = ClassManager<classname>::getIdentifier()->getConfigValueContainer(#varname); \
    if (!container##varname) \
    { \
        container##varname = new orxonox::ConfigValueContainer(CFT_Settings, ClassManager<classname>::getIdentifier(), #varname, varname = defvalue); \
        ClassManager<classname>::getIdentifier()->addConfigValueContainer(#varname, container##varname); \
    } \
    container##varname->getValue(&varname)

/**
    @brief Assigns the vector-values, defined in the config-file, to the vector (or the default-value, if there are no entries in the file).
    @param varname The name of the std::vector
    @param defvalue The default-value
*/
#define SetConfigValueVector(varname, defvalue) \
    orxonox::ConfigValueContainer* container##varname = this->getIdentifier()->getConfigValueContainer(#varname); \
    if (!container##varname) \
    { \
        std::vector<MultiTypeMath> temp; \
        for (unsigned int i = 0; i < defvalue.size(); i++) \
            temp.push_back(MultiTypeMath(defvalue[i])); \
        container##varname = new orxonox::ConfigValueContainer(CFT_Settings, this->getIdentifier(), #varname, temp); \
        container##varname->setVectorType(varname); \
        this->getIdentifier()->addConfigValueContainer(#varname, container##varname); \
    } \
    container##varname->getValue(&varname)

/**
    @brief Sets the variable and the config-file entry back to the previously defined default-value.
    @param varname The name of the variable
*/
#define ResetConfigValue(varname) \
    orxonox::ConfigValueContainer* container##varname##reset = this->getIdentifier()->getConfigValueContainer(#varname); \
    if (container##varname##reset) \
    { \
        container##varname##reset->reset(); \
        container##varname##reset->getValue(&varname); \
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
        container##varname##modify##modifier->getValue(&varname); \
    } \
    else \
    { \
        COUT(2) << "Warning: Couln't modify config-value '" << #varname << "', corresponding container doesn't exist." << std::endl; \
    }

#endif /* _ConfigValueIncludes_H__ */
