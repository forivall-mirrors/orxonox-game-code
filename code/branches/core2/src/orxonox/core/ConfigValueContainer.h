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
    @file ConfigValueContainer.h
    @brief Definition of the ConfigValueContainer class.

    The ConfigValueContainer class contains all needed informations about a configurable variable:
     - the name of the variable
     - the name of the class the variable belongs to
     - the default value
     - the user-specified value
     - a pointer to the entry in the config-file

    This is needed to assign the configured values to all newly created objects.
*/

#ifndef _ConfigValueContainer_H__
#define _ConfigValueContainer_H__

#include <list>
#include <string>

#include "CorePrereqs.h"

#include "util/Math.h"
#include "util/MultiTypeMath.h"

namespace orxonox
{
    //! The ConfigValuecontainer contains all needed informations about a configurable variable.
    /**
        The ConfigValueContainer class contains all needed informations about a configurable variable:
         - the name of the variable
         - the name of the class the variable belongs to
         - the default value
         - the user-specified value
         - a pointer to the entry in the config-file

        This is needed to assign the configured values to all newly created objects.

        The container searches for the entry in the config file.
        If there is an entry, it parses the specified value and assigns it to the variable of the right type.
        If there is no entry, it adds the entry with the default-value to the section of the variables class.
        If there is no section, the section and the entry are added to the end of the config-file.
    */
    class _CoreExport ConfigValueContainer
    {
        public:
            ConfigValueContainer(Identifier* identifier, const std::string& varname, MultiTypeMath defvalue);

            /** @brief Returns the configured value. @param value This is only needed to determine the right type. @return The value */
            template <typename T>
            inline ConfigValueContainer& getValue(T* value)
                { this->value_.getValue(value); return *this; }

            void description(const std::string& description);
            const std::string& getDescription() const;

            bool set(const std::string& input);
            bool tset(const std::string& input);
            bool reset();

            /** @brief Converts the config-value to a string. @return The string */
            inline std::string toString() const
                { return this->value_.toString(); }
            /** @brief Returns the typename of the assigned config-value. @return The typename */
            inline std::string getTypename() const
                { return this->value_.getTypename(); }

        private:
            static void readConfigFile(const std::string& filename);
            static void writeConfigFile(const std::string& filename);
            static std::list<std::string>& getConfigFileLines();
            static bool finishedReadingConfigFile(bool finished = false);

            bool parse(const std::string& input);
            bool parse(const std::string& input, const MultiTypeMath& defvalue);

            void setLineInConfigFile(const std::string& input);
            void resetLineInConfigFile();
            void searchLineInConfigFile();

            std::string parseValueStringFromConfigFile(bool bStripped = true);

            Identifier*         identifier_;                    //!< The name of the class the variable belongs to
            std::string         varname_;                       //!< The name of the variable
            std::string         defvalueString_;                //!< The string of the default-variable

            MultiTypeMath       value_;                         //!< The value

            std::list<std::string>::iterator configFileLine_;   //!< An iterator, pointing to the entry of the variable in the config-file

            bool bAddedDescription_;                            //!< True if a description was added
            LanguageEntryLabel description_;                    //!< The description
    };
}

#endif /* _ConfigValueContainer_H__ */
