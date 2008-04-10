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

#ifndef _ConfigFileManager_H__
#define _ConfigFileManager_H__

#include <iostream>
#include <string>
#include <list>
#include <map>

#include "util/Math.h"

#include "CorePrereqs.h"

#define DEFAULT_CONFIG_FILE "default.ini"

namespace orxonox
{
    enum _CoreExport ConfigFileType
    {
        CFT_Settings,
        CFT_Keybindings
    };


    void reloadConfig();
    void saveConfig();
    void cleanConfig();
    void loadSettings(const std::string& filename);
    void loadKeybindings(const std::string& filename);


    /////////////////////
    // ConfigFileEntry //
    /////////////////////
    class _CoreExport ConfigFileEntry
    {
        public:
            virtual void setValue(const std::string& value) = 0;
            virtual const std::string& getValue() const = 0;
            virtual const std::string& getName() const = 0;
            virtual unsigned int getIndex() const { return 0; }
            virtual std::string getFileEntry() const = 0;
    };


    //////////////////////////
    // ConfigFileEntryValue //
    //////////////////////////
    class _CoreExport ConfigFileEntryValue : public ConfigFileEntry
    {
        public:
            inline ConfigFileEntryValue(const std::string& name, const std::string& value = "", const std::string& additionalComment = "") : name_(name), value_(value), additionalComment_(additionalComment), bString_(false) {}
            inline virtual ~ConfigFileEntryValue() {}

            inline virtual const std::string& getName() const
                { return this->name_; }

            inline virtual void setValue(const std::string& value)
                { this->value_ = value; }
            inline virtual const std::string& getValue() const
                { return this->value_; }

            inline bool isString() const
                { return this->bString_; }
            inline void setString(bool bString)
                { this->bString_ = bString; }

            virtual std::string getFileEntry() const;

        protected:
            std::string name_;
            std::string value_;
            std::string additionalComment_;
            bool bString_;
    };


    ///////////////////////////////
    // ConfigFileEntryArrayValue //
    ///////////////////////////////
    class _CoreExport ConfigFileEntryArrayValue : public ConfigFileEntryValue
    {
        public:
            inline ConfigFileEntryArrayValue(const std::string& name, unsigned int index, const std::string& value = "", const std::string& additionalComment = "") : ConfigFileEntryValue(name, value, additionalComment), index_(index) {}
            inline virtual ~ConfigFileEntryArrayValue() {}

            inline virtual unsigned int getIndex() const
                { return this->index_; }

            virtual std::string getFileEntry() const;

        private:
            unsigned int index_;
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

            inline virtual void setValue(const std::string& value)
                {}
            inline virtual const std::string& getValue() const
                { return this->comment_; }

            inline virtual std::string getFileEntry() const
                { return this->comment_; }

        private:
            std::string comment_;
    };


    ///////////////////////
    // ConfigFileSection //
    ///////////////////////
    class _CoreExport ConfigFileSection
    {
        friend class ConfigFile;

        public:
            inline ConfigFileSection(const std::string& name, const std::string& additionalComment = "") : name_(name), additionalComment_(additionalComment), bUpdated_(false) {}
            ~ConfigFileSection();

            inline const std::string& getName() const
                { return this->name_; }

            inline void setValue(const std::string& name, const std::string& value)
                { this->getEntry(name, value)->setValue(value); }
            inline const std::string& getValue(const std::string& name, const std::string& fallback)
                { return this->getEntry(name, fallback)->getValue(); }

            inline void setValue(const std::string& name, unsigned int index, const std::string& value)
                { this->getEntry(name, index, value)->setValue(value); }
            inline const std::string& getValue(const std::string& name, unsigned int index, const std::string& fallback)
                { return this->getEntry(name, index, fallback)->getValue(); }

            std::string getFileEntry() const;

        private:
            std::list<ConfigFileEntry*>& getEntries()
                { return this->entries_; }
            std::list<ConfigFileEntry*>::const_iterator getEntriesBegin() const
                { return this->entries_.begin(); }
            std::list<ConfigFileEntry*>::const_iterator getEntriesEnd() const
                { return this->entries_.end(); }

            std::list<ConfigFileEntry*>::iterator getEntryIterator(const std::string& name, const std::string& fallback = "");
            std::list<ConfigFileEntry*>::iterator getEntryIterator(const std::string& name, unsigned int index, const std::string& fallback = "");

            inline ConfigFileEntry* getEntry(const std::string& name, const std::string& fallback)
                { return (*this->getEntryIterator(name, fallback)); }
            inline ConfigFileEntry* getEntry(const std::string& name, unsigned int index, const std::string& fallback)
                { return (*this->getEntryIterator(name, index, fallback)); }

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
            inline ConfigFile(const std::string& filename) : filename_(filename), bUpdated_(false) {}
            ~ConfigFile();

            void load();
            void save() const;
            void clean();

            inline void setValue(const std::string& section, const std::string& name, const std::string& value)
                { this->getSection(section)->setValue(name, value); this->save(); }
            inline const std::string& getValue(const std::string& section, const std::string& name, const std::string& fallback)
                { const std::string& output = this->getSection(section)->getValue(name, fallback); this->saveIfUpdated(); return output; }

            inline void setValue(const std::string& section, const std::string& name, unsigned int index, const std::string& value)
                { this->getSection(section)->setValue(name, index, value); this->save(); }
            inline const std::string& getValue(const std::string& section, const std::string& name, unsigned int index, const std::string& fallback)
                { const std::string& output = this->getSection(section)->getValue(name, index, fallback); this->saveIfUpdated(); return output; }

        private:
            ConfigFileSection* getSection(const std::string& section);
            void saveIfUpdated();

            std::string filename_;
            std::list<ConfigFileSection*> sections_;
            bool bUpdated_;
    };


    ///////////////////////
    // ConfigFileManager //
    ///////////////////////
    class _CoreExport ConfigFileManager
    {
        public:
            static ConfigFileManager* getSingleton();

            void setFile(ConfigFileType type, const std::string& filename);

            void load();
            void save();
            void clean();

            void load(ConfigFileType type);
            void save(ConfigFileType type);
            void clean(ConfigFileType type);

            inline void setValue(ConfigFileType type, const std::string& section, const std::string& name, const std::string& value)
                { this->getFile(type)->setValue(section, name, value); }
            inline const std::string& getValue(ConfigFileType type, const std::string& section, const std::string& name, const std::string& fallback)
                { return this->getFile(type)->getValue(section, name, fallback); }

            inline void setValue(ConfigFileType type, const std::string& section, const std::string& name, unsigned int index, const std::string& value)
                { this->getFile(type)->setValue(section, name, index, value); }
            inline const std::string& getValue(ConfigFileType type, const std::string& section, const std::string& name, unsigned int index, const std::string& fallback)
                { return this->getFile(type)->getValue(section, name, index, fallback); }

            void updateConfigValues() const;
            void updateConfigValues(ConfigFileType type) const;

        private:
            ConfigFileManager();
            ConfigFileManager(const ConfigFileManager& other) {}
            ~ConfigFileManager();

            ConfigFile* getFile(ConfigFileType type);

            std::string getFilePath(const std::string& name) const;

            std::map<ConfigFileType, ConfigFile*> configFiles_;
    };
}

#endif /* _ConfigFileManager_H__ */
