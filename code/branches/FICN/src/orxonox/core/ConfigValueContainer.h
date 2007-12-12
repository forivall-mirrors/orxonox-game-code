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

#include <string>
#include <list>

#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"

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
    class ConfigValueContainer
    {
        public:
            ConfigValueContainer(const std::string& classname, const std::string& varname, int defvalue);
            ConfigValueContainer(const std::string& classname, const std::string& varname, double defvalue);
            ConfigValueContainer(const std::string& classname, const std::string& varname, bool defvalue);
            ConfigValueContainer(const std::string& classname, const std::string& varname, const char* defvalue);
            ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::Vector2 defvalue);
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

            /** @returns the value of the type int. @param value This is only needed to determine the right type. */
            inline int getValue(int value)                                      { return this->value_.value_int_; }
            /** @returns the value of the type double. @param value This is only needed to determine the right type. */
            inline double getValue(double value)                                { return this->value_.value_double_; }
            /** @returns the value of the type bool. @param value This is only needed to determine the right type. */
            inline bool getValue(bool value)                                    { return this->value_.value_bool_; }
            /** @returns the value of the type std::string. @param value This is only needed to determine the right type. */
            inline std::string getValue(const std::string& value)               { return this->value_string_; }
            /** @returns the value of the type Vector2. @param value This is only needed to determine the right type. */
            inline Ogre::Vector2 getValue(const Ogre::Vector2& value)           { return this->value_vector2_; }
            /** @returns the value of the type Vector3. @param value This is only needed to determine the right type. */
            inline Ogre::Vector3 getValue(const Ogre::Vector3& value)           { return this->value_vector3_; }
            /** @returns the value of the type Colour£Value. @param value This is only needed to determine the right type. */
            inline Ogre::ColourValue getValue(const Ogre::ColourValue& value)   { return this->value_colourvalue_; }

        private:
            std::string         classname_;                     //!< The name of the class the variable belongs to
            std::string         varname_;                       //!< The name of the variable
            std::string         defvalue_;                      //!< The string of the default-variable

            union MultiType
            {
                int                 value_int_;                 //!< The value, if the variable is of the type int
                double              value_double_;              //!< The value, if the variable is of the type double
                bool                value_bool_;                //!< The value, if the variable is of the type bool
            } value_;

            std::string         value_string_;                  //!< The value, if the variable is of the type string
            Ogre::Vector2       value_vector2_;                 //!< The value, if the variable is of the type Vector2
            Ogre::Vector3       value_vector3_;                 //!< The value, if the variable is of the type Vector3
            Ogre::ColourValue   value_colourvalue_;             //!< The value, if the variable is of the type ColourValue

            std::list<std::string>::iterator configFileLine_;   //!< An iterator, pointing to the entry of the variable in the config-file
            static std::list<std::string>* configFileLines_s;   //!< A list, containing all entrys in the config-file
            static bool readConfigFile_s;                       //!< True if the config-file is read and stored in the list
    };
}

#endif
