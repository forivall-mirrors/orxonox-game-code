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
    @brief Declaration of ConfigFileManager and its helper classes, used to load and save config files.
*/

#ifndef _ConfigFileManager_H__
#define _ConfigFileManager_H__

#include "CorePrereqs.h"

#include <list>
#include <map>
#include <set>
#include <string>
#include <boost/array.hpp>

#include "util/Singleton.h"

namespace orxonox // tolua_export
{ // tolua_export

    /////////////////////
    // ConfigFileEntry //
    /////////////////////
    /**
        @brief This class represents an entry in the config file.

        This class is pure virtual. Use one of the derived classes to define the type of the entry.
    */
    class _CoreExport ConfigFileEntry
    {
        public:
            /// Destructor
            virtual ~ConfigFileEntry() {};

            /// Changes the value of the entry.
            virtual void setValue(const std::string& value) = 0;
            /// Returns the value of the entry.
            virtual const std::string& getValue() const = 0;

            /// Returns the name of the entry
            virtual const std::string& getName() const = 0;

            /// Changes the comment of the entry (will be placed after the value)
            virtual void setComment(const std::string& comment) = 0;

            /// Returns the index of the entry in a vector (used only if it is a vector)
            virtual unsigned int getIndex() const { return 0; }

            /// Defines if this entry is treated as string which means some special treatment of special characters.
            virtual void setString(bool bString) = 0;

            /// Returns the line as it will be stored in the config file.
            virtual const std::string& getFileEntry() const = 0;
    };


    //////////////////////////
    // ConfigFileEntryValue //
    //////////////////////////
    /**
        @brief This class represents a normal value in the config file.
    */
    class _CoreExport ConfigFileEntryValue : public ConfigFileEntry
    {
        public:
            /**
                @brief Constructor: Initializes the entry.

                @param name                 The name of the entry
                @param value                The value of the entry
                @param bString              If true, the value is treated as string which means some special treatment of special characters.
                @param additionalComment    An optional comment that will be placed behind the value in the config file
            */
            inline ConfigFileEntryValue(const std::string& name, const std::string& value = "", bool bString = false, const std::string& additionalComment = "")
                : name_(name)
                , value_(value)
                , additionalComment_(additionalComment)
                , bString_(bString)
                { this->update(); }

            /// Destructor
            inline virtual ~ConfigFileEntryValue() {}

            inline virtual const std::string& getName() const
                { return this->name_; }

            inline virtual void setComment(const std::string& comment)
                { this->additionalComment_ = comment; this->update(); }

            inline virtual void setValue(const std::string& value)
                { this->value_ = value; this->update(); }
            inline virtual const std::string& getValue() const
                { return this->value_; }

            inline void virtual setString(bool bString)
                { this->bString_ = bString; this->update(); }

            inline virtual const std::string& getFileEntry() const
                { return this->fileEntry_; }

            /// Returns the "key" of the value (in this case it's just the name of the entry, but for vectors it's different)
            inline virtual const std::string& getKeyString() const
                { return this->name_; }

        protected:
            virtual void update();

            const std::string name_;            ///< The name of the value
            std::string value_;                 ///< The value
            std::string additionalComment_;     ///< The additional comment
            std::string fileEntry_;             ///< The string as it will be stored in the config file
            bool bString_;                      ///< If true, the value is treated as string which means some special treatment of special characters.
    };


    ////////////////////////////////
    // ConfigFileEntryVectorValue //
    ////////////////////////////////
    /**
        @brief Subclass of ConfigFileEntryValue, represents an element of a vector.
    */
    class _CoreExport ConfigFileEntryVectorValue : public ConfigFileEntryValue
    {
        public:
            /**
                @brief Constructor: Initializes the entry.

                @param name                 The name of the vector
                @param index                The index of the element in the vector
                @param value                The value of the element
                @param bString              If true, the value is treated as string which means some special treatment of special characters.
                @param additionalComment    An optional comment that will be placed behind the value in the config file
            */
            inline ConfigFileEntryVectorValue(const std::string& name, unsigned int index, const std::string& value = "", bool bString = false, const std::string& additionalComment = "")
                : ConfigFileEntryValue(name, value, bString, additionalComment)
                , index_(index)
                { this->update(); /*No virtual calls in base class ctor*/ }

            /// Destructor
            inline ~ConfigFileEntryVectorValue() {}

            inline unsigned int getIndex() const
                { return this->index_; }

            /// Returns the "key" of the value (the name of the vector plus the index of the element)
            inline const std::string& getKeyString() const
                { return this->keyString_; }

        private:
            void update();

            unsigned int index_;        ///< The index of the element in the vector
            std::string keyString_;     ///< The full name of the entry (the name of the vector plus the index of the element)
    };


    ////////////////////////////
    // ConfigFileEntryComment //
    ////////////////////////////
    /**
        @brief This class represents a line in the config file which contains only a comment.
    */
    class _CoreExport ConfigFileEntryComment : public ConfigFileEntry
    {
        public:
            /// Constructor: Initializes the object.
            inline ConfigFileEntryComment(const std::string& comment) : comment_(comment) {}

            /// Destructor
            inline virtual ~ConfigFileEntryComment() {}

            inline virtual const std::string& getName() const
                { return this->comment_; }

            inline virtual void setComment(const std::string& comment)
                { this->comment_ = comment; }

            inline virtual void setValue(const std::string& value)
                {}
            inline virtual const std::string& getValue() const
                { return BLANKSTRING; }

            inline void setString(bool bString)
                {}

            inline virtual const std::string& getFileEntry() const
                { return this->comment_; }

        private:
            std::string comment_;   ///< The comment
    };


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


    ////////////////
    // ConfigFile //
    ////////////////
    /**
        @brief This class represents a config file, which is stored on the hard-disk and contains config values in different sections.

        It provides an interface to manipulate the sections and values.
    */
    class _CoreExport ConfigFile
    {
        public:
            ConfigFile(const std::string& filename, bool bCopyFallbackFile = true);
            virtual ~ConfigFile();

            virtual void load();
            virtual void save() const;
            virtual void saveAs(const std::string& filename) const;
            virtual void clear();

            /// Returns the file-name of this config file
            inline const std::string& getFilename()
                { return this->filename_; }

            /**
                @brief Stores a value in the config file. If the entry or its section doesn't exist, it's created.

                @param section  The name of the section
                @param name     The name of the entry
                @param value    The new value
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline void setValue(const std::string& section, const std::string& name, const std::string& value, bool bString)
            {
                this->getOrCreateSection(section)->setValue(name, value, bString);
                this->save();
            }
            /**
                @brief Returns the value of a given entry in the config file. Returns a blank string if the value doesn't exist.

                @param section  The name of the section
                @param name     The name of the entry
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline const std::string& getValue(const std::string& section, const std::string& name, bool bString)
            {
                ConfigFileSection* sectionPtr = this->getSection(section);
                return (sectionPtr ? sectionPtr->getValue(name, bString) : BLANKSTRING);
            }
            /**
                @brief Returns the value of a given entry in the config file. If it doesn't exist, the entry is created using the fallback value.

                @param section  The name of the section
                @param name     The name of the entry
                @param fallback The value that will be used if the entry doesn't exist
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline const std::string& getOrCreateValue(const std::string& section, const std::string& name, const std::string& fallback, bool bString)
            {
                const std::string& output = this->getOrCreateSection(section)->getOrCreateValue(name, fallback, bString);
                this->saveIfUpdated();
                return output;
            }

            /**
                @brief Stores the value of an element of a vector in the config file. If the entry or its section doesn't exist, it's created.

                @param section  The name of the section
                @param name     The name of the vector
                @param index    The index of the element in the vector
                @param value    The new value
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline void setValue(const std::string& section, const std::string& name, unsigned int index, const std::string& value, bool bString)
            {
                this->getOrCreateSection(section)->setValue(name, index, value, bString);
                this->save();
            }
            /**
                @brief Returns the value of a given element of a vector in the config file. Returns a blank string if the value doesn't exist.

                @param section  The name of the section
                @param name     The name of the vector
                @param index    The index of the element in the vector
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline const std::string& getValue(const std::string& section, const std::string& name, unsigned int index, bool bString)
            {
                ConfigFileSection* sectionPtr = this->getSection(section);
                return (sectionPtr ? sectionPtr->getValue(name, index, bString) : BLANKSTRING);
            }
            /**
                @brief Returns the value of a given element of a vector in the config file. If it doesn't exist, the entry is created using the fallback value.

                @param section  The name of the section
                @param name     The name of the vector
                @param index    The index of the element in the vector
                @param fallback The value that will be used if the entry doesn't exist
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            const std::string& getOrCreateValue(const std::string& section, const std::string& name, unsigned int index, const std::string& fallback, bool bString)
            {
                const std::string& output = this->getOrCreateSection(section)->getOrCreateValue(name, index, fallback, bString);
                this->saveIfUpdated();
                return output;
            }

            void deleteVectorEntries(const std::string& section, const std::string& name, unsigned int startindex = 0);
            /**
                @brief Returns the size of a config vector.
                @param section  The section of the vector
                @param name     The name of the vector
            */
            inline unsigned int getVectorSize(const std::string& section, const std::string& name) const
            {
                ConfigFileSection* sectionPtr = this->getSection(section);
                return (sectionPtr ? sectionPtr->getVectorSize(name) : 0);
            }

            static const char* DEFAULT_CONFIG_FOLDER;   ///< The folder where the default config files will be stored

        protected:
            ConfigFileSection* getSection(const std::string& section) const;
            ConfigFileSection* getOrCreateSection(const std::string& section);

            std::list<ConfigFileSection*> sections_;    ///< A list of sections in this config file

        private:
            void saveIfUpdated();

            const std::string filename_;                ///< The filename of this config file
            const bool bCopyFallbackFile_;              ///< If true, the default config file is copied into the config-directory before loading the file
            bool bUpdated_;                             ///< Becomes true if a section is added
    };


    ////////////////////////
    // SettingsConfigFile //
    ////////////////////////
    /**
        @brief Child class of ConfigFile, used to store the settings of the game.

        In addition to ConfigFile, this class provides an interface to manipulate the settings
        with console commands and to cache entries in instances of ConfigValueContainer.

        SettingsConfigFile is a Singleton, meaning there's only one instance of this class
        (and thus only one config file that stores settings).
    */
    class _CoreExport SettingsConfigFile // tolua_export
        : public ConfigFile, public Singleton<SettingsConfigFile>
    { // tolua_export
        friend class Singleton<SettingsConfigFile>;

        public:
            typedef std::multimap<std::string, std::pair<std::string, ConfigValueContainer*> > ContainerMap;

            SettingsConfigFile(const std::string& filename);
            ~SettingsConfigFile();

            void load(); // tolua_export
            void setFilename(const std::string& filename); // tolua_export
            void clean(bool bCleanComments = false); // tolua_export

            void config(const std::string& section, const std::string& entry, const std::string& value); // tolua_export
            void tconfig(const std::string& section, const std::string& entry, const std::string& value); // tolua_export
            std::string getConfig(const std::string& section, const std::string& entry); // tolua_export

            void addConfigValueContainer(ConfigValueContainer* container);
            void removeConfigValueContainer(ConfigValueContainer* container);

            /// Returns a set containing the names of all sections in this config file.
            inline const std::set<std::string>& getSectionNames()
                { return this->sectionNames_; }
            /// Returns the lower-bound-iterator of the @ref ConfigValueContainer "config value containers" for the given section.
            inline ContainerMap::const_iterator getContainerLowerBound(const std::string section)
                { return this->containers_.lower_bound(section); }
            /// Returns the upper-bound-iterator of the @ref ConfigValueContainer "config value containers" for the given section.
            inline ContainerMap::const_iterator getContainerUpperBound(const std::string section)
                { return this->containers_.upper_bound(section); }

            static SettingsConfigFile& getInstance() { return Singleton<SettingsConfigFile>::getInstance(); } // tolua_export

        private:
            void updateConfigValues();
            bool configImpl(const std::string& section, const std::string& entry, const std::string& value, bool (ConfigValueContainer::*function)(const MultiType&));

            ContainerMap containers_;                   ///< Stores all @ref ConfigValueContainer "config value containers"
            std::set<std::string> sectionNames_;        ///< Stores all section names
            static SettingsConfigFile* singletonPtr_s;  ///< The singleton pointer
    }; // tolua_export


    ///////////////////////
    // ConfigFileManager //
    ///////////////////////
    /**
        @brief Manages the different config files (settings, calibration, etc). Implemented as Singleton.
    */
    class _CoreExport ConfigFileManager : public Singleton<ConfigFileManager>
    {
        friend class Singleton<ConfigFileManager>;
        public:
            ConfigFileManager();
            ~ConfigFileManager();

            void setFilename(ConfigFileType::Value type, const std::string& filename);

            /// Returns the config file of a given type (settings, calibration, etc.)
            inline ConfigFile* getConfigFile(ConfigFileType::Value type)
            {
                // Check array bounds
                return configFiles_.at(type);
            }

        private:
            ConfigFileManager(const ConfigFileManager&);    ///< Copy-constructor: not implemented

            boost::array<ConfigFile*, 3> configFiles_;      ///< Stores the config files for each type in an array (must have the same size like ConfigFileType::Value)
            static ConfigFileManager* singletonPtr_s;       ///< Stores the singleton-pointer
    };
} // tolua_export

#endif /* _ConfigFileManager_H__ */
