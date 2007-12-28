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

#include "CorePrereqs.h"

#include "misc/Vector2.h"
#include "misc/Vector3.h"
#include "misc/Matrix3.h"
#include "misc/Quaternion.h"
#include "misc/String.h"
#include "misc/ColourValue.h"
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
            enum VariableType
            {
                Int,
                uInt,
                Char,
                uChar,
                Float,
                Double,
                LongDouble,
                Bool,
                ConstChar,
                _String,
                _Vector2,
                _Vector3,
                _ColourValue
            };

            ConfigValueContainer(const String& classname, const String& varname, int defvalue);
            ConfigValueContainer(const String& classname, const String& varname, unsigned int defvalue);
            ConfigValueContainer(const String& classname, const String& varname, char defvalue);
            ConfigValueContainer(const String& classname, const String& varname, unsigned char defvalue);
            ConfigValueContainer(const String& classname, const String& varname, float defvalue);
            ConfigValueContainer(const String& classname, const String& varname, double defvalue);
            ConfigValueContainer(const String& classname, const String& varname, long double defvalue);
            ConfigValueContainer(const String& classname, const String& varname, bool defvalue);
            ConfigValueContainer(const String& classname, const String& varname, const String& defvalue);
            ConfigValueContainer(const String& classname, const String& varname, const char* defvalue);
            ConfigValueContainer(const String& classname, const String& varname, Vector2 defvalue);
            ConfigValueContainer(const String& classname, const String& varname, Vector3 defvalue);
            ConfigValueContainer(const String& classname, const String& varname, ColourValue defvalue);

            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(int& value)                           { value = this->value_.value_int_; return *this; }
            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(unsigned int& value)                  { value = this->value_.value_uint_; return *this; }
            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(char& value)                          { value = this->value_.value_char_; return *this; }
            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(unsigned char& value)                 { value = this->value_.value_uchar_; return *this; }
            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(float& value)                         { value = this->value_.value_float_; return *this; }
            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(double& value)                        { value = this->value_.value_double_; return *this; }
            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(long double& value)                   { value = this->value_.value_long_double_; return *this; }
            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(bool& value)                          { value = this->value_.value_bool_; return *this; }
            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(String& value)                   { value = this->value_string_; return *this; }
            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(const char* value)                    { value = this->value_string_.c_str(); return *this; }
            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(Vector2& value)                 { value = this->value_vector2_; return *this; }
            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(Vector3& value)                 { value = this->value_vector3_; return *this; }
            /** @returns the value. @param value This is only needed to determine the right type. */
            inline ConfigValueContainer& getValue(ColourValue& value)             { value = this->value_colourvalue_; return *this; }

            void description(const String& description);

            bool parseSting(const String& input);
            void resetConfigFileEntry();
            void resetConfigValue();

            static String getStrippedLine(const String& line);
            static bool isEmpty(const String& line);
            static bool isComment(const String& line);

        private:
            bool parseSting(const String& input, int defvalue);
            bool parseSting(const String& input, unsigned int defvalue);
            bool parseSting(const String& input, char defvalue);
            bool parseSting(const String& input, unsigned char defvalue);
            bool parseSting(const String& input, float defvalue);
            bool parseSting(const String& input, double defvalue);
            bool parseSting(const String& input, long double defvalue);
            bool parseSting(const String& input, bool defvalue);
            bool parseSting(const String& input, const String& defvalue);
            bool parseSting(const String& input, const char* defvalue);
            bool parseSting(const String& input, const Vector2& defvalue);
            bool parseSting(const String& input, const Vector3& defvalue);
            bool parseSting(const String& input, const ColourValue& defvalue);

            static std::list<String>& getConfigFileLines();
            static bool finishedReadingConfigFile(bool finished = false);
            void searchConfigFileLine();
            String parseValueString(bool bStripped = true);

            static void readConfigFile(const String& filename);
            static void writeConfigFile(const String& filename);

            String         classname_;                     //!< The name of the class the variable belongs to
            String         varname_;                       //!< The name of the variable
            String         defvalueString_;                //!< The string of the default-variable

            union MultiType
            {
                int                 value_int_;                 //!< The value, if the variable is of the type int
                unsigned int        value_uint_;                //!< The value, if the variable is of the type unsigned int
                char                value_char_;                //!< The value, if the variable is of the type char
                unsigned char       value_uchar_;               //!< The value, if the variable is of the type unsigned char
                float               value_float_;               //!< The value, if the variable is of the type float
                double              value_double_;              //!< The value, if the variable is of the type double
                long double         value_long_double_;         //!< The value, if the variable is of the type long double
                bool                value_bool_;                //!< The value, if the variable is of the type bool
            } value_;                                           //!< The value of the variable

            String         value_string_;                  //!< The value, if the variable is of the type string
            Vector2       value_vector2_;                 //!< The value, if the variable is of the type Vector2
            Vector3       value_vector3_;                 //!< The value, if the variable is of the type Vector3
            ColourValue   value_colourvalue_;             //!< The value, if the variable is of the type ColourValue

            std::list<String>::iterator configFileLine_;   //!< An iterator, pointing to the entry of the variable in the config-file

            VariableType type_;                                 //!< The type of the variable
            bool bAddedDescription_;                            //!< True if a description was added
            LanguageEntryName description_;                     //!< The description
    };
}

#endif /* _ConfigValueContainer_H__ */
