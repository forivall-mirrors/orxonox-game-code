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
    @file ConfigValueContainer.cc
    @brief Implementation of the ConfigValueContainer class.
*/

#include <fstream>

#include "ConfigValueContainer.h"
#include "Language.h"


namespace orxonox
{
    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(ConfigFileType type, Identifier* identifier, const std::string& varname, MultiTypeMath defvalue)
    {
        this->type_ = type;
        this->identifier_ = identifier;
        this->sectionname_ = identifier->getName();
        this->varname_ = varname;

        this->value_ = defvalue;
        this->bAddedDescription_ = false;

        this->defvalueString_ = defvalue.toString();
        this->update();
    }

    /**
        @brief Assigns a new value to the config-value of all objects and writes the change into the config-file.
        @param input The new value
        @return True if the new value was successfully assigned
    */
    bool ConfigValueContainer::set(const std::string& input)
    {
        bool success = this->tset(input);
        this->setLineInConfigFile(input);
        return success;
    }

    /**
        @brief Assigns a new value to the config-value of all objects, but doesn't change the config-file (t stands for temporary).
        @param input The new value
        @return True if the new value was successfully assigned
    */
    bool ConfigValueContainer::tset(const std::string& input)
    {
        bool success = this->parse(input);
        if (this->identifier_)
            this->identifier_->updateConfigValues();
        return success;
    }

    /**
        @brief Sets the value of the variable back to the default value and resets the config-file entry.
    */
    bool ConfigValueContainer::reset()
    {
        return this->set(this->defvalueString_);
    }

    /**
        @brief Retrieves the configured value from the currently loaded config-file.
    */
    void ConfigValueContainer::update()
    {
        this->value_.fromString(ConfigFileManager::getSingleton()->getValue(this->type_, this->sectionname_, this->varname_, this->defvalueString_));
    }

    /**
        @brief Parses a given std::string into a value of the type of the associated variable and assigns it.
        @param input The string to convert
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parse(const std::string& input)
    {
        MultiTypeMath temp = this->value_;
        if (temp.fromString(input))
        {
            this->value_ = temp;
            return true;
        }
        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type of the associated variable and assigns it.
        @param input The string to convert
        @param defvalue The default value to assign if the parsing fails
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parse(const std::string& input, const MultiTypeMath& defvalue)
    {
        MultiTypeMath temp = defvalue;
        if (temp.fromString(input))
        {
            this->value_ = temp;
            return true;
        }
        else
        {
            this->value_ = defvalue;
            return false;
        }
    }

    /**
        @brief Sets the corresponding entry in the config-file to a given value.
    */
    void ConfigValueContainer::setLineInConfigFile(const std::string& input)
    {
        ConfigFileManager::getSingleton()->setValue(this->type_, this->sectionname_, this->varname_, input);
    }

    /**
        @brief Sets the corresponding entry in the config-file back to the default value.
    */
    void ConfigValueContainer::resetLineInConfigFile()
    {
        this->setLineInConfigFile(this->value_.toString());
    }

    /**
        @brief Adds a description to the config-value.
        @param description The description
    */
    void ConfigValueContainer::description(const std::string& description)
    {
        if (!this->bAddedDescription_)
        {
            this->description_ = std::string("ConfigValueDescription::" + this->identifier_->getName() + "::" + this->varname_);
            AddLanguageEntry(this->description_, description);
            this->bAddedDescription_ = true;
        }
    }

    /**
        @brief Returns the description of the config-value.
        @return The description
    */
    const std::string& ConfigValueContainer::getDescription() const
    {
        return GetLocalisation(this->description_);
    }
}
