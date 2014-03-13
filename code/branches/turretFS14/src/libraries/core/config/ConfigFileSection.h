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
    @file
    @ingroup Config ConfigFile
*/

#ifndef _ConfigFileSection_H__
#define _ConfigFileSection_H__

#include "core/CorePrereqs.h"

#include <string>
#include <list>
#include "ConfigFileEntry.h"

namespace orxonox
{
    ///////////////////////
    // ConfigFileSection //
    ///////////////////////
    /**
        @brief Represents a section in a config file.

        A section has a name and a list of config values.
    */
    class _CoreExport ConfigFileSection
    {
        friend class ConfigFile;
        friend class SettingsConfigFile;

        public:
            /**
                @brief Constructor: Initializes the section.

                @param name The name of the section
                @param additionalComment An additional comment placed after the title of the section in the config file
            */
            inline ConfigFileSection(const std::string& name, const std::string& additionalComment = "")
                : name_(name)
                , additionalComment_(additionalComment)
                , bUpdated_(false)
                {}
            ~ConfigFileSection();

            /// Returns the name of the section.
            inline const std::string& getName() const
                { return this->name_; }

            /// Changes the comment which is placed after the title of the section in the config file.
            inline void setComment(const std::string& comment)
                { this->additionalComment_ = comment; }

            /**
                @brief Stores a value in the section. If the entry doesn't exist, it's created.

                @param name     The name of the entry
                @param value    The new value
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline void setValue(const std::string& name, const std::string& value, bool bString)
                { this->getOrCreateEntry(name, value, bString)->setValue(value); }
            /**
                @brief Returns the value of a given entry in the section. Returns a blank string if the value doesn't exist.

                @param name     The name of the entry
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline const std::string& getValue(const std::string& name, bool bString)
            {
                ConfigFileEntry* entry = this->getEntry(name);
                if (entry)
                {
                    entry->setString(bString);  // if the entry was loaded from the config file, we have to tell it if it's a string
                    return entry->getValue();
                }
                return BLANKSTRING;
            }
            /**
                @brief Returns the value of a given entry in the section. If it doesn't exist, the entry is created using the fallback value.

                @param name     The name of the entry
                @param fallback The value that will be used if the entry doesn't exist
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline const std::string& getOrCreateValue(const std::string& name, const std::string& fallback, bool bString)
                { return this->getOrCreateEntry(name, fallback, bString)->getValue(); }

            /**
                @brief Stores the value of an element of a vector in the section. If the entry doesn't exist, it's created.

                @param name     The name of the vector
                @param index    The index of the element in the vector
                @param value    The new value
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline void setValue(const std::string& name, unsigned int index, const std::string& value, bool bString)
                { this->getOrCreateEntry(name, index, value, bString)->setValue(value); }
            /**
                @brief Returns the value of a given element of a vector in the section. Returns a blank string if the value doesn't exist.

                @param name     The name of the vector
                @param index    The index of the element in the vector
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline const std::string& getValue(const std::string& name, unsigned int index, bool bString)
            {
                ConfigFileEntry* entry = this->getEntry(name, index);
                if (entry)
                {
                    entry->setString(bString);  // if the entry was loaded from the config file, we have to tell it if it's a string
                    return entry->getValue();
                }
                return BLANKSTRING;
            }
            /**
                @brief Returns the value of a given element of a vector in the section. If it doesn't exist, the entry is created using the fallback value.

                @param name     The name of the vector
                @param index    The index of the element in the vector
                @param fallback The value that will be used if the entry doesn't exist
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline const std::string& getOrCreateValue(const std::string& name, unsigned int index, const std::string& fallback, bool bString)
                { return this->getOrCreateEntry(name, index, fallback, bString)->getValue(); }

            void deleteVectorEntries(const std::string& name, unsigned int startindex = 0);
            unsigned int getVectorSize(const std::string& name) const;

            std::string getFileEntry() const;

        private:
            /// Returns the list of entries in this section.
            std::list<ConfigFileEntry*>& getEntries()
                { return this->entries_; }
            /// Returns the begin-iterator of the list of entries in this section.
            std::list<ConfigFileEntry*>::const_iterator getEntriesBegin() const
                { return this->entries_.begin(); }
            /// Returns the end-iterator of the list of entries in this section.
            std::list<ConfigFileEntry*>::const_iterator getEntriesEnd() const
                { return this->entries_.end(); }

            std::list<ConfigFileEntry*>::iterator getOrCreateEntryIterator(const std::string& name, const std::string& fallback, bool bString);
            std::list<ConfigFileEntry*>::iterator getOrCreateEntryIterator(const std::string& name, unsigned int index, const std::string& fallback, bool bString);

            ConfigFileEntry* getEntry(const std::string& name) const;
            /**
                @brief Returns the entry with given name. If it doesn't exist, the entry is created using the fallback value.

                @param name     The name of the entry
                @param fallback The value that will be used if the entry doesn't exist
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline ConfigFileEntry* getOrCreateEntry(const std::string& name, const std::string& fallback, bool bString)
                { return (*this->getOrCreateEntryIterator(name, fallback, bString)); }

            ConfigFileEntry* getEntry(const std::string& name, unsigned int index) const;
            /**
                @brief Returns the entry that contains an element of a vector with given name. If it doesn't exist, the entry is created using the fallback value.

                @param name     The name of the entry
                @param index    The index of the element in the vector
                @param fallback The value that will be used if the entry doesn't exist
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline ConfigFileEntry* getOrCreateEntry(const std::string& name, unsigned int index, const std::string& fallback, bool bString)
                { return (*this->getOrCreateEntryIterator(name, index, fallback, bString)); }

            std::string name_;                      ///< The name of the section
            std::string additionalComment_;         ///< The additional comment which is placed after the title of the section in the config file
            std::list<ConfigFileEntry*> entries_;   ///< The list of entries in this section
            bool bUpdated_;                         ///< True if an entry is created
    };
}

#endif /* _ConfigFileSection_H__ */
