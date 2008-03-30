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
#include "Iterator.h"
#include "BaseObject.h"

#define CONFIGFILEPATH "orxonox.ini"

namespace orxonox
{
    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(Identifier* identifier, const std::string& varname, MultiTypeMath defvalue)
    {
        this->bAddedDescription_ = false;
        this->identifier_ = identifier;
        this->varname_ = varname;

        this->defvalueString_ = defvalue.toString();                                // Convert the default-value to a string
        this->searchLineInConfigFile();                                             // Search the entry in the config-file

        std::string valueString = this->parseValueStringFromConfigFile(!(defvalue.isA(MT_string) || defvalue.isA(MT_constchar)));     // Parses the value string from the config-file-entry
        if (!this->parse(valueString, defvalue))                                    // Try to convert the string to a value
            this->resetLineInConfigFile();                                            // The conversion failed
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
        (*this->configFileLine_) = this->varname_ + "=" + input;
        ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
    }

    /**
        @brief Sets the corresponding entry in the config-file back to the default value.
    */
    void ConfigValueContainer::resetLineInConfigFile()
    {
        this->setLineInConfigFile(this->defvalueString_);
    }

    /**
        @brief Searches the corresponding entry in the config-file and creates it, if there is no entry.
    */
    void ConfigValueContainer::searchLineInConfigFile()
    {
        // Read the file if needed
        if (!ConfigValueContainer::finishedReadingConfigFile())
            ConfigValueContainer::readConfigFile(CONFIGFILEPATH);

        // The string of the section we're searching
        std::string section = "";
        section.append("[");
        section.append(this->identifier_->getName());
        section.append("]");

        // Iterate through all config-file-lines
        bool success = false;
        std::list<std::string>::iterator it1;
        for(it1 = ConfigValueContainer::getConfigFileLines().begin(); it1 != ConfigValueContainer::getConfigFileLines().end(); ++it1)
        {
            // Don't try to parse comments
            if (isComment(*it1))
                continue;

            if ((*it1).find(section) < (*it1).length())
            {
                // We found the right section
                bool bLineIsEmpty = false;
                std::list<std::string>::iterator positionToPutNewLineAt;

                // Iterate through all lines in the section
                std::list<std::string>::iterator it2;
                for(it2 = ++it1; it2 != ConfigValueContainer::getConfigFileLines().end(); ++it2)
                {
                    // Don't try to parse comments
                    if (isComment(*it2))
                        continue;

                    // This if-else block is used to write a new line right after the last line of the
                    // section but in front of the following empty lines before the next section.
                    // (So this helps to keep a nice formatting with empty-lines between sections in the config-file)
                    if (isEmpty(*it2))
                    {
                        if (!bLineIsEmpty)
                        {
                            bLineIsEmpty = true;
                            positionToPutNewLineAt = it2;
                        }
                    }
                    else
                    {
                        if (!bLineIsEmpty)
                            positionToPutNewLineAt = it2;

                        bLineIsEmpty = false;
                    }

                    // Look out for the beginning of the next section
                    unsigned int open = (*it2).find("[");
                    unsigned int close = (*it2).find("]");
                    if ((open < (*it2).length()) && (close < (*it2).length()) && (open < close))
                    {
                        // The next section startet, so our line isn't yet in the file - now we add it and safe the file
                        this->configFileLine_ = this->getConfigFileLines().insert(positionToPutNewLineAt, this->varname_ + "=" + this->defvalueString_);
                        ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
                        success = true;
                        break;
                    }

                    // Look out for the variable-name
                    if ((*it2).find(this->varname_) < (*it2).length())
                    {
                        // We found the right line - safe it and return
                        this->configFileLine_ = it2;
                        success = true;
                        break;
                    }
                }

                // Check if we succeeded
                if (!success)
                {
                    // Looks like we found the right section, but the file ended without containing our variable - so we add it and safe the file
                    this->configFileLine_ = this->getConfigFileLines().insert(positionToPutNewLineAt, this->varname_ + "=" + this->defvalueString_);
                    ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
                    success = true;
                }
                break;
            }
        }

        // Check if we succeeded
        if (!success)
        {
            // We obviously didn't found the right section, so we'll create it
            this->getConfigFileLines().push_back("[" + this->identifier_->getName() + "]");                   // Create the section
            this->getConfigFileLines().push_back(this->varname_ + "=" + this->defvalueString_);   // Create the line
            this->configFileLine_ = --this->getConfigFileLines().end();                           // Set the pointer to the last element
            success = true;
            this->getConfigFileLines().push_back("");                                             // Add an empty line - this is needed for the algorithm in the searchLineInConfigFile-function
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);                                // Save the changed config-file
        }
    }

    /**
        @brief Returns the part in the corresponding config-file-entry of the container that defines the value.
        @param bStripped True = strip the value-string
        @return The value-string
    */
    std::string ConfigValueContainer::parseValueStringFromConfigFile(bool bStripped)
    {
        std::string output;
        if (bStripped)
            output = getStripped(*this->configFileLine_);
        else
            output = *this->configFileLine_;

        return output.substr(output.find("=") + 1);
    }

    /**
        @brief Rreturns a list, containing all entrys in the config-file.
        @return The list
    */
    std::list<std::string>& ConfigValueContainer::getConfigFileLines()
    {
        // This is done to avoid problems while executing this code before main()
        static std::list<std::string> configFileLinesStaticReference = std::list<std::string>();
        return configFileLinesStaticReference;
    }

    /**
        @brief Returns true if the ConfigFile is read and stored into the ConfigFile-lines-list.
        @param finished This is used to change the state
        @return True if the ConfigFile is read and stored into the ConfigFile-lines-list
    */
    bool ConfigValueContainer::finishedReadingConfigFile(bool finished)
    {
        // This is done to avoid problems while executing this code before main()
        static bool finishedReadingConfigFileStaticVariable = false;

        if (finished)
            finishedReadingConfigFileStaticVariable = true;

        return finishedReadingConfigFileStaticVariable;
    }

    /**
        @brief Reads the config-file and stores the lines in a list.
        @param filename The name of the config-file
    */
    void ConfigValueContainer::readConfigFile(const std::string& filename)
    {
        // This creates the file if it's not existing
        std::ofstream createFile;
        createFile.open(filename.c_str(), std::fstream::app);
        createFile.close();

        // Open the file
        std::ifstream file;
        file.open(filename.c_str(), std::fstream::in);

        if (!file.is_open())
        {
            COUT(1) << "An error occurred in ConfigValueContainer.cc:" << std::endl;
            COUT(1) << "Error: Couldn't open config-file " << filename << " to read the config values!" << std::endl;
            return;
        }

        char line[1024];

        // Iterate through the file and add the lines into the list
        while (file.good() && !file.eof())
        {
            file.getline(line, 1024);
            ConfigValueContainer::getConfigFileLines().push_back(line);
        }

        // The last line is useless
        ConfigValueContainer::getConfigFileLines().pop_back();

        // Add an empty line to the end of the file if needed
        // this is needed for the algorithm in the searchLineInConfigFile-function
        if ((ConfigValueContainer::getConfigFileLines().size() > 0) && !isEmpty(*ConfigValueContainer::getConfigFileLines().rbegin()))
            ConfigValueContainer::getConfigFileLines().push_back("");

        file.close();

        ConfigValueContainer::finishedReadingConfigFile(true);
    }

    /**
        @brief Writes the content of the list, containing all lines of the config-file, into the config-file.
        @param filename The name of the config-file
    */
    void ConfigValueContainer::writeConfigFile(const std::string& filename)
    {
        // Make sure we stored the config-file in the list
        if (!ConfigValueContainer::finishedReadingConfigFile())
            ConfigValueContainer::readConfigFile(filename);

        // Open the file
        std::ofstream file;
        file.open(filename.c_str(), std::fstream::out);
        file.setf(std::ios::fixed, std::ios::floatfield);
        file.precision(6);

        if (!file.is_open())
        {
            COUT(1) << "An error occurred in ConfigValueContainer.cc:" << std::endl;
            COUT(1) << "Error: Couldn't open config-file " << filename << " to write the config values!" << std::endl;
            return;
        }

        // Iterate through the list an write the lines into the file
        std::list<std::string>::iterator it;
        for (it = ConfigValueContainer::getConfigFileLines().begin(); it != ConfigValueContainer::getConfigFileLines().end(); ++it)
        {
            file << (*it) << std::endl;
        }

        file.close();
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
