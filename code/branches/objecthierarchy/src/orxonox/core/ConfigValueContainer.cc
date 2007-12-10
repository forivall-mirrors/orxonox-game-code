#include <fstream>
#include <string>
#include "ConfigValueContainer.h"

//#define CONFIGFILEPATH "O:\\oh\\bin\\orxonox.ini"
#define CONFIGFILEPATH "orxonox.ini"

namespace orxonox
{
    std::list<std::string>* ConfigValueContainer::configFileLines_s = 0; // Set the static member variable configFileLines_s to zero
    bool ConfigValueContainer::readConfigFile_s = false;                 // Set the static member variable readConfigFile_s to false

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets this->value_int_.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, int defvalue)
    {
        // Try to convert the default-value from int to string
        std::ostringstream ostream;
        if (ostream << defvalue)
            this->defvalue_ = ostream.str();
        else
            this->defvalue_ = "0";

        // Set the default values, then get the value-string
        this->setDefaultValues(classname, varname);
        std::string valueString = this->getValueString();

        // Try to convert the value-string to int
        std::istringstream istream(valueString);
        if (!(istream >> this->value_int_))
        {
            // The conversion failed - use the default value and restore the entry in the config-file
            this->value_int_ = defvalue;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets this->value_double_.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, double defvalue)
    {
        // Try to convert the default-value from double to string
        std::ostringstream ostream;
        if (ostream << defvalue)
            this->defvalue_ = ostream.str();
        else
            this->defvalue_ = "0.000000";

        // Set the default values, then get the value-string
        this->setDefaultValues(classname, varname);
        std::string valueString = this->getValueString();

        // Try to convert the value-string to double
        std::istringstream istream(valueString);
        if (!(istream >> this->value_double_))
        {
            // The conversion failed - use the default value and restore the entry in the config-file
            this->value_double_ = defvalue;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets this->value_bool_.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, bool defvalue)
    {
        // Convert the default-value from bool to string
        if (defvalue)
            this->defvalue_ = "true";
        else
            this->defvalue_ = "false";

        // Set the default values, then get the value-string
        this->setDefaultValues(classname, varname);
        std::string valueString = this->getValueString();

        // Try to parse the value-string - is it a word?
        if (valueString.find("true") < valueString.size() || valueString.find("yes") < valueString.size())
            this->value_bool_ = true;
        else if (valueString.find("false") < valueString.size() || valueString.find("no") < valueString.size())
            this->value_bool_ = false;
        else
        {
            // Its not a known word - is it a number?
            std::istringstream istream(valueString);
            if (!(istream >> this->value_bool_))
            {
                // The conversion failed - use the default value and restore the entry in the config-file
                this->value_bool_ = defvalue;
                (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
                ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
            }
        }
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets this->value_string_.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, const char* defvalue)
    {
        // Convert the string to a "config-file-string" with quotes
        this->defvalue_ = "\"" + std::string(defvalue) + "\"";

        // Set the default-values, then get the value-string
        this->setDefaultValues(classname, varname);
        std::string valueString = this->getValueString(false);

        // Strip the quotes
        unsigned int pos1 = valueString.find("\"") + 1;
        unsigned int pos2 = valueString.find("\"", pos1);

        // Check if the entry was correctly quoted
        if (pos1 < valueString.length() && pos2 < valueString.length() && !(valueString.find("\"", pos2 + 1) < valueString.length()))
        {
            // It was - get the string between the quotes
            valueString = valueString.substr(pos1, pos2 - pos1);
            this->value_string_ = valueString;
        }
        else
        {
            // It wasn't - use the default-value and restore the entry in the config-file.
            this->value_string_ = defvalue;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets this->value_vector2_.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::Vector2 defvalue)
    {
        // Try to convert the default-value from Vector2 to string
        std::ostringstream ostream;
        if (ostream << "(" << defvalue.x << "," << defvalue.y << ")")
            this->defvalue_ = ostream.str();
        else
            this->defvalue_ = "(0,0)";

        // Set the default values, then get the value-string
        this->setDefaultValues(classname, varname);
        std::string valueString = this->getValueString();

        // Strip the value-string
        bool bEntryIsCorrupt = false;
        valueString = this->getStrippedLine(valueString);
        unsigned int pos1, pos2, pos3;
        pos1 = valueString.find("(");
        if (pos1 == 0)
            valueString.erase(pos1, 1);
        else
            bEntryIsCorrupt = true;

        pos2 = valueString.find(")");
        if (pos2 == valueString.length() - 1)
            valueString.erase(pos2, 1);
        else
            bEntryIsCorrupt = true;

        int count = 0;
        while ((pos3 = valueString.find(",")) < valueString.length())
        {
            count++;
            valueString.replace(pos3, 1, " ");
            if (pos3 < pos1)
                bEntryIsCorrupt = true;
        }

        if (count != 1)
            bEntryIsCorrupt = true;

        // Try to convert the stripped value-string to Vector2
        if (!bEntryIsCorrupt)
        {
            std::istringstream istream(valueString);
            if (!(istream >> this->value_vector2_.x))
            {
                // The conversion failed - use the default value and restore the entry in the config-file
                this->value_vector2_.x = defvalue.x;
                (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
                ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
            }
            if (!(istream >> this->value_vector2_.y))
            {
                // The conversion failed - use the default value and restore the entry in the config-file
                this->value_vector2_.y = defvalue.y;
                (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
                ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
            }
        }
        else
        {
            // The conversion failed - use the default value and restore the entry in the config-file
            this->value_vector2_ = defvalue;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets this->value_vector3_.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::Vector3 defvalue)
    {
        // Try to convert the default-value from Vector3 to string
        std::ostringstream ostream;
        if (ostream << "(" << defvalue.x << "," << defvalue.y << "," << defvalue.z << ")")
            this->defvalue_ = ostream.str();
        else
            this->defvalue_ = "(0,0,0)";

        // Set the default values, then get the value-string
        this->setDefaultValues(classname, varname);
        std::string valueString = this->getValueString();

        // Strip the value-string
        bool bEntryIsCorrupt = false;
        valueString = this->getStrippedLine(valueString);
        unsigned int pos1, pos2, pos3;
        pos1 = valueString.find("(");
        if (pos1 == 0)
            valueString.erase(pos1, 1);
        else
            bEntryIsCorrupt = true;

        pos2 = valueString.find(")");
        if (pos2 == valueString.length() - 1)
            valueString.erase(pos2, 1);
        else
            bEntryIsCorrupt = true;

        int count = 0;
        while ((pos3 = valueString.find(",")) < valueString.length())
        {
            count++;
            valueString.replace(pos3, 1, " ");
            if (pos3 < pos1)
                bEntryIsCorrupt = true;
        }

        if (count != 2)
            bEntryIsCorrupt = true;

        // Try to convert the stripped value-string to Vector3
        if (!bEntryIsCorrupt)
        {
            std::istringstream istream(valueString);
            if (!(istream >> this->value_vector3_.x))
            {
                // The conversion failed - use the default value and restore the entry in the config-file
                this->value_vector3_.x = defvalue.x;
                (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
                ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
            }
            if (!(istream >> this->value_vector3_.y))
            {
                // The conversion failed - use the default value and restore the entry in the config-file
                this->value_vector3_.y = defvalue.y;
                (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
                ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
            }
            if (!(istream >> this->value_vector3_.z))
            {
                // The conversion failed - use the default value and restore the entry in the config-file
                this->value_vector3_.z = defvalue.z;
                (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
                ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
            }
        }
        else
        {
            // The conversion failed - use the default value and restore the entry in the config-file
            this->value_vector3_ = defvalue;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets this->value_colourvalue_.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::ColourValue defvalue)
    {
        // Try to convert the default-value from ColourValue to string
        std::ostringstream ostream;
        if (ostream << "(" << defvalue.r << "," << defvalue.g << "," << defvalue.b << "," << defvalue.a << ")")
            this->defvalue_ = ostream.str();
        else
            this->defvalue_ = "(0,0,0,0)";

        // Set the default values, then get the value-string
        this->setDefaultValues(classname, varname);
        std::string valueString = this->getValueString();

        // Strip the value-string
        bool bEntryIsCorrupt = false;
        valueString = this->getStrippedLine(valueString);
        unsigned int pos1, pos2, pos3;
        pos1 = valueString.find("(");
        if (pos1 == 0)
            valueString.erase(pos1, 1);
        else
            bEntryIsCorrupt = true;

        pos2 = valueString.find(")");
        if (pos2 == valueString.length() - 1)
            valueString.erase(pos2, 1);
        else
            bEntryIsCorrupt = true;

        int count = 0;
        while ((pos3 = valueString.find(",")) < valueString.length())
        {
            count++;
            valueString.replace(pos3, 1, " ");
            if (pos3 < pos1)
                bEntryIsCorrupt = true;
        }

        if (count != 3)
            bEntryIsCorrupt = true;

        // Try to convert the stripped value-string to Vector3
        if (!bEntryIsCorrupt)
        {
            std::istringstream istream(valueString);
            if (!(istream >> this->value_colourvalue_.r))
            {
                // The conversion failed - use the default value and restore the entry in the config-file
                this->value_colourvalue_.r = defvalue.r;
                (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
                ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
            }
            if (!(istream >> this->value_colourvalue_.g))
            {
                // The conversion failed - use the default value and restore the entry in the config-file
                this->value_colourvalue_.g = defvalue.g;
                (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
                ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
            }
            if (!(istream >> this->value_colourvalue_.b))
            {
                // The conversion failed - use the default value and restore the entry in the config-file
                this->value_colourvalue_.b = defvalue.b;
                (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
                ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
            }
            if (!(istream >> this->value_colourvalue_.a))
            {
                // The conversion failed - use the default value and restore the entry in the config-file
                this->value_colourvalue_.a = defvalue.a;
                (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
                ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
            }
        }
        else
        {
            // The conversion failed - use the default value and restore the entry in the config-file
            this->value_colourvalue_ = defvalue;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
    }

    /**
        @brief Sets the default values of the container and searches the coresponding entry in the config-file.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
    */
    void ConfigValueContainer::setDefaultValues(const std::string& classname, const std::string& varname)
    {
        // Set the class and variable names
        this->classname_ = classname;
        this->varname_ = varname;

        // Search the entry in the config-file
        this->searchConfigFileLine();

        // Set the values of all types to zero
        this->value_int_ = 0;
        this->value_double_ = 0.000000;
        this->value_bool_ = false;
        this->value_string_ = "";
        this->value_vector2_ = Ogre::Vector2(0, 0);
        this->value_vector3_ = Ogre::Vector3(0, 0, 0);
        this->value_colourvalue_ = Ogre::ColourValue(0, 0, 0, 0);
    }

    /**
        @brief Searches the corresponding entry in the config-file and creates it, if there is no entry.
    */
    void ConfigValueContainer::searchConfigFileLine()
    {
        // Read the file if needed
        if (!ConfigValueContainer::readConfigFile_s)
            ConfigValueContainer::readConfigFile(CONFIGFILEPATH);

        // Just in case something goes wrong
        this->configFileLine_ = 0;

        // The string of the section we're searching
        std::string section = "";
        section.append("[");
        section.append(this->classname_);
        section.append("]");

        // Iterate through all config-file-lines
        bool success = false;
        std::list<std::string>::iterator it1;
        for(it1 = ConfigValueContainer::configFileLines_s->begin(); it1 != ConfigValueContainer::configFileLines_s->end(); ++it1)
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
                for(it2 = ++it1; it2 != ConfigValueContainer::configFileLines_s->end(); ++it2)
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
                        this->configFileLine_ = this->configFileLines_s->insert(positionToPutNewLineAt, this->varname_ + "=" + this->defvalue_);
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
                    this->configFileLine_ = this->configFileLines_s->insert(positionToPutNewLineAt, this->varname_ + "=" + this->defvalue_);
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
            this->configFileLines_s->push_back("[" + this->classname_ + "]");           // Create the section
            this->configFileLines_s->push_back(this->varname_ + "=" + this->defvalue_); // Create the line
            this->configFileLine_ = --this->configFileLines_s->end();                   // Set the pointer to the last element
            success = true;
            this->configFileLines_s->push_back("");                                     // Add an empty line - this is needed for the algorithm in the searchConfigFileLine-function
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);                      // Save the changed config-file
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
    std::string ConfigValueContainer::getValueString(bool bStripped)
    {
        std::string output;
        if (bStripped)
            output = this->getStrippedLine(*this->configFileLine_);
        else
            output = *this->configFileLine_;

        return output.substr(output.find("=") + 1);
    }

    /**
        @brief Reads the config-file and stores the lines in a list.
        @param filename The name of the config-file
    */
    void ConfigValueContainer::readConfigFile(const std::string& filename)
    {
        ConfigValueContainer::readConfigFile_s = true;

        // Create the list if needed
        if (!ConfigValueContainer::configFileLines_s)
            ConfigValueContainer::configFileLines_s = new std::list<std::string>;

        // This creates the file if it's not existing
        std::ofstream createFile;
        createFile.open(filename.c_str(), std::fstream::app);
        createFile.close();

        // Open the file
        std::ifstream file;
        file.open(filename.c_str(), std::fstream::in);

        char line[1024];

        // Iterate through the file and add the lines into the list
        while (file.good() && !file.eof())
        {
            file.getline(line, 1024);
            ConfigValueContainer::configFileLines_s->push_back(line);
//            std::cout << "### ->" << line << "<- : empty: " << isEmpty(line) << " comment: " << isComment(line) << std::endl;
        }

        // The last line is useless
        ConfigValueContainer::configFileLines_s->pop_back();

        // Add an empty line to the end of the file if needed
        // this is needed for the algorithm in the searchConfigFileLine-function
        if ((ConfigValueContainer::configFileLines_s->size() > 0) && !isEmpty(*ConfigValueContainer::configFileLines_s->rbegin()))
        {
//            std::cout << "### newline added" << std::endl;
            ConfigValueContainer::configFileLines_s->push_back("");
        }

        file.close();
    }

    /**
        @param Writes the content of the list, containing all lines of the config-file, into the config-file.
        @param filename The name of the config-file
    */
    void ConfigValueContainer::writeConfigFile(const std::string& filename)
    {
        // Make sure we stored the config-file in the list
        if (!ConfigValueContainer::readConfigFile_s)
            ConfigValueContainer::readConfigFile(filename);

        // Open the file
        std::ofstream file;
        file.open(filename.c_str(), std::fstream::out);

        // Iterate through the list an write the lines into the file
        std::list<std::string>::iterator it;
        for(it = ConfigValueContainer::configFileLines_s->begin(); it != ConfigValueContainer::configFileLines_s->end(); ++it)
        {
            file << (*it) << std::endl;
        }

        file.close();
    }
}
