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

#include <iostream>
#include <string>
#include <list>
#include <map>

#include "util/Math.h"

#define DEFAULT_CONFIG_FILE "default.ini"

namespace orxonox
{
    enum _CoreExport ConfigFileType
    {
        CFT_Settings,
        CFT_Keybindings
    };


    bool config(const std::string& classname, const std::string& varname, const std::string& value);
    bool tconfig(const std::string& classname, const std::string& varname, const std::string& value);
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
            virtual ~ConfigFileEntry() {};
            virtual void setValue(const std::string& value) = 0;
            virtual std::string getValue() const = 0;
            virtual const std::string& getName() const = 0;
            virtual void setComment(const std::string& comment) = 0;
            virtual unsigned int getIndex() const { return 0; }
            virtual void setString(bool bString) = 0;
            virtual std::string getFileEntry() const = 0;
    };


    //////////////////////////
    // ConfigFileEntryValue //
    //////////////////////////
    class _CoreExport ConfigFileEntryValue : public ConfigFileEntry
    {
        public:
            ConfigFileEntryValue(const std::string& name, const std::string& value = "", bool bString = false, const std::string& additionalComment = "") : name_(name), value_(value), bString_(bString), additionalComment_(additionalComment) {}
            virtual ~ConfigFileEntryValue() {}

            virtual const std::string& getName() const
                { return this->name_; }

            virtual void setComment(const std::string& comment)
                { this->additionalComment_ = comment; }

            virtual void setValue(const std::string& value);
            virtual std::string getValue() const;

            bool isString() const
                { return this->bString_; }
            void setString(bool bString)
                { this->bString_ = bString; }

            virtual std::string getFileEntry() const;

        protected:
            std::string name_;
            std::string value_;
            bool bString_;
            std::string additionalComment_;
    };


    ///////////////////////////////
    // ConfigFileEntryVectorValue //
    ///////////////////////////////
    class _CoreExport ConfigFileEntryVectorValue : public ConfigFileEntryValue
    {
        public:
            ConfigFileEntryVectorValue(const std::string& name, unsigned int index, const std::string& value = "", bool bString = false, const std::string& additionalComment = "") : ConfigFileEntryValue(name, value, bString, additionalComment), index_(index) {}
            virtual ~ConfigFileEntryVectorValue() {}

            virtual unsigned int getIndex() const
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
            ConfigFileEntryComment(const std::string& comment) : comment_(comment) {}
            virtual ~ConfigFileEntryComment() {}

            virtual const std::string& getName() const
                { return this->comment_; }

            virtual void setComment(const std::string& comment)
                { this->comment_ = comment; }

            virtual void setValue(const std::string& value)
                {}
            virtual std::string getValue() const
                { return this->comment_; }

            void setString(bool bString) {}

            virtual std::string getFileEntry() const
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
            ConfigFileSection(const std::string& name, const std::string& additionalComment = "") : name_(name), additionalComment_(additionalComment), bUpdated_(false) {}
            ~ConfigFileSection();

            const std::string& getName() const
                { return this->name_; }

            void setComment(const std::string& comment)
                { this->additionalComment_ = comment; }

            void setValue(const std::string& name, const std::string& value, bool bString)
                { this->getEntry(name, value, bString)->setValue(value); }
            std::string getValue(const std::string& name, const std::string& fallback, bool bString)
                { return this->getEntry(name, fallback, bString)->getValue(); }

            void setValue(const std::string& name, unsigned int index, const std::string& value, bool bString)
                { this->getEntry(name, index, value, bString)->setValue(value); }
            std::string getValue(const std::string& name, unsigned int index, const std::string& fallback, bool bString)
                { return this->getEntry(name, index, fallback, bString)->getValue(); }

            void deleteVectorEntries(const std::string& name, unsigned int startindex = 0);
            unsigned int getVectorSize(const std::string& name);

            std::string getFileEntry() const;

        private:
            std::list<ConfigFileEntry*>& getEntries()
                { return this->entries_; }
            std::list<ConfigFileEntry*>::const_iterator getEntriesBegin() const
                { return this->entries_.begin(); }
            std::list<ConfigFileEntry*>::const_iterator getEntriesEnd() const
                { return this->entries_.end(); }

            std::list<ConfigFileEntry*>::iterator getEntryIterator(const std::string& name, const std::string& fallback, bool bString);
            std::list<ConfigFileEntry*>::iterator getEntryIterator(const std::string& name, unsigned int index, const std::string& fallback, bool bString);

            ConfigFileEntry* getEntry(const std::string& name, const std::string& fallback, bool bString)
                { return (*this->getEntryIterator(name, fallback, bString)); }
            ConfigFileEntry* getEntry(const std::string& name, unsigned int index, const std::string& fallback, bool bString)
                { return (*this->getEntryIterator(name, index, fallback, bString)); }

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
            ConfigFile(const std::string& filename) : filename_(filename), bUpdated_(false) {}
            ~ConfigFile();

            void load(bool bCreateIfNotExisting = true);
            void save() const;
            void save(const std::string& filename);
            void clean(bool bCleanComments = false);

            void setValue(const std::string& section, const std::string& name, const std::string& value, bool bString)
                { this->getSection(section)->setValue(name, value, bString); this->save(); }
            std::string getValue(const std::string& section, const std::string& name, const std::string& fallback, bool bString)
                { std::string output = this->getSection(section)->getValue(name, fallback, bString); this->saveIfUpdated(); return output; }

            void setValue(const std::string& section, const std::string& name, unsigned int index, const std::string& value, bool bString)
                { this->getSection(section)->setValue(name, index, value, bString); this->save(); }
            std::string getValue(const std::string& section, const std::string& name, unsigned int index, const std::string& fallback, bool bString)
                { std::string output = this->getSection(section)->getValue(name, index, fallback, bString); this->saveIfUpdated(); return output; }

            void deleteVectorEntries(const std::string& section, const std::string& name, unsigned int startindex = 0)
                { this->getSection(section)->deleteVectorEntries(name, startindex); }
            unsigned int getVectorSize(const std::string& section, const std::string& name)
                { return this->getSection(section)->getVectorSize(name); }

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

            void setFile(ConfigFileType type, const std::string& filename, bool bCreateIfNotExisting = true);

            void load(bool bCreateIfNotExisting = true);
            void save();
            void clean(bool bCleanComments = false);

            void load(ConfigFileType type, bool bCreateIfNotExisting = true);
            void save(ConfigFileType type);
            void save(ConfigFileType type, const std::string& filename);
            void clean(ConfigFileType type, bool bCleanComments = false);

            void setValue(ConfigFileType type, const std::string& section, const std::string& name, const std::string& value, bool bString)
                { this->getFile(type)->setValue(section, name, value, bString); }
            std::string getValue(ConfigFileType type, const std::string& section, const std::string& name, const std::string& fallback, bool bString)
                { return this->getFile(type)->getValue(section, name, fallback, bString); }

            void setValue(ConfigFileType type, const std::string& section, const std::string& name, unsigned int index, const std::string& value, bool bString)
                { this->getFile(type)->setValue(section, name, index, value, bString); }
            std::string getValue(ConfigFileType type, const std::string& section, const std::string& name, unsigned int index, const std::string& fallback, bool bString)
                { return this->getFile(type)->getValue(section, name, index, fallback, bString); }

            void deleteVectorEntries(ConfigFileType type, const std::string& section, const std::string& name, unsigned int startindex = 0)
                { this->getFile(type)->deleteVectorEntries(section, name, startindex); }
            unsigned int getVectorSize(ConfigFileType type, const std::string& section, const std::string& name)
                { return this->getFile(type)->getVectorSize(section, name); }

            void updateConfigValues() const;
            void updateConfigValues(ConfigFileType type) const;

        private:
            ConfigFileManager();
            ConfigFileManager(const ConfigFileManager& other);
            ~ConfigFileManager();

            ConfigFile* getFile(ConfigFileType type);

            std::string getFilePath(const std::string& name) const;

            std::map<ConfigFileType, ConfigFile*> configFiles_;
    };
}

#endif /* _ConfigFileManager_H__ */
