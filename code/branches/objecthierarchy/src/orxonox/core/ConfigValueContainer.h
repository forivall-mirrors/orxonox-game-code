#ifndef _ConfigValueContainer_H__
#define _ConfigValueContainer_H__

#include <string>
#include <list>
#include "OgreVector3.h"
#include "OgreColourValue.h"

namespace orxonox
{
    class ConfigValueContainer
    {
        public:
            ConfigValueContainer(const std::string& classname, const std::string& varname, int defvalue);
            ConfigValueContainer(const std::string& classname, const std::string& varname, double defvalue);
            ConfigValueContainer(const std::string& classname, const std::string& varname, bool defvalue);
            ConfigValueContainer(const std::string& classname, const std::string& varname, const char* defvalue);
            ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::Vector3 defvalue);
            ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::ColourValue defvalue);

            void setDefaultValues(const std::string& classname, const std::string& varname);
            void searchConfigFileLine();
            std::string getValueString(bool bStripped = true);

            static std::string getStrippedLine(const std::string& line);
            static bool isEmpty(const std::string& line);
            static bool isComment(const std::string& line);
            static void readConfigFile(const std::string& filename);
            static void writeConfigFile(const std::string& filename);

            inline int getValue(int value)                                      { return this->value_int_; }
            inline double getValue(double value)                                { return this->value_double_; }
            inline bool getValue(bool value)                                    { return this->value_bool_; }
            inline std::string getValue(const std::string& value)               { return this->value_string_; }
            inline Ogre::Vector3 getValue(const Ogre::Vector3& value)           { return this->value_vector3_; }
            inline Ogre::ColourValue getValue(const Ogre::ColourValue& value)   { return this->value_colourvalue_; }

        private:
            std::string         classname_;
            std::string         varname_;
            std::string         defvalue_;

            int                 value_int_;
            double              value_double_;
            bool                value_bool_;
            std::string         value_string_;
            Ogre::Vector3       value_vector3_;
            Ogre::ColourValue   value_colourvalue_;

            std::list<std::string>::iterator configFileLine_;
            static std::list<std::string>* configFileLines_s;
            static bool readConfigFile_s;
    };
}

#endif
