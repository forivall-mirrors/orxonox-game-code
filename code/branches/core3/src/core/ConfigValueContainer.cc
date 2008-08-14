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
    @file ConfigValueContainer.cc
    @brief Implementation of the ConfigValueContainer class.
*/

#include "ConfigValueContainer.h"

#include <fstream>

#include "util/SubString.h"
#include "util/Convert.h"
#include "Language.h"
#include "Identifier.h"

#define MAX_VECTOR_INDEX 255 // to avoid up to 4*10^9 vector entries in the config file after accidentally using a wrong argument


namespace orxonox
{
    /**
        @brief Initializes the ConfigValueContainer with defaultvalues.
    */
    void ConfigValueContainer::init(ConfigFileType type, Identifier* identifier, const std::string& varname)
    {
        this->type_ = type;
        this->identifier_ = identifier;
        this->sectionname_ = identifier->getName();
        this->varname_ = varname;
        this->callback_ = 0;
        this->bContainerIsNew_ = true;
        this->bDoInitialCallback_ = false;
        this->bAddedDescription_ = false;
    }

    /**
        @brief Does some special initialization for single config-values.
    */
    void ConfigValueContainer::initValue(const MultiTypeMath& defvalue)
    {
        this->value_ = defvalue;
        this->bIsVector_ = false;

        this->defvalueString_ = this->value_.toString();
        this->update();
    }

    /**
        @brief Does some special initialization for vector config-values.
    */
    void ConfigValueContainer::initVector()
    {
        this->bIsVector_ = true;

        for (unsigned int i = 0; i < this->valueVector_.size(); i++)
        {
            ConfigFileManager::getInstance()->getValue(this->type_, this->sectionname_, this->varname_, i, this->valueVector_[i].toString(), this->value_.isA(MT_string));
            this->defvalueStringVector_.push_back(this->valueVector_[i].toString());
        }

        this->update();
    }

    /**
        @brief Destructor: Deletes the callback object if necessary.
    */
    ConfigValueContainer::~ConfigValueContainer()
    {
        if (this->callback_)
            delete this->callback_;
    }

    /**
        @brief Assigns a new value to the config-value of all objects and writes the change into the config-file.
        @param input The new value
        @return True if the new value was successfully assigned
    */
    bool ConfigValueContainer::set(const MultiTypeMath& input)
    {
        if (this->bIsVector_)
        {
            return this->callFunctionWithIndex(&ConfigValueContainer::set, input.toString());
        }
        else
        {
            if (this->tset(input))
            {
                ConfigFileManager::getInstance()->setValue(this->type_, this->sectionname_, this->varname_, input.toString(), this->value_.isA(MT_string));
                return true;
            }
        }
        return false;
    }

    /**
        @brief Assigns a new value to the config-value of all objects and writes the change into the config-file.
        @param index The index in the vector
        @param input The new value
        @return True if the new value was successfully assigned
    */
    bool ConfigValueContainer::set(unsigned int index, const MultiTypeMath& input)
    {
        if (this->bIsVector_)
        {
            if (this->tset(index, input))
            {
                ConfigFileManager::getInstance()->setValue(this->type_, this->sectionname_, this->varname_, index, input.toString(), this->value_.isA(MT_string));
                return true;
            }
        }
        else
        {
            COUT(1) << "Error: Config-value '" << this->varname_ << "' in " << this->sectionname_ << " is not a vector." << std::endl;
        }
        return false;
    }

    /**
        @brief Assigns a new value to the config-value of all objects, but doesn't change the config-file (t stands for temporary).
        @param input The new value. If bIsVector_ then write "index value"
        @return True if the new value was successfully assigned
    */
    bool ConfigValueContainer::tset(const MultiTypeMath& input)
    {
        if (this->bIsVector_)
        {
            return this->callFunctionWithIndex(&ConfigValueContainer::tset, input.toString());
        }
        else
        {
            MultiTypeMath temp = this->value_;
            if (temp.assimilate(input))
            {
                this->value_ = temp;
                if (this->identifier_)
                    this->identifier_->updateConfigValues();

                return true;
            }
        }
        return false;
    }

    /**
        @brief Assigns a new value to the config-value of all objects, but doesn't change the config-file (t stands for temporary).
        @param index The index in the vector
        @param input The new value
        @return True if the new value was successfully assigned
    */
    bool ConfigValueContainer::tset(unsigned int index, const MultiTypeMath& input)
    {
        if (this->bIsVector_)
        {
            if (index > MAX_VECTOR_INDEX)
            {
                COUT(1) << "Error: Index " << index << " is too large." << std::endl;
                return false;
            }

            if (index >= this->valueVector_.size())
            {
                for (unsigned int i = this->valueVector_.size(); i <= index; i++)
                {
                    this->valueVector_.push_back(MultiTypeMath());
                }
            }

            MultiTypeMath temp = this->value_;
            if (temp.assimilate(input))
            {
                this->valueVector_[index] = temp;

                if (this->identifier_)
                    this->identifier_->updateConfigValues();

                return true;
            }
        }
        else
        {
            COUT(1) << "Error: Config-value '" << this->varname_ << "' in " << this->sectionname_ << " is not a vector." << std::endl;
        }
        return false;
    }

    /**
        @brief Adds a new entry to the end of the vector.
        @param input The new entry
        @return True if the new entry was successfully added
    */
    bool ConfigValueContainer::add(const MultiTypeMath& input)
    {
        if (this->bIsVector_)
            return this->set(this->valueVector_.size(), input);

        COUT(1) << "Error: Config-value '" << this->varname_ << "' in " << this->sectionname_ << " is not a vector." << std::endl;
        return false;
    }

    /**
        @brief Removes an existing entry from the vector.
        @param index The index of the entry
        @return True if the entry was removed
    */
    bool ConfigValueContainer::remove(unsigned int index)
    {
        if (this->bIsVector_)
        {
            if (index < this->valueVector_.size())
            {
                // Erase the entry from the vector, change (shift) all entries beginning with index in the config file, remove the last entry from the file
                this->valueVector_.erase(this->valueVector_.begin() + index);
                for (unsigned int i = index; i < this->valueVector_.size(); i++)
                    ConfigFileManager::getInstance()->setValue(this->type_, this->sectionname_, this->varname_, i, this->valueVector_[i], this->value_.isA(MT_string));
                ConfigFileManager::getInstance()->deleteVectorEntries(this->type_, this->sectionname_, this->varname_, this->valueVector_.size());

                return true;
            }
            COUT(1) << "Error: Invalid vector-index." << std::endl;
        }

        COUT(1) << "Error: Config-value '" << this->varname_ << "' in " << this->sectionname_ << " is not a vector." << std::endl;
        return false;
    }

    /**
        @brief Sets the value of the variable back to the default value and resets the config-file entry.
    */
    bool ConfigValueContainer::reset()
    {
        if (!this->bIsVector_)
            return this->set(this->defvalueString_);
        else
        {
            bool success = true;
            for (unsigned int i = 0; i < this->defvalueStringVector_.size(); i++)
                if (!this->set(i, this->defvalueStringVector_[i]))
                    success = false;
            ConfigFileManager::getInstance()->deleteVectorEntries(this->type_, this->sectionname_, this->varname_, this->defvalueStringVector_.size());
            return success;
        }
    }

    /**
        @brief Retrieves the configured value from the currently loaded config-file.
    */
    void ConfigValueContainer::update()
    {
        if (!this->bIsVector_)
            this->value_.fromString(ConfigFileManager::getInstance()->getValue(this->type_, this->sectionname_, this->varname_, this->defvalueString_, this->value_.isA(MT_string)));
        else
        {
            this->valueVector_.clear();
            for (unsigned int i = 0; i < ConfigFileManager::getInstance()->getVectorSize(this->type_, this->sectionname_, this->varname_); i++)
            {
                if (i < this->defvalueStringVector_.size())
                {
                    this->value_.fromString(ConfigFileManager::getInstance()->getValue(this->type_, this->sectionname_, this->varname_, i, this->defvalueStringVector_[i], this->value_.isA(MT_string)));
                }
                else
                {
                    this->value_.fromString(ConfigFileManager::getInstance()->getValue(this->type_, this->sectionname_, this->varname_, i, MultiTypeMath(), this->value_.isA(MT_string)));
                }

                this->valueVector_.push_back(this->value_);
            }
        }
    }

    /**
        @brief Calls the given function with parsed index and the parsed argument from the input string.
        @param function The function to call
        @param input The input string
        @return The returnvalue of the functioncall
    */
    bool ConfigValueContainer::callFunctionWithIndex(bool (ConfigValueContainer::* function) (unsigned int, const MultiTypeMath&), const std::string& input)
    {
        SubString token(input, " ", SubString::WhiteSpaces, true, '\\', false, '"', false, '(', ')', false, '\0');
        int index = -1;
        bool success = false;

        if (token.size() > 0)
            success = ConvertValue(&index, token[0]);

        if (!success || index < 0 || index > MAX_VECTOR_INDEX)
        {
            if (!success)
            {
                COUT(1) << "Error: Config-value '" << this->varname_ << "' in " << this->sectionname_ << " is a vector." << std::endl;
            }
            else
            {
                COUT(1) << "Error: Invalid vector-index." << std::endl;
            }
            return false;
        }

        if (token.size() >= 2)
            return (this->*function)(index, token.subSet(1).join());
        else
            return (this->*function)(index, "");
    }

    /**
        @brief Adds a description to the config-value.
        @param description The description
    */
    ConfigValueContainer& ConfigValueContainer::description(const std::string& description)
    {
        if (!this->bAddedDescription_)
        {
            this->description_ = std::string("ConfigValueDescription::" + this->identifier_->getName() + "::" + this->varname_);
            AddLanguageEntry(this->description_, description);
            this->bAddedDescription_ = true;
        }
        return (*this);
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
