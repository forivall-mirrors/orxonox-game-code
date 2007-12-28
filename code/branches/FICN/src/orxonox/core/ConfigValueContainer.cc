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

#include <fstream>

#include "../../misc/Tokenizer.h"
#include "../../misc/String2Number.h"
#include "ConfigValueContainer.h"

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
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, int defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Int;

        this->defvalueString_ = number2String(defvalue, "0");                       // Try to convert the default-value to a string
        this->searchConfigFileLine();                                               // Search the entry in the config-file

        std::string valueString = this->parseValueString();                         // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, unsigned int defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = uInt;

        this->defvalueString_ = number2String(defvalue, "0");                       // Try to convert the default-value to a string
        this->searchConfigFileLine();                                               // Search the entry in the config-file

        std::string valueString = this->parseValueString();                         // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, char defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Char;

        this->defvalueString_ = number2String((int)defvalue, "0");                  // Try to convert the default-value to a string
        this->searchConfigFileLine();                                               // Search the entry in the config-file

        std::string valueString = this->parseValueString();                         // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, unsigned char defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = uChar;

        this->defvalueString_ = number2String((unsigned int)defvalue, "0");         // Try to convert the default-value to a string
        this->searchConfigFileLine();                                               // Search the entry in the config-file

        std::string valueString = this->parseValueString();                         // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, float defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Float;

        this->defvalueString_ = number2String(defvalue, "0.000000");                // Try to convert the default-value to a string
        this->searchConfigFileLine();                                               // Search the entry in the config-file

        std::string valueString = this->parseValueString();                         // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, double defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Double;

        this->defvalueString_ = number2String(defvalue, "0.000000");                // Try to convert the default-value to a string
        this->searchConfigFileLine();                                               // Search the entry in the config-file

        std::string valueString = this->parseValueString();                         // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, long double defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = LongDouble;

        this->defvalueString_ = number2String(defvalue, "0.000000");                // Try to convert the default-value to a string
        this->searchConfigFileLine();                                               // Search the entry in the config-file

        std::string valueString = this->parseValueString();                         // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, bool defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Bool;

        // Convert the default-value from bool to string
        if (defvalue)
            this->defvalueString_ = "true";
        else
            this->defvalueString_ = "false";

        this->searchConfigFileLine();                                               // Search the entry in the config-file
        std::string valueString = this->parseValueString();                         // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, const std::string& defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = String;

        this->defvalueString_ = "\"" + defvalue + "\"";                             // Convert the string to a "config-file-string" with quotes
        this->searchConfigFileLine();                                               // Search the entry in the config-file
        std::string valueString = this->parseValueString(false);                    // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, const char* defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = ConstChar;

        this->defvalueString_ = "\"" + std::string(defvalue) + "\"";                // Convert the string to a "config-file-string" with quotes
        this->searchConfigFileLine();                                               // Search the entry in the config-file
        std::string valueString = this->parseValueString(false);                    // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::Vector2 defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Vector2;

        // Try to convert the default-value from Vector2 to string
        std::ostringstream ostream;
        if (ostream << "(" << defvalue.x << "," << defvalue.y << ")")
            this->defvalueString_ = ostream.str();
        else
            this->defvalueString_ = "(0,0)";

        this->searchConfigFileLine();                                               // Search the entry in the config-file
        std::string valueString = this->parseValueString();                         // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::Vector3 defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Vector3;

        // Try to convert the default-value from Vector3 to string
        std::ostringstream ostream;
        if (ostream << "(" << defvalue.x << "," << defvalue.y << "," << defvalue.z << ")")
            this->defvalueString_ = ostream.str();
        else
            this->defvalueString_ = "(0,0,0)";

        this->searchConfigFileLine();                                               // Search the entry in the config-file
        std::string valueString = this->parseValueString();                         // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::ColourValue defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = ColourValue;

        // Try to convert the default-value from ColourValue to string
        std::ostringstream ostream;
        if (ostream << "(" << defvalue.r << "," << defvalue.g << "," << defvalue.b << "," << defvalue.a << ")")
            this->defvalueString_ = ostream.str();
        else
            this->defvalueString_ = "(0,0,0,0)";

        this->searchConfigFileLine();                                               // Search the entry in the config-file
        std::string valueString = this->parseValueString();                         // Parses the value string from the config-file-entry
        if (!this->parseSting(valueString, defvalue))                               // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Parses a given std::string into a value of the type of the associated variable and assigns it.
        @param input The string to convert
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input)
    {
        if (this->type_ == ConfigValueContainer::Int)
            return this->parseSting(input, this->value_.value_int_);
        else if (this->type_ == ConfigValueContainer::uInt)
            return this->parseSting(input, this->value_.value_uint_);
        else if (this->type_ == ConfigValueContainer::Char)
            return this->parseSting(input, this->value_.value_char_);
        else if (this->type_ == ConfigValueContainer::uChar)
            return this->parseSting(input, this->value_.value_uchar_);
        else if (this->type_ == ConfigValueContainer::Float)
            return this->parseSting(input, this->value_.value_float_);
        else if (this->type_ == ConfigValueContainer::Double)
            return this->parseSting(input, this->value_.value_double_);
        else if (this->type_ == ConfigValueContainer::LongDouble)
            return this->parseSting(input, this->value_.value_long_double_);
        else if (this->type_ == ConfigValueContainer::Bool)
            return this->parseSting(input, this->value_.value_bool_);
        else if (this->type_ == ConfigValueContainer::String)
            return this->parseSting(input, this->value_string_);
        else if (this->type_ == ConfigValueContainer::ConstChar)
            return this->parseSting(input, this->value_string_);
        else if (this->type_ == ConfigValueContainer::Vector2)
            return this->parseSting(input, this->value_vector2_);
        else if (this->type_ == ConfigValueContainer::Vector3)
            return this->parseSting(input, this->value_vector3_);
        else if (this->type_ == ConfigValueContainer::ColourValue)
            return this->parseSting(input, this->value_colourvalue_);

        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type int and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, int defvalue)
    {
        return string2Number(this->value_.value_int_, input, defvalue);
    }

    /**
        @brief Parses a given std::string into a value of the type unsigned int and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, unsigned int defvalue)
    {
        return string2Number(this->value_.value_uint_, input, defvalue);
    }

    /**
        @brief Parses a given std::string into a value of the type char and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, char defvalue)
    {
        // I used value_int_ instead of value_char_ to avoid number <-> char confusion in the config-file
        return string2Number(this->value_.value_int_, input, (int)defvalue);
    }

    /**
        @brief Parses a given std::string into a value of the type unsigned char and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, unsigned char defvalue)
    {
        // I used value_uint_ instead of value_uchar_ to avoid number <-> char confusion in the config-file
        return string2Number(this->value_.value_uint_, input, (unsigned int)defvalue);
    }

    /**
        @brief Parses a given std::string into a value of the type float and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, float defvalue)
    {
        return string2Number(this->value_.value_float_, input, defvalue);
    }

    /**
        @brief Parses a given std::string into a value of the type double and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, double defvalue)
    {
        return string2Number(this->value_.value_double_, input, defvalue);
    }

    /**
        @brief Parses a given std::string into a value of the type long double and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, long double defvalue)
    {
        return string2Number(this->value_.value_long_double_, input, defvalue);
    }

    /**
        @brief Parses a given std::string into a value of the type bool and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, bool defvalue)
    {
        // Try to parse the value-string - is it a word?
        if (input.find("true") < input.size()
         || input.find("True") < input.size()
         || input.find("yes") < input.size()
         || input.find("Yes") < input.size())
            this->value_.value_bool_ = true;
        else if (input.find("false") < input.size()
              || input.find("False") < input.size()
              || input.find("no") < input.size()
              || input.find("No") < input.size())
            this->value_.value_bool_ = false;
        else
        {
            // Its not a known word - is it a number?
            return string2Number(this->value_.value_bool_, input, defvalue);
        }

        return true;
    }

    /**
        @brief Parses a given std::string into a value of the type std::string and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, const std::string& defvalue)
    {
        // Strip the quotes
        unsigned int pos1 = input.find("\"") + 1;
        unsigned int pos2 = input.find("\"", pos1);

        // Check if the entry was correctly quoted
        if (pos1 < input.length() && pos2 < input.length() && !(input.find("\"", pos2 + 1) < input.length()))
        {
            // It was - get the string between the quotes
            this->value_string_ = input.substr(pos1, pos2 - pos1);
            return true;
        }

        // It wasn't - use the default-value and restore the entry in the config-file.
        this->value_string_ = defvalue;
        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type const char* and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, const char* defvalue)
    {
        // Strip the quotes
        unsigned int pos1 = input.find("\"") + 1;
        unsigned int pos2 = input.find("\"", pos1);

        // Check if the entry was correctly quoted
        if (pos1 < input.length() && pos2 < input.length() && !(input.find("\"", pos2 + 1) < input.length()))
        {
            // It was - get the string between the quotes
            this->value_string_ = input.substr(pos1, pos2 - pos1);
            return true;
        }

        // It wasn't - use the default-value and restore the entry in the config-file.
        this->value_string_ = defvalue;
        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type Ogre::Vector2 and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, const Ogre::Vector2& defvalue)
    {
        // Strip the value-string
        unsigned int pos1 = input.find("(") + 1;
        unsigned int pos2 = input.find(")", pos1);

        // Try to convert the stripped value-string to Vector2
        if (pos1 < input.length() && pos2 < input.length() && pos1 < pos2)
        {
            std::vector<std::string> tokens = tokenize(input.substr(pos1, pos2 - pos1), ",");
            if (!string2Number(this->value_vector2_.x, tokens[0], defvalue.x))
            {
                this->value_vector2_ = defvalue;
                return false;
            }
            if (!string2Number(this->value_vector2_.y, tokens[1], defvalue.y))
            {
                this->value_vector2_ = defvalue;
                return false;
            }

            return true;
        }

        this->value_vector2_ = defvalue;
        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type Ogre::Vector3 and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, const Ogre::Vector3& defvalue)
    {
        // Strip the value-string
        unsigned int pos1 = input.find("(") + 1;
        unsigned int pos2 = input.find(")", pos1);

        // Try to convert the stripped value-string to Vector3
        if (pos1 < input.length() && pos2 < input.length() && pos1 < pos2)
        {
            std::vector<std::string> tokens = tokenize(input.substr(pos1, pos2 - pos1), ",");
            if (!string2Number(this->value_vector3_.x, tokens[0], defvalue.x))
            {
                this->value_vector3_ = defvalue;
                return false;
            }
            if (!string2Number(this->value_vector3_.y, tokens[1], defvalue.y))
            {
                this->value_vector3_ = defvalue;
                return false;
            }
            if (!string2Number(this->value_vector3_.z, tokens[2], defvalue.z))
            {
                this->value_vector3_ = defvalue;
                return false;
            }

            return true;
        }

        this->value_vector3_ = defvalue;
        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type Ogre::ColourValue and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseSting(const std::string& input, const Ogre::ColourValue& defvalue)
    {
        // Strip the value-string
        unsigned int pos1 = input.find("(") + 1;
        unsigned int pos2 = input.find(")", pos1);

        // Try to convert the stripped value-string to Vector3
        if (pos1 < input.length() && pos2 < input.length() && pos1 < pos2)
        {
            std::vector<std::string> tokens = tokenize(input.substr(pos1, pos2 - pos1), ",");
            if (!string2Number(this->value_colourvalue_.r, tokens[0], defvalue.r))
            {
                this->value_colourvalue_ = defvalue;
                return false;
            }
            if (!string2Number(this->value_colourvalue_.g, tokens[1], defvalue.g))
            {
                this->value_colourvalue_ = defvalue;
                return false;
            }
            if (!string2Number(this->value_colourvalue_.b, tokens[2], defvalue.b))
            {
                this->value_colourvalue_ = defvalue;
                return false;
            }
            if (!string2Number(this->value_colourvalue_.a, tokens[3], defvalue.a))
            {
                this->value_colourvalue_ = defvalue;
                return false;
            }

            return true;
        }

        this->value_colourvalue_ = defvalue;
        return false;
    }

    /**
        @brief Sets the corresponding entry in the config-file back to the default value.
    */
    void ConfigValueContainer::resetConfigFileEntry()
    {
        (*this->configFileLine_) = this->varname_ + "=" + this->defvalueString_;
        ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
    }

    /**
        @brief Sets the value of the variable back to the default value and resets the config-file entry.
    */
    void ConfigValueContainer::resetConfigValue()
    {
        this->parseSting(this->defvalueString_);
        this->resetConfigFileEntry();
    }

    /**
        @brief Searches the corresponding entry in the config-file and creates it, if there is no entry.
    */
    void ConfigValueContainer::searchConfigFileLine()
    {
        // Read the file if needed
        if (!ConfigValueContainer::finishedReadingConfigFile())
            ConfigValueContainer::readConfigFile(CONFIGFILEPATH);

        // The string of the section we're searching
        std::string section = "";
        section.append("[");
        section.append(this->classname_);
        section.append("]");

        // Iterate through all config-file-lines
        bool success = false;
        std::list<std::string>::iterator it1;
        for(it1 = ConfigValueContainer::getConfigFileLines().begin(); it1 != ConfigValueContainer::getConfigFileLines().end(); ++it1)
        {
            // Don't try to parse comments
            if (this->isComment(*it1))
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
                    if (this->isComment(*it2))
                        continue;

                    // This if-else block is used to write a new line right after the last line of the
                    // section but in front of the following empty lines before the next section.
                    // (So this helps to keep a nice formatting with empty-lines between sections in the config-file)
                    if (this->isEmpty(*it2))
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
            this->getConfigFileLines().push_back("[" + this->classname_ + "]");                   // Create the section
            this->getConfigFileLines().push_back(this->varname_ + "=" + this->defvalueString_);   // Create the line
            this->configFileLine_ = --this->getConfigFileLines().end();                           // Set the pointer to the last element
            success = true;
            this->getConfigFileLines().push_back("");                                             // Add an empty line - this is needed for the algorithm in the searchConfigFileLine-function
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);                              // Save the changed config-file
        }
    }

    /**
        @brief Determines if a line in the config-file is a comment.
        @param line The line to check
        @return True = it's a comment
    */
    bool ConfigValueContainer::isComment(const std::string& line)
    {
        // Strip the line, whitespaces are disturbing
        std::string teststring = getStrippedLine(line);

        // There are four possible comment-symbols:
        //  1) #comment in script-language style
        //  2) %comment in matlab style
        //  3) ;comment in unreal tournament config-file style
        //  4) //comment in code style
        if (teststring[0] == '#' || teststring[0] == '%' || teststring[0] == ';' || (teststring[0] == '/' && teststring[0] == '/'))
            return true;

        return false;
    }

    /**
        @brief Determines if a line in the config-file is empty (contains only whitespaces).
        @param line The line to check
        @return True = it's empty
    */
    bool ConfigValueContainer::isEmpty(const std::string& line)
    {
        return getStrippedLine(line) == "";
    }

    /**
        @brief Removes all whitespaces from a line.
        @param line The line to strip
        @return The stripped line
    */
    std::string ConfigValueContainer::getStrippedLine(const std::string& line)
    {
        std::string output = line;
        unsigned int pos;
        while ((pos = output.find(" ")) < output.length())
            output.erase(pos, 1);
        while ((pos = output.find("\t")) < output.length())
            output.erase(pos, 1);

        return output;
    }

    /**
        @brief Returns the part in the corresponding config-file-entry of the container that defines the value.
        @param bStripped True = strip the value-string
        @return The value-string
    */
    std::string ConfigValueContainer::parseValueString(bool bStripped)
    {
        std::string output;
        if (bStripped)
            output = this->getStrippedLine(*this->configFileLine_);
        else
            output = *this->configFileLine_;

        return output.substr(output.find("=") + 1);
    }

    /**
        @returns a list, containing all entrys in the config-file.
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
            COUT(1) << "Error: Couldn't open config-file " << filename << " to read the config values!" << std::endl;
            return;
        }

        char line[1024];

        // Iterate through the file and add the lines into the list
        while (file.good() && !file.eof())
        {
            file.getline(line, 1024);
            ConfigValueContainer::getConfigFileLines().push_back(line);
//            std::cout << "### ->" << line << "<- : empty: " << isEmpty(line) << " comment: " << isComment(line) << std::endl;
        }

        // The last line is useless
        ConfigValueContainer::getConfigFileLines().pop_back();

        // Add an empty line to the end of the file if needed
        // this is needed for the algorithm in the searchConfigFileLine-function
        if ((ConfigValueContainer::getConfigFileLines().size() > 0) && !isEmpty(*ConfigValueContainer::getConfigFileLines().rbegin()))
        {
//            std::cout << "### newline added" << std::endl;
            ConfigValueContainer::getConfigFileLines().push_back("");
        }

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

        if (!file.is_open())
        {
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
            this->description_ = std::string("ConfigValueDescription::" + this->classname_ + "::" + this->varname_);
            Language::getLanguage().addEntry(description_, description);
            this->bAddedDescription_ = true;
        }
    }
}
