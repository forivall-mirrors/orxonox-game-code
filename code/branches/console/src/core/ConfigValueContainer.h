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

#include "CorePrereqs.h"

#include <string>
#include <vector>

#include "util/Math.h"
#include "util/MultiTypeMath.h"
#include "ConfigFileManager.h"

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
            ConfigValueContainer(ConfigFileType type, Identifier* identifier, const std::string& varname, const MultiTypeMath& defvalue);
            ConfigValueContainer(ConfigFileType type, Identifier* identifier, const std::string& varname, const std::vector<MultiTypeMath>& defvalue);

            /** @brief Returns the configured value. @param value This is only needed to determine the right type. @return The value */
            template <typename T>
            inline ConfigValueContainer& getValue(T* value)
                { this->value_.getValue(value); return *this; }
            template <typename T>
            inline ConfigValueContainer& getValue(std::vector<T>* value)
            {
                value->clear();
                for (unsigned int i = 0; i < this->valueVector_.size(); i++)
                    value->push_back(this->valueVector_[i]);
                return *this;
            }

            template <typename T>
            inline void setVectorType(const std::vector<T>& value)
            {
                this->value_ = T();
            }

            inline const std::string& getName() const
                { return this->varname_; }
            inline bool isVector() const
                { return this->bIsVector_; }
            inline unsigned int getVectorSize() const
                { return this->valueVector_.size(); }

            void description(const std::string& description);
            const std::string& getDescription() const;

            bool set(const MultiTypeMath& input);
            bool tset(const MultiTypeMath& input);

            bool set(unsigned int index, const MultiTypeMath& input);
            bool tset(unsigned int index, const MultiTypeMath& input);
            bool add(const MultiTypeMath& input);
            bool remove(unsigned int index);

            bool reset();
            void update();

            /** @brief Converts the config-value to a string. @return The string */
            inline std::string toString() const
                { return this->value_.toString(); }
            /** @brief Returns the typename of the assigned config-value. @return The typename */
            inline std::string getTypename() const
                { return this->value_.getTypename(); }

        private:
            bool callFunctionWithIndex(bool (ConfigValueContainer::* function) (unsigned int, const MultiTypeMath&), const std::string& input);

            bool                       bIsVector_;                  //!< True if the container contains a std::vector

            ConfigFileType             type_;                       //!< The type of the corresponding config-file
            Identifier*                identifier_;                 //!< The identifier of the class
            std::string                sectionname_;                //!< The name of the class the variable belongs to
            std::string                varname_;                    //!< The name of the variable
            std::string                defvalueString_;             //!< The string of the default-value
            std::vector<std::string>   defvalueStringVector_;       //!< A vector, containg the strings of the default-values in case we're storing a vector

            MultiTypeMath              value_;                      //!< The value
            std::vector<MultiTypeMath> valueVector_;                //!< A vector, containg the values in case we're storing a vector

            bool                       bAddedDescription_;          //!< True if a description was added
            LanguageEntryLabel         description_;                //!< The description
    };
}

#endif /* _ConfigValueContainer_H__ */
