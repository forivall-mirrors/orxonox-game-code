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

#include "ConfigValueContainer.h"
#include "util/Tokenizer.h"
#include "util/Convert.h"

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
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, MultiTypeMath defvalue)
    {
        this->bAddedDescription_ = false;
        this->classname_ = classname;
        this->varname_ = varname;

        this->valueToString(&this->defvalueString_, defvalue);                      // Try to convert the default-value to a string
        this->searchConfigFileLine();                                               // Search the entry in the config-file

        std::string valueString = this->parseValueString(!(defvalue.isA(MT_string) || defvalue.isA(MT_constchar)));     // Parses the value string from the config-file-entry
        if (!this->parseString(valueString, defvalue))                              // Try to convert the string to a value
            this->resetConfigFileEntry();                                           // The conversion failed
    }

    /**
        @brief Converts a value to a string.
        @param output The string to write to
        @param input The value to convert
        @return True if the converson was successful
    */
    bool ConfigValueContainer::valueToString(std::string* output, MultiTypeMath& input)
    {
        if (input.getType() == MT_int)
            return ConvertValue(output, input.getInt(), std::string("0"));
        else if (input.getType() == MT_uint)
            return ConvertValue(output, input.getUnsignedInt(), std::string("0"));
        else if (input.getType() == MT_char)
            return ConvertValue(output, (int)input.getChar(), std::string("0"));
        else if (input.getType() == MT_uchar)
            return ConvertValue(output, (unsigned int)input.getUnsignedChar(), std::string("0"));
        else if (input.getType() == MT_short)
            return ConvertValue(output, input.getShort(), std::string("0"));
        else if (input.getType() == MT_ushort)
            return ConvertValue(output, input.getUnsignedShort(), std::string("0"));
        else if (input.getType() == MT_long)
            return ConvertValue(output, input.getLong(), std::string("0"));
        else if (input.getType() == MT_ulong)
            return ConvertValue(output, input.getUnsignedLong(), std::string("0"));
        else if (input.getType() == MT_float)
            return ConvertValue(output, input.getFloat(), std::string("0.000000"));
        else if (input.getType() == MT_double)
            return ConvertValue(output, input.getDouble(), std::string("0.000000"));
        else if (input.getType() == MT_longdouble)
            return ConvertValue(output, input.getChar(), std::string("0.000000"));
        else if (input.getType() == MT_bool)
        {
            if (input.getBool())
                (*output) = "true";
            else
                (*output) = "false";

            return true;
        }
        else if (input.getType() == MT_constchar)
        {
            (*output) = "\"" + input.getString() + "\"";
            return true;
        }
        else if (input.getType() == MT_string)
        {
            (*output) = "\"" + input.getString() + "\"";
            return true;
        }
        else if (input.getType() == MT_vector2)
        {
            std::ostringstream ostream;
            if (ostream << "(" << input.getVector2().x << "," << input.getVector2().y << ")")
            {
                (*output) = ostream.str();
                return true;
            }
            else
            {
                (*output) = "(0,0)";
                return false;
            }
        }
        else if (input.getType() == MT_vector3)
        {
            std::ostringstream ostream;
            if (ostream << "(" << input.getVector3().x << "," << input.getVector3().y << "," << input.getVector3().z << ")")
            {
                (*output) = ostream.str();
                return true;
            }
            else
            {
                (*output) = "(0,0,0)";
                return false;
            }
        }
        else if (input.getType() == MT_colourvalue)
        {
            std::ostringstream ostream;
            if (ostream << "(" << input.getColourValue().r << "," << input.getColourValue().g << "," << input.getColourValue().b << "," << input.getColourValue().a << ")")
            {
                (*output) = ostream.str();
                return true;
            }
            else
            {
                (*output) = "(0,0,0,0)";
                return false;
            }
        }
        else if (input.getType() == MT_quaternion)
        {
            std::ostringstream ostream;
            if (ostream << "(" << input.getQuaternion().w << "," << input.getQuaternion().x << "," << input.getQuaternion().y << "," << input.getQuaternion().z << ")")
            {
                (*output) = ostream.str();
                return true;
            }
            else
            {
                (*output) = "(0,0,0,0)";
                return false;
            }
        }
        else if (input.getType() == MT_radian)
            return ConvertValue(output, input.getRadian(), std::string("0.000000"));
        else if (input.getType() == MT_degree)
            return ConvertValue(output, input.getDegree(), std::string("0.000000"));

        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type of the associated variable and assigns it.
        @param input The string to convert
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, MultiTypeMath& defvalue)
    {
        if (defvalue.getType() == MT_int)
            return this->parseString(input, defvalue.getInt());
        else if (defvalue.getType() == MT_uint)
            return this->parseString(input, defvalue.getUnsignedInt());
        else if (defvalue.getType() == MT_char)
            return this->parseString(input, defvalue.getChar());
        else if (defvalue.getType() == MT_uchar)
            return this->parseString(input, defvalue.getUnsignedChar());
        else if (defvalue.getType() == MT_short)
            return this->parseString(input, defvalue.getShort());
        else if (defvalue.getType() == MT_ushort)
            return this->parseString(input, defvalue.getUnsignedShort());
        else if (defvalue.getType() == MT_long)
            return this->parseString(input, defvalue.getLong());
        else if (defvalue.getType() == MT_ulong)
            return this->parseString(input, defvalue.getUnsignedLong());
        else if (defvalue.getType() == MT_float)
            return this->parseString(input, defvalue.getFloat());
        else if (defvalue.getType() == MT_double)
            return this->parseString(input, defvalue.getDouble());
        else if (defvalue.getType() == MT_longdouble)
            return this->parseString(input, defvalue.getLongDouble());
        else if (defvalue.getType() == MT_bool)
            return this->parseString(input, defvalue.getBool());
        else if (defvalue.getType() == MT_constchar)
            return this->parseString(input, defvalue.getString());
        else if (defvalue.getType() == MT_string)
            return this->parseString(input, defvalue.getString());
        else if (defvalue.getType() == MT_vector2)
            return this->parseString(input, defvalue.getVector2());
        else if (defvalue.getType() == MT_vector3)
            return this->parseString(input, defvalue.getVector3());
        else if (defvalue.getType() == MT_colourvalue)
            return this->parseString(input, defvalue.getColourValue());
        else if (defvalue.getType() == MT_quaternion)
            return this->parseString(input, defvalue.getQuaternion());
        else if (defvalue.getType() == MT_radian)
            return this->parseString(input, defvalue.getRadian());
        else if (defvalue.getType() == MT_degree)
            return this->parseString(input, defvalue.getDegree());

        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type int and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, int defvalue)
    {
        int temp;
        bool success = ConvertValue(&temp, input, defvalue);
        this->value_.setValue(temp);
        return success;
    }

    /**
        @brief Parses a given std::string into a value of the type unsigned int and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, unsigned int defvalue)
    {
        unsigned int temp;
        bool success = ConvertValue(&temp, input, defvalue);
        this->value_.setValue(temp);
        return success;
    }

    /**
        @brief Parses a given std::string into a value of the type char and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, char defvalue)
    {
        // I used value_int_ instead of value_char_ to avoid number <-> char confusion in the config-file
        int temp;
        bool success = ConvertValue(&temp, input, (int)defvalue);
        this->value_.setValue((char)temp);
        return success;
    }

    /**
        @brief Parses a given std::string into a value of the type unsigned char and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, unsigned char defvalue)
    {
        // I used value_uint_ instead of value_uchar_ to avoid number <-> char confusion in the config-file
        unsigned int temp;
        bool success = ConvertValue(&temp, input, (unsigned int)defvalue);
        this->value_.setValue((unsigned char)temp);
        return success;
    }

    /**
        @brief Parses a given std::string into a value of the type short and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, short defvalue)
    {
        short temp;
        bool success = ConvertValue(&temp, input, defvalue);
        this->value_.setValue(temp);
        return success;
    }

    /**
        @brief Parses a given std::string into a value of the type unsigned short and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, unsigned short defvalue)
    {
        unsigned short temp;
        bool success = ConvertValue(&temp, input, defvalue);
        this->value_.setValue(temp);
        return success;
    }

    /**
        @brief Parses a given std::string into a value of the type long and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, long defvalue)
    {
        long temp;
        bool success = ConvertValue(&temp, input, defvalue);
        this->value_.setValue(temp);
        return success;
    }

    /**
        @brief Parses a given std::string into a value of the type unsigned long and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, unsigned long defvalue)
    {
        unsigned long temp;
        bool success = ConvertValue(&temp, input, defvalue);
        this->value_.setValue(temp);
        return success;
    }

    /**
        @brief Parses a given std::string into a value of the type float and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, float defvalue)
    {
        float temp;
        bool success = ConvertValue(&temp, input, defvalue);
        this->value_.setValue(temp);
        return success;
    }

    /**
        @brief Parses a given std::string into a value of the type double and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, double defvalue)
    {
        double temp;
        bool success = ConvertValue(&temp, input, defvalue);
        this->value_.setValue(temp);
        return success;
    }

    /**
        @brief Parses a given std::string into a value of the type long double and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, long double defvalue)
    {
        long double temp;
        bool success = ConvertValue(&temp, input, defvalue);
        this->value_.setValue(temp);
        return success;
    }

    /**
        @brief Parses a given std::string into a value of the type bool and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, bool defvalue)
    {
        // Try to parse the value-string - is it a word?
        if (input.find("true") < input.size()
         || input.find("True") < input.size()
         || input.find("yes") < input.size()
         || input.find("Yes") < input.size())
            this->value_.setValue(true);
        else if (input.find("false") < input.size()
              || input.find("False") < input.size()
              || input.find("no") < input.size()
              || input.find("No") < input.size())
            this->value_.setValue(false);
        else
        {
            // Its not a known word - is it a number?
            bool temp;
            bool success = ConvertValue(&temp, input, defvalue);
            this->value_.setValue(temp);
            return success;
        }

        return true;
    }

    /**
        @brief Parses a given std::string into a value of the type std::string and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, const std::string& defvalue)
    {
        // Strip the quotes
        unsigned int pos1 = input.find("\"") + 1;
        unsigned int pos2 = input.find("\"", pos1);

        // Check if the entry was correctly quoted
        if (pos1 < input.length() && pos2 < input.length() && !(input.find("\"", pos2 + 1) < input.length()))
        {
            // It was - get the string between the quotes
            this->value_.setValue(input.substr(pos1, pos2 - pos1));
            return true;
        }

        // It wasn't - use the default-value and restore the entry in the config-file.
        this->value_.setValue(defvalue);
        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type const char* and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, const char* defvalue)
    {
        // Strip the quotes
        unsigned int pos1 = input.find("\"") + 1;
        unsigned int pos2 = input.find("\"", pos1);

        // Check if the entry was correctly quoted
        if (pos1 < input.length() && pos2 < input.length() && !(input.find("\"", pos2 + 1) < input.length()))
        {
            // It was - get the string between the quotes
            this->value_.setValue(input.substr(pos1, pos2 - pos1));
            return true;
        }

        // It wasn't - use the default-value and restore the entry in the config-file.
        this->value_.setValue(defvalue);
        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type _Vector2 and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, const Vector2& defvalue)
    {
        // Strip the value-string
        unsigned int pos1 = input.find("(") + 1;
        unsigned int pos2 = input.find(")", pos1);

        // Try to convert the stripped value-string to Vector2
        if (pos1 < input.length() && pos2 < input.length() && pos1 < pos2)
        {
            std::vector<std::string> tokens = tokenize(input.substr(pos1, pos2 - pos1), ",");
            if (!ConvertValue(&this->value_.getVector2().x, tokens[0]))
            {
                this->value_.setValue(defvalue);
                return false;
            }
            if (!ConvertValue(&this->value_.getVector2().y, tokens[1]))
            {
                this->value_.setValue(defvalue);
                return false;
            }

            return true;
        }

        this->value_.setValue(defvalue);
        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type Vector3 and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, const Vector3& defvalue)
    {
        // Strip the value-string
        unsigned int pos1 = input.find("(") + 1;
        unsigned int pos2 = input.find(")", pos1);

        // Try to convert the stripped value-string to Vector3
        if (pos1 < input.length() && pos2 < input.length() && pos1 < pos2)
        {
            std::vector<std::string> tokens = tokenize(input.substr(pos1, pos2 - pos1), ",");
            if (!ConvertValue(&this->value_.getVector3().x, tokens[0]))
            {
                this->value_.setValue(defvalue);
                return false;
            }
            if (!ConvertValue(&this->value_.getVector3().y, tokens[1]))
            {
                this->value_.setValue(defvalue);
                return false;
            }
            if (!ConvertValue(&this->value_.getVector3().z, tokens[2]))
            {
                this->value_.setValue(defvalue);
                return false;
            }

            return true;
        }

        this->value_.setValue(defvalue);
        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type ColourValue and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, const ColourValue& defvalue)
    {
        // Strip the value-string
        unsigned int pos1 = input.find("(") + 1;
        unsigned int pos2 = input.find(")", pos1);

        // Try to convert the stripped value-string to Vector3
        if (pos1 < input.length() && pos2 < input.length() && pos1 < pos2)
        {
            std::vector<std::string> tokens = tokenize(input.substr(pos1, pos2 - pos1), ",");
            if (!ConvertValue(&this->value_.getColourValue().r, tokens[0]))
            {
                this->value_.setValue(defvalue);
                return false;
            }
            if (!ConvertValue(&this->value_.getColourValue().g, tokens[1]))
            {
                this->value_.setValue(defvalue);
                return false;
            }
            if (!ConvertValue(&this->value_.getColourValue().b, tokens[2]))
            {
                this->value_.setValue(defvalue);
                return false;
            }
            if (!ConvertValue(&this->value_.getColourValue().a, tokens[3]))
            {
                this->value_.setValue(defvalue);
                return false;
            }

            return true;
        }

        this->value_.setValue(defvalue);
        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type Quaternion and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, const Quaternion& defvalue)
    {
        // Strip the value-string
        unsigned int pos1 = input.find("(") + 1;
        unsigned int pos2 = input.find(")", pos1);

        // Try to convert the stripped value-string to Vector3
        if (pos1 < input.length() && pos2 < input.length() && pos1 < pos2)
        {
            std::vector<std::string> tokens = tokenize(input.substr(pos1, pos2 - pos1), ",");
            if (!ConvertValue(&this->value_.getQuaternion().w, tokens[0]))
            {
                this->value_.setValue(defvalue);
                return false;
            }
            if (!ConvertValue(&this->value_.getQuaternion().x, tokens[1]))
            {
                this->value_.setValue(defvalue);
                return false;
            }
            if (!ConvertValue(&this->value_.getQuaternion().y, tokens[2]))
            {
                this->value_.setValue(defvalue);
                return false;
            }
            if (!ConvertValue(&this->value_.getQuaternion().z, tokens[3]))
            {
                this->value_.setValue(defvalue);
                return false;
            }

            return true;
        }

        this->value_.setValue(defvalue);
        return false;
    }

    /**
        @brief Parses a given std::string into a value of the type long double and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, const Radian& defvalue)
    {
        return ConvertValue(&this->value_.getRadian(), input, defvalue);
    }

    /**
        @brief Parses a given std::string into a value of the type long double and assigns it to the right variable. If the conversion failed, the default-value gets assigned.
        @param input The string to convert
        @param defvalue The default-value
        @return True if the string was successfully parsed
    */
    bool ConfigValueContainer::parseString(const std::string& input, const Degree& defvalue)
    {
        return ConvertValue(&this->value_.getDegree(), input, defvalue);
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
        this->parseString(this->defvalueString_, this->value_);
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
            COUT(1) << "An error occurred in ConfigValueContainer:" << std::endl;
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
            COUT(1) << "An error occurred in ConfigValueContainer:" << std::endl;
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
            Language::getLanguage().addEntry(this->description_, description);
            this->bAddedDescription_ = true;
        }
    }
}
