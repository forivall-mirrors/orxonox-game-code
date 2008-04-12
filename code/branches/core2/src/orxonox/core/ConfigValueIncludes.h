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
    @file ConfigValueIncludes.h
    @brief Definition of macros for config-values.
*/

#ifndef _ConfigValueIncludes_H__
#define _ConfigValueIncludes_H__

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
        COUT(2) << "Warning: Couldn't reset variable " << #varname << ", corresponding container doesn't exist." << std::endl; \
    }

/**
    @brief Assigns the command, defined in the keybind-file, to the key-variable (or an empty string, if there is no entry in the file).
    @param varname The name of the key-variable
*/
#define SetKeybind(keyname) \
    orxonox::ConfigValueContainer* container##keyname = this->getIdentifier()->getConfigValueContainer(#keyname); \
    if (!container##keyname) \
    { \
        container##keyname = new orxonox::ConfigValueContainer(CFT_Keybindings, this->getIdentifier(), #keyname, keyname = ""); \
        this->getIdentifier()->addConfigValueContainer(#keyname, container##keyname); \
    } \
    container##keyname->getValue(&varname)

#endif /* _ConfigValueIncludes_H__ */
