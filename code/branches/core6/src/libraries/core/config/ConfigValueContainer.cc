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
    @brief Implementation of the ConfigValueContainer class.
*/

#include "ConfigValueContainer.h"

#include "util/Convert.h"
#include "util/SubString.h"
#include "core/Language.h"
#include "ConfigFileManager.h"
#include "SettingsConfigFile.h"

namespace orxonox
{
    const unsigned int MAX_VECTOR_INDEX  = 255; // to avoid up to 4*10^9 vector entries in the config file after accidentally using a wrong argument

    /**
        @brief Initializes the ConfigValueContainer with default values.
    */
    void ConfigValueContainer::init(ConfigFileType::Value type, Identifier* identifier, const std::string& sectionname, const std::string& varname)
    {
        this->type_ = type;
        this->identifier_ = identifier;
        this->sectionname_ = sectionname;
        this->varname_ = varname;
        this->callback_ = 0;
        this->bContainerIsNew_ = true;
        this->bDoInitialCallback_ = false;
        this->bAddedDescription_ = false;

        // Register containers for general settings
        if (this->type_ == ConfigFileType::Settings)
            SettingsConfigFile::getInstance().addConfigValueContainer(this);
    }

    /**
        @brief Does some special initialization for single config-values.
    */
    void ConfigValueContainer::initValue(const MultiType& defvalue)
    {
        this->value_ = defvalue;
        this->bIsVector_ = false;

        this->defvalueString_ = this->value_.get<std::string>();
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
            ConfigFileManager::getInstance().getConfigFile(this->type_)->getOrCreateValue(this->sectionname_, this->varname_, i, this->valueVector_[i], this->value_.isType<std::string>());
            this->defvalueStringVector_.push_back(this->valueVector_[i]);
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

        // Unregister general settings containers
        if (this->type_ == ConfigFileType::Settings && SettingsConfigFile::exists())
            SettingsConfigFile::getInstance().removeConfigValueContainer(this);
    }

    /**
        @brief Assigns a new value to the config-value of all objects and writes the change into the config-file.
        @param input The new value
        @return True if the new value was successfully assigned
    */
    bool ConfigValueContainer::set(const MultiType& input)
    {
        if (this->bIsVector_)
        {
            return this->callFunctionWithIndex(&ConfigValueContainer::set, input);
        }
        else
        {
            if (this->tset(input))
            {
                ConfigFileManager::getInstance().getConfigFile(this->type_)->setValue(this->sectionname_, this->varname_, input, this->value_.isType<std::string>());
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
    bool ConfigValueContainer::set(unsigned int index, const MultiType& input)
    {
        if (this->bIsVector_)
        {
            if (this->tset(index, input))
            {
                ConfigFileManager::getInstance().getConfigFile(this->type_)->setValue(this->sectionname_, this->varname_, index, input, this->value_.isType<std::string>());
                return true;
            }
        }
        else
        {
            orxout(user_error, context::config) << "Config-value '" << this->varname_ << "' in " << this->sectionname_ << " is not a vector." << endl;
        }
        return false;
    }

    /**
        @brief Assigns a new value to the config-value of all objects, but doesn't change the config-file (t stands for temporary).
        @param input The new value. If bIsVector_ then write "index value"
        @return True if the new value was successfully assigned
    */
    bool ConfigValueContainer::tset(const MultiType& input)
    {
        if (this->bIsVector_)
        {
            return this->callFunctionWithIndex(&ConfigValueContainer::tset, input);
        }
        else
        {
            this->value_ = input;

            if (this->identifier_)
                this->identifier_->updateConfigValues();

            return true;
        }
    }

    /**
        @brief Assigns a new value to the config-value of all objects, but doesn't change the config-file (t stands for temporary).
        @param index The index in the vector
        @param input The new value
        @return True if the new value was successfully assigned
    */
    bool ConfigValueContainer::tset(unsigned int index, const MultiType& input)
    {
        if (this->bIsVector_)
        {
            if (index > MAX_VECTOR_INDEX)
            {
                orxout(user_error, context::config) << "Index " << index << " is too large." << endl;
                return false;
            }

            if (index >= this->valueVector_.size())
            {
                for (unsigned int i = this->valueVector_.size(); i <= index; i++)
                {
                    this->valueVector_.push_back(MultiType());
                }
            }

            this->valueVector_[index] = input;

            if (this->identifier_)
                this->identifier_->updateConfigValues();

            return true;
        }
        else
        {
            orxout(user_error, context::config) << "Config-value '" << this->varname_ << "' in " << this->sectionname_ << " is not a vector." << endl;
            return false;
        }
    }

    /**
        @brief Adds a new entry to the end of the vector.
        @param input The new entry
        @return True if the new entry was successfully added
    */
    bool ConfigValueContainer::add(const MultiType& input)
    {
        if (this->bIsVector_)
            return this->set(this->valueVector_.size(), input);

        orxout(user_error, context::config) << "Config-value '" << this->varname_ << "' in " << this->sectionname_ << " is not a vector." << endl;
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
                    ConfigFileManager::getInstance().getConfigFile(this->type_)->setValue(this->sectionname_, this->varname_, i, this->valueVector_[i], this->value_.isType<std::string>());
                ConfigFileManager::getInstance().getConfigFile(this->type_)->deleteVectorEntries(this->sectionname_, this->varname_, this->valueVector_.size());

                return true;
            }
            orxout(user_error, context::config) << "Invalid vector-index." << endl;
        }

        orxout(user_error, context::config) << "Config-value '" << this->varname_ << "' in " << this->sectionname_ << " is not a vector." << endl;
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
            ConfigFileManager::getInstance().getConfigFile(this->type_)->deleteVectorEntries(this->sectionname_, this->varname_, this->defvalueStringVector_.size());
            return success;
        }
    }

    /**
        @brief Retrieves the configured value from the currently loaded config-file.
    */
    void ConfigValueContainer::update()
    {
        if (!this->bIsVector_)
            this->value_ = ConfigFileManager::getInstance().getConfigFile(this->type_)->getOrCreateValue(this->sectionname_, this->varname_, this->defvalueString_, this->value_.isType<std::string>());
        else
        {
            this->valueVector_.clear();
            unsigned int vectorSize = ConfigFileManager::getInstance().getConfigFile(this->type_)->getVectorSize(this->sectionname_, this->varname_);
            for (unsigned int i = 0; i < vectorSize; i++)
            {
                if (i < this->defvalueStringVector_.size())
                {
                    this->value_ = ConfigFileManager::getInstance().getConfigFile(this->type_)->getOrCreateValue(this->sectionname_, this->varname_, i, this->defvalueStringVector_[i], this->value_.isType<std::string>());
                }
                else
                {
                    this->value_ = ConfigFileManager::getInstance().getConfigFile(this->type_)->getOrCreateValue(this->sectionname_, this->varname_, i, MultiType(), this->value_.isType<std::string>());
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
    bool ConfigValueContainer::callFunctionWithIndex(bool (ConfigValueContainer::* function) (unsigned int, const MultiType&), const std::string& input)
    {
        SubString token(input, " ", SubString::WhiteSpaces, true, '\\', false, '"', false, '\0', '\0', false, '\0');
        int index = -1;
        bool success = false;

        if (token.size() > 0)
            success = convertValue(&index, token[0]);

        if (!success || index < 0 || index > (signed int)MAX_VECTOR_INDEX)
        {
            if (!success)
            {
                orxout(user_error, context::config) << "Config-value '" << this->varname_ << "' in " << this->sectionname_ << " is a vector." << endl;
            }
            else
            {
                orxout(user_error, context::config) << "Invalid vector-index." << endl;
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
            this->description_ = std::string("ConfigValueDescription::" + this->sectionname_ + "::" + this->varname_);
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
