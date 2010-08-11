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
    class _CoreExport ConfigFileEntry
    {
        public:
            virtual ~ConfigFileEntry() {};
            virtual void setValue(const std::string& value) = 0;
            virtual const std::string& getValue() const = 0;
            virtual const std::string& getName() const = 0;
            virtual void setComment(const std::string& comment) = 0;
            virtual unsigned int getIndex() const { return 0; }
            virtual void setString(bool bString) = 0;
            virtual const std::string& getFileEntry() const = 0;
    };


    //////////////////////////
    // ConfigFileEntryValue //
    //////////////////////////
    class _CoreExport ConfigFileEntryValue : public ConfigFileEntry
    {
        public:
            inline ConfigFileEntryValue(const std::string& name, const std::string& value = "", bool bString = false, const std::string& additionalComment = "")
                : name_(name)
                , value_(value)
                , additionalComment_(additionalComment)
                , bString_(bString)
                { this->update(); }

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

            inline virtual const std::string& getKeyString() const
                { return this->name_; }

        protected:
            virtual void update();

            const std::string name_;
            std::string value_;
            std::string additionalComment_;
            std::string fileEntry_;
            bool bString_;
    };


    ////////////////////////////////
    // ConfigFileEntryVectorValue //
    ////////////////////////////////
    class _CoreExport ConfigFileEntryVectorValue : public ConfigFileEntryValue
    {
        public:
            inline ConfigFileEntryVectorValue(const std::string& name, unsigned int index, const std::string& value = "", bool bString = false, const std::string& additionalComment = "")
                : ConfigFileEntryValue(name, value, bString, additionalComment)
                , index_(index)
                { this->update(); /*No virtual calls in base class ctor*/ }

            inline ~ConfigFileEntryVectorValue() {}

            inline unsigned int getIndex() const
                { return this->index_; }

            inline const std::string& getKeyString() const
                { return this->keyString_; }

        private:
            void update();

            unsigned int index_;
            std::string keyString_;
    };


    ////////////////////////////
    // ConfigFileEntryComment //
    ////////////////////////////
    class _CoreExport ConfigFileEntryComment : public ConfigFileEntry
    {
        public:
            inline ConfigFileEntryComment(const std::string& comment) : comment_(comment) {}
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

            inline virtual const std::string& getKeyString() const
                { return BLANKSTRING; }

        private:
            std::string comment_;
    };


    ///////////////////////
    // ConfigFileSection //
    ///////////////////////
    class _CoreExport ConfigFileSection
    {
        friend class ConfigFile;
        friend class SettingsConfigFile;

        public:
            inline ConfigFileSection(const std::string& name, const std::string& additionalComment = "")
                : name_(name)
                , additionalComment_(additionalComment)
                , bUpdated_(false)
                {}
            ~ConfigFileSection();

            inline const std::string& getName() const
                { return this->name_; }

            inline void setComment(const std::string& comment)
                { this->additionalComment_ = comment; }

            inline void setValue(const std::string& name, const std::string& value, bool bString)
                { this->getOrCreateEntry(name, value, bString)->setValue(value); }
            inline const std::string& getValue(const std::string& name, bool bString)
            {
                ConfigFileEntry* entry = this->getEntry(name);
                if (entry)
                {
                    entry->setString(bString);
                    return entry->getValue();
                }
                return BLANKSTRING;
            }
            inline const std::string& getOrCreateValue(const std::string& name, const std::string& fallback, bool bString)
                { return this->getOrCreateEntry(name, fallback, bString)->getValue(); }

            inline void setValue(const std::string& name, unsigned int index, const std::string& value, bool bString)
                { this->getOrCreateEntry(name, index, value, bString)->setValue(value); }
            inline const std::string& getValue(const std::string& name, unsigned int index, bool bString)
            {
                ConfigFileEntry* entry = this->getEntry(name, index);
                if (entry)
                {
                    entry->setString(bString);
                    return entry->getValue();
                }
                return BLANKSTRING;
            }
            inline const std::string& getOrCreateValue(const std::string& name, unsigned int index, const std::string& fallback, bool bString)
                { return this->getOrCreateEntry(name, index, fallback, bString)->getValue(); }

            void deleteVectorEntries(const std::string& name, unsigned int startindex = 0);
            unsigned int getVectorSize(const std::string& name) const;

            std::string getFileEntry() const;

        private:
            std::list<ConfigFileEntry*>& getEntries()
                { return this->entries_; }
            std::list<ConfigFileEntry*>::const_iterator getEntriesBegin() const
                { return this->entries_.begin(); }
            std::list<ConfigFileEntry*>::const_iterator getEntriesEnd() const
                { return this->entries_.end(); }

            std::list<ConfigFileEntry*>::iterator getOrCreateEntryIterator(const std::string& name, const std::string& fallback, bool bString);
            std::list<ConfigFileEntry*>::iterator getOrCreateEntryIterator(const std::string& name, unsigned int index, const std::string& fallback, bool bString);

            ConfigFileEntry* getEntry(const std::string& name) const;
            inline ConfigFileEntry* getOrCreateEntry(const std::string& name, const std::string& fallback, bool bString)
                { return (*this->getOrCreateEntryIterator(name, fallback, bString)); }
            ConfigFileEntry* getEntry(const std::string& name, unsigned int index) const;
            inline ConfigFileEntry* getOrCreateEntry(const std::string& name, unsigned int index, const std::string& fallback, bool bString)
                { return (*this->getOrCreateEntryIterator(name, index, fallback, bString)); }

            std::string name_;
            std::string additionalComment_;
            std::list<ConfigFileEntry*> entries_;
            bool bUpdated_;
    };


    ////////////////
    // ConfigFile //
    ////////////////
    class _CoreExport ConfigFile
    {
        public:
            ConfigFile(const std::string& filename, bool bCopyFallbackFile = true);
            virtual ~ConfigFile();

            virtual void load();
            virtual void save() const;
            virtual void saveAs(const std::string& filename) const;
            virtual void clear();

            inline const std::string& getFilename()
                { return this->filename_; }

            inline void setValue(const std::string& section, const std::string& name, const std::string& value, bool bString)
            {
                this->getOrCreateSection(section)->setValue(name, value, bString);
                this->save();
            }
            inline const std::string& getValue(const std::string& section, const std::string& name, bool bString)
            {
                ConfigFileSection* sectionPtr = this->getSection(section);
                return (sectionPtr ? sectionPtr->getValue(name, bString) : BLANKSTRING);
            }
            const std::string& getOrCreateValue(const std::string& section, const std::string& name, const std::string& fallback, bool bString);

            inline void setValue(const std::string& section, const std::string& name, unsigned int index, const std::string& value, bool bString)
            {
                this->getOrCreateSection(section)->setValue(name, index, value, bString);
                this->save();
            }
            inline const std::string& getValue(const std::string& section, const std::string& name, unsigned int index, bool bString)
            {
                ConfigFileSection* sectionPtr = this->getSection(section);
                return (sectionPtr ? sectionPtr->getValue(name, index, bString) : BLANKSTRING);
            }
            const std::string& getOrCreateValue(const std::string& section, const std::string& name, unsigned int index, const std::string& fallback, bool bString);

            void deleteVectorEntries(const std::string& section, const std::string& name, unsigned int startindex = 0);
            inline unsigned int getVectorSize(const std::string& section, const std::string& name) const
            {
                ConfigFileSection* sectionPtr = this->getSection(section);
                return (sectionPtr ? sectionPtr->getVectorSize(name) : 0);
            }

            static const char* DEFAULT_CONFIG_FOLDER;

        protected:
            ConfigFileSection* getSection(const std::string& section) const;
            ConfigFileSection* getOrCreateSection(const std::string& section);

            std::list<ConfigFileSection*> sections_;

        private:
            void saveIfUpdated();
            const std::string filename_;
            const bool bCopyFallbackFile_;
            bool bUpdated_;
    };


    ////////////////////////
    // SettingsConfigFile //
    ////////////////////////
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

            bool config(const std::string& section, const std::string& entry, const std::string& value); // tolua_export
            bool tconfig(const std::string& section, const std::string& entry, const std::string& value); // tolua_export
            std::string getConfig(const std::string& section, const std::string& entry); // tolua_export

            void addConfigValueContainer(ConfigValueContainer* container);
            void removeConfigValueContainer(ConfigValueContainer* container);

            inline const std::set<std::string>& getSectionNames()
                { return this->sectionNames_; }
            inline ContainerMap::const_iterator getContainerLowerBound(const std::string section)
                { return this->containers_.lower_bound(section); }
            inline ContainerMap::const_iterator getContainerUpperBound(const std::string section)
                { return this->containers_.upper_bound(section); }

            static SettingsConfigFile& getInstance() { return Singleton<SettingsConfigFile>::getInstance(); } // tolua_export

        private:
            void updateConfigValues();
            bool configImpl(const std::string& section, const std::string& entry, const std::string& value, bool (ConfigValueContainer::*function)(const MultiType&));

            ContainerMap containers_;
            std::set<std::string> sectionNames_;
            static SettingsConfigFile* singletonPtr_s;
    }; // tolua_export


    ///////////////////////
    // ConfigFileManager //
    ///////////////////////
    class _CoreExport ConfigFileManager : public Singleton<ConfigFileManager>
    {
        friend class Singleton<ConfigFileManager>;
        public:
            ConfigFileManager();
            ~ConfigFileManager();

            void setFilename(ConfigFileType::Value type, const std::string& filename);

            inline ConfigFile* getConfigFile(ConfigFileType::Value type)
            {
                // Check array bounds
                return configFiles_.at(type);
            }

        private:
            ConfigFileManager(const ConfigFileManager&);

            boost::array<ConfigFile*, 3> configFiles_;
            static ConfigFileManager* singletonPtr_s;
    };
} // tolua_export

#endif /* _ConfigFileManager_H__ */
