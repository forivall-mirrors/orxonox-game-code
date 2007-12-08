#include <fstream>
#include <string>
#include "ConfigValueContainer.h"

#define CONFIGFILEPATH "O:\\oh\\bin\\orxonox.ini"

namespace orxonox
{
    std::list<std::string>* ConfigValueContainer::configFileLines_s = 0;
    bool ConfigValueContainer::readConfigFile_s = false;

    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, int defvalue)
    {
        std::ostringstream ostream;

        if (ostream << defvalue)
            this->defvalue_ = ostream.str();
        else
            this->defvalue_ = "0";

        this->setDefaultValues(classname, varname);
        std::string valueString = this->getValueString();

        std::istringstream istream(valueString);
        if (!(istream >> this->value_int_))
        {
            this->value_int_ = defvalue;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
    }

    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, double defvalue)
    {
        std::ostringstream ostream;

        if (ostream << defvalue)
            this->defvalue_ = ostream.str();
        else
            this->defvalue_ = "0.000000";

        this->setDefaultValues(classname, varname);
        std::string valueString = this->getValueString();

        std::istringstream istream(valueString);
        if (!(istream >> this->value_double_))
        {
            this->value_double_ = defvalue;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
    }

    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, bool defvalue)
    {
        if (defvalue)
            this->defvalue_ = "true";
        else
            this->defvalue_ = "false";

        this->setDefaultValues(classname, varname);
        std::string valueString = this->getValueString();

        if (valueString.find("true") < valueString.size() || valueString.find("yes") < valueString.size())
            this->value_bool_ = true;
        else if (valueString.find("false") < valueString.size() || valueString.find("no") < valueString.size())
            this->value_bool_ = false;
        else
        {
            std::istringstream istream(valueString);
            if (!(istream >> this->value_bool_))
            {
                this->value_bool_ = defvalue;
                (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
                ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
            }
        }
    }

    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, const char* defvalue)
    {
        this->defvalue_ = defvalue;
        this->setDefaultValues(classname, varname);
        this->value_string_ = this->getValueString(false);
    }

    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::Vector3 defvalue)
    {
        std::ostringstream ostream;
        if (ostream << "(" << defvalue.x << "," << defvalue.y << "," << defvalue.z << ")")
            this->defvalue_ = ostream.str();
        else
            this->defvalue_ = "(0,0,0)";

        this->setDefaultValues(classname, varname);
        std::string valueString = this->getValueString();

        unsigned int pos;
        while ((pos = valueString.find(" ")) < valueString.length())
            valueString.erase(pos, 1);
        while ((pos = valueString.find("(")) < valueString.length())
            valueString.erase(pos, 1);
        while ((pos = valueString.find(")")) < valueString.length())
            valueString.erase(pos, 1);
        while ((pos = valueString.find(",")) < valueString.length())
            valueString.replace(pos, 1, " ");

        std::istringstream istream(valueString);

        if (!(istream >> this->value_vector3_.x))
        {
            this->value_vector3_.x = defvalue.x;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
        if (!(istream >> this->value_vector3_.y))
        {
            this->value_vector3_.y = defvalue.y;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
        if (!(istream >> this->value_vector3_.z))
        {
            this->value_vector3_.z = defvalue.z;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
    }

    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::ColourValue defvalue)
    {
        std::ostringstream ostream;
        if (ostream << "(" << defvalue.r << "," << defvalue.g << "," << defvalue.b << "," << defvalue.a << ")")
            this->defvalue_ = ostream.str();
        else
            this->defvalue_ = "(0,0,0,0)";

        this->setDefaultValues(classname, varname);
        std::string valueString = this->getValueString();

        unsigned int pos;
        while ((pos = valueString.find(" ")) < valueString.length())
            valueString.erase(pos, 1);
        while ((pos = valueString.find("(")) < valueString.length())
            valueString.erase(pos, 1);
        while ((pos = valueString.find(")")) < valueString.length())
            valueString.erase(pos, 1);
        while ((pos = valueString.find(",")) < valueString.length())
            valueString.replace(pos, 1, " ");

        std::istringstream istream(valueString);

        if (!(istream >> this->value_colourvalue_.r))
        {
            this->value_colourvalue_.r = defvalue.r;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
        if (!(istream >> this->value_colourvalue_.g))
        {
            this->value_colourvalue_.g = defvalue.g;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
        if (!(istream >> this->value_colourvalue_.b))
        {
            this->value_colourvalue_.b = defvalue.b;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
        if (!(istream >> this->value_colourvalue_.a))
        {
            this->value_colourvalue_.a = defvalue.a;
            (*this->configFileLine_) = this->varname_ + "=" + this->defvalue_;
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
    }

    void ConfigValueContainer::setDefaultValues(const std::string& classname, const std::string& varname)
    {
        this->classname_ = classname;
        this->varname_ = varname;

        this->searchConfigFileLine();

        this->value_int_ = 0;
        this->value_double_ = 0.000000;
        this->value_bool_ = false;
        this->value_string_ = "";
        this->value_vector3_ = Ogre::Vector3(0, 0, 0);
        this->value_colourvalue_ = Ogre::ColourValue(0, 0, 0, 0);
    }

    void ConfigValueContainer::searchConfigFileLine()
    {
        if (!ConfigValueContainer::readConfigFile_s)
            ConfigValueContainer::readConfigFile(CONFIGFILEPATH);

        this->configFileLine_ = 0;

        std::string section = "";
        section.append("[");
        section.append(this->classname_);
        section.append("]");

        bool success = false;
        std::list<std::string>::iterator it1;
        for(it1 = ConfigValueContainer::configFileLines_s->begin(); it1 != ConfigValueContainer::configFileLines_s->end(); ++it1)
        {
            if (this->isComment(*it1))
                continue;

            if ((*it1).find(section) < (*it1).length())
            {
                bool bLineIsEmpty = false;
                std::list<std::string>::iterator positionToPutNewLineAt;

                std::list<std::string>::iterator it2;
                for(it2 = ++it1; it2 != ConfigValueContainer::configFileLines_s->end(); ++it2)
                {
                    if (this->isComment(*it2))
                        continue;

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

                    unsigned int open = (*it2).find("[");
                    unsigned int close = (*it2).find("]");
                    if ((open < (*it2).length()) && (close < (*it2).length()) && (open < close))
                    {
                        this->configFileLine_ = this->configFileLines_s->insert(positionToPutNewLineAt, this->varname_ + "=" + this->defvalue_);
                        ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
                        success = true;
                        break;
                    }

                    if ((*it2).find(this->varname_) < (*it2).length())
                    {
                        this->configFileLine_ = it2;
                        success = true;
                        break;
                    }
                }
                if (!success)
                {
                    this->configFileLine_ = this->configFileLines_s->insert(positionToPutNewLineAt, this->varname_ + "=" + this->defvalue_);
                    ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
                    success = true;
                }
                break;
            }
        }
        if (!success)
        {
            this->configFileLines_s->push_back("[" + this->classname_ + "]");
            this->configFileLines_s->push_back(this->varname_ + "=" + this->defvalue_);
            this->configFileLine_ = --this->configFileLines_s->end();
            success = true;
            this->configFileLines_s->push_back("");
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
        }
    }

    bool ConfigValueContainer::isComment(const std::string& line)
    {
        std::string teststring = getStrippedLine(line);

        if (teststring[0] == '#' || teststring[0] == '%' || teststring[0] == ';' || (teststring[0] == '/' && teststring[0] == '/'))
            return true;

        return false;
    }

    bool ConfigValueContainer::isEmpty(const std::string& line)
    {
        return getStrippedLine(line) == "";
    }

    std::string ConfigValueContainer::getStrippedLine(const std::string& line)
    {
        std::string output = line;
        unsigned int pos;
        while ((pos = output.find(" ")) < output.length())
            output.erase(pos, 1);

        return output;
    }

    std::string ConfigValueContainer::getValueString(bool bStripped)
    {
        std::string output;
        if (bStripped)
            output = this->getStrippedLine(*this->configFileLine_);
        else
            output = *this->configFileLine_;

        return output.substr(output.find("=") + 1);
    }

    void ConfigValueContainer::readConfigFile(const std::string& filename)
    {
        ConfigValueContainer::readConfigFile_s = true;

        if (!ConfigValueContainer::configFileLines_s)
            ConfigValueContainer::configFileLines_s = new std::list<std::string>;

        std::ofstream createFile;
        createFile.open(filename.c_str(), std::fstream::app);
        createFile.close();

        std::ifstream file;
        file.open(filename.c_str(), std::fstream::in);

        char line[1024];

        while (file.good() && !file.eof())
        {
            file.getline(line, 1024);
            ConfigValueContainer::configFileLines_s->push_back(line);
            std::cout << "### ->" << line << "<- : empty: " << isEmpty(line) << " comment: " << isComment(line) << std::endl;
        }

        ConfigValueContainer::configFileLines_s->pop_back();

        if ((ConfigValueContainer::configFileLines_s->size() > 0) && !isEmpty(*ConfigValueContainer::configFileLines_s->rbegin()))
        {
            std::cout << "### newline added" << std::endl;
            ConfigValueContainer::configFileLines_s->push_back("");
        }

        file.close();
    }

    void ConfigValueContainer::writeConfigFile(const std::string& filename)
    {
        ConfigValueContainer::readConfigFile_s = true;

        if (!ConfigValueContainer::configFileLines_s)
            ConfigValueContainer::configFileLines_s = new std::list<std::string>;

        std::ofstream createFile;
        createFile.open(filename.c_str(), std::fstream::app);
        createFile.close();

        std::ofstream file;
        file.open(filename.c_str(), std::fstream::out);

        std::list<std::string>::iterator it;
        for(it = ConfigValueContainer::configFileLines_s->begin(); it != ConfigValueContainer::configFileLines_s->end(); ++it)
        {
            file << (*it) << std::endl;
        }

        file.close();
    }
}
