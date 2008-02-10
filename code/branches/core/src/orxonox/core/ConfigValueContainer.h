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

/*!
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
#include "Language.h"

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
            ConfigValueContainer(const std::string& classname, const std::string& varname, MultiTypeMath defvalue);

            /** @returns the value. @param value This is only needed to determine the right type. */
/*            template <typename T>
            inline ConfigValueContainer& getValue(T& value)                           { this->value_.getValue(value); return *this; }
*/
            inline ConfigValueContainer& getValue(int* value)            { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(unsigned int* value)   { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(char* value)           { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(unsigned char* value)  { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(short* value)          { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(unsigned short* value) { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(long* value)           { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(unsigned long* value)  { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(float* value)          { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(double* value)         { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(long double* value)    { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(bool* value)           { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(std::string* value)    { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(Vector2* value)        { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(Vector3* value)        { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(ColourValue* value)    { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(Quaternion* value)     { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(Radian* value)         { this->value_.getValue(value); return *this; }
            inline ConfigValueContainer& getValue(Degree* value)         { this->value_.getValue(value); return *this; }

            void description(const std::string& description);

            bool parseString(const std::string& input, MultiTypeMath& defvalue);
            bool valueToString(std::string* output, MultiTypeMath& input);
            void resetConfigFileEntry();
            void resetConfigValue();

            static std::string getStrippedLine(const std::string& line);
            static bool isEmpty(const std::string& line);
            static bool isComment(const std::string& line);

        private:
            bool parseString(const std::string& input, int defvalue);
            bool parseString(const std::string& input, unsigned int defvalue);
            bool parseString(const std::string& input, char defvalue);
            bool parseString(const std::string& input, unsigned char defvalue);
            bool parseString(const std::string& input, short defvalue);
            bool parseString(const std::string& input, unsigned short defvalue);
            bool parseString(const std::string& input, long defvalue);
            bool parseString(const std::string& input, unsigned long defvalue);
            bool parseString(const std::string& input, float defvalue);
            bool parseString(const std::string& input, double defvalue);
            bool parseString(const std::string& input, long double defvalue);
            bool parseString(const std::string& input, bool defvalue);
            bool parseString(const std::string& input, const std::string& defvalue);
            bool parseString(const std::string& input, const char* defvalue);
            bool parseString(const std::string& input, const Vector2& defvalue);
            bool parseString(const std::string& input, const Vector3& defvalue);
            bool parseString(const std::string& input, const ColourValue& defvalue);
            bool parseString(const std::string& input, const Quaternion& defvalue);
            bool parseString(const std::string& input, const Radian& defvalue);
            bool parseString(const std::string& input, const Degree& defvalue);

            static std::list<std::string>& getConfigFileLines();
            static bool finishedReadingConfigFile(bool finished = false);
            void searchConfigFileLine();
            std::string parseValueString(bool bStripped = true);

            static void readConfigFile(const std::string& filename);
            static void writeConfigFile(const std::string& filename);

            std::string         classname_;                     //!< The name of the class the variable belongs to
            std::string         varname_;                       //!< The name of the variable
            std::string         defvalueString_;                //!< The string of the default-variable

            MultiTypeMath       value_;                         //!< The value

            std::list<std::string>::iterator configFileLine_;   //!< An iterator, pointing to the entry of the variable in the config-file

            bool bAddedDescription_;                            //!< True if a description was added
            LanguageEntryName description_;                     //!< The description
    };
}

#endif /* _ConfigValueContainer_H__ */
