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
#include "util/MultiType.h"
#include "ConfigFileManager.h"
#include "Identifier.h"

namespace orxonox
{
    class ConfigValueCallbackBase
    {
        public:
            virtual void call(void* object) = 0;
            inline virtual ~ConfigValueCallbackBase() {}
    };

    template <class T>
    class ConfigValueCallback: public ConfigValueCallbackBase
    {
        public:
            inline ConfigValueCallback(void (T::*function) (void)) : function_(function) {}
            inline virtual ~ConfigValueCallback() {}
            inline virtual void call(void* object)
                { if (!Identifier::isCreatingHierarchy()) { (((T*)object)->*this->function_)(); } }

        private:
            void (T::*function_) (void);
    };


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
            /**
                @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
                @param type The type of the corresponding config-file
                @param identifier The identifier of the class the variable belongs to
                @param sectionname Name of the section the configValue should be put in.
                @param varname The name of the variable
                @param defvalue The default-value
                @param value Only needed do determine the right type.
            */
            template <class D, class V>
            ConfigValueContainer(ConfigFileType type, Identifier* identifier, const std::string& sectionname, const std::string& varname, const D& defvalue, const V& value)
            {
                this->init(type, identifier, sectionname, varname);
                this->initValue((V)defvalue);
            }

            /**
                @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
                @param type The type of the corresponding config-file
                @param identifier The identifier of the class the variable belongs to
                @param varname The name of the variable
                @param defvalue The default-value
            */
            template <class V>
            ConfigValueContainer(ConfigFileType type, Identifier* identifier, const std::string& sectionname, const std::string& varname, const std::vector<V>& defvalue)
            {
                this->init(type, identifier, sectionname, varname);

                this->value_ = V();
                for (unsigned int i = 0; i < defvalue.size(); i++)
                    this->valueVector_.push_back(MultiType(defvalue[i]));

                this->initVector();
            }

            ~ConfigValueContainer();

            /**
                @brief Returns the configured value.
                @param value A pointer to the variable to store the value.
                @param object The object calling this function
                @return The ConfigValueContainer
            */
            template <typename T, class C>
            ConfigValueContainer& getValue(T* value, C* object)
            {
                if ((this->callback_ && object) || this->bContainerIsNew_)
                {
                    if (this->bContainerIsNew_)
                        this->bContainerIsNew_ = false;

                    T temp = *value;
                    this->value_.getValue(value);
                    if ((*value) != temp)
                    {
                        if (this->callback_ && object)
                            this->callback_->call(object);
                        else
                            this->bDoInitialCallback_ = true;
                    }
                }
                else
                {
                    this->value_.getValue(value);
                }
                return *this;
            }

            /**
                @brief Returns the configured vector.
                @param value A pointer to the vector to store the values.
                @param object The object calling this function
                @return The ConfigValueContainer
            */
            template <typename T, class C>
            ConfigValueContainer& getValue(std::vector<T>* value, C* object)
            {
                if ((this->callback_ && object) || this->bContainerIsNew_)
                {
                    if (this->bContainerIsNew_)
                        this->bContainerIsNew_ = false;

                    std::vector<T> temp = *value;
                    value->clear();
                    for (unsigned int i = 0; i < this->valueVector_.size(); ++i)
                        value->push_back(this->valueVector_[i]);

                    if (value->size() != temp.size())
                    {
                        if (this->callback_ && object)
                            this->callback_->call(object);
                        else
                            this->bDoInitialCallback_ = true;
                    }
                    else
                    {
                        for (unsigned int i = 0; i < value->size(); ++i)
                        {
                            if ((*value)[i] != temp[i])
                            {
                                if (this->callback_ && object)
                                    this->callback_->call(object);
                                else
                                    this->bDoInitialCallback_ = true;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    value->clear();
                    for (unsigned int i = 0; i < this->valueVector_.size(); ++i)
                        value->push_back(this->valueVector_[i]);
                }
                return *this;
            }

            /** @brief Returns the name of this container. */
            inline const std::string& getName() const
                { return this->varname_; }
            /** @brief Returns true if this config-value is a vector */
            inline bool isVector() const
                { return this->bIsVector_; }
            /** @brief Returns the vectors size (or zero if it's not a vector). */
            inline unsigned int getVectorSize() const
                { return this->valueVector_.size(); }

            ConfigValueContainer& description(const std::string& description);
            const std::string& getDescription() const;

            /**
                @brief Adds a callback function, that gets called after getValue() if the newly assigned value differs from the old value of the variable.
                @param object The object to call the function
                @param function The callback function
            */
            template <class T>
            inline ConfigValueContainer& callback(T* object, void (T::*function) (void))
            {
                if (!this->callback_)
                {
                    this->callback_ = new ConfigValueCallback<T>(function);

                    if (this->bDoInitialCallback_)
                    {
                        this->bDoInitialCallback_ = false;
                        this->callback_->call(object);
                    }
                }

                return (*this);
            }

            bool set(const MultiType& input);
            bool tset(const MultiType& input);

            bool set(unsigned int index, const MultiType& input);
            bool tset(unsigned int index, const MultiType& input);
            bool add(const MultiType& input);
            bool remove(unsigned int index);

            bool reset();
            void update();

            /** @brief Converts the config-value to a string. @return The string */
            inline std::string toString() const
                { return this->value_; }
            /** @brief Returns the typename of the assigned config-value. @return The typename */
            inline std::string getTypename() const
                { return this->value_.getTypename(); }

        private:
            void init(ConfigFileType type, Identifier* identifier, const std::string& sectionname, const std::string& varname);
            void initValue(const MultiType& defvalue);
            void initVector();
            bool callFunctionWithIndex(bool (ConfigValueContainer::* function) (unsigned int, const MultiType&), const std::string& input);

            bool                       bIsVector_;                  //!< True if the container contains a std::vector

            ConfigFileType             type_;                       //!< The type of the corresponding config-file
            Identifier*                identifier_;                 //!< The identifier of the class
            std::string                sectionname_;                //!< The name of the class the variable belongs to
            std::string                varname_;                    //!< The name of the variable
            std::string                defvalueString_;             //!< The string of the default-value
            std::vector<std::string>   defvalueStringVector_;       //!< A vector, containg the strings of the default-values in case we're storing a vector

            MultiType                  value_;                      //!< The value
            std::vector<MultiType>     valueVector_;                //!< A vector, containg the values in case we're storing a vector

            bool                       bAddedDescription_;          //!< True if a description was added
            LanguageEntryLabel         description_;                //!< The description
            ConfigValueCallbackBase*   callback_;                   //!< A callback function to call after getValue if the value changed

            bool                       bContainerIsNew_;            //!< True if it's the first time getValue() gets called
            bool                       bDoInitialCallback_;         //!< True if the callback should be called as soon as it gets created
    };
}

#endif /* _ConfigValueContainer_H__ */
