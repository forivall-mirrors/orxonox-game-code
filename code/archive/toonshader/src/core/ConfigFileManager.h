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
#include "util/OrxEnum.h"

namespace orxonox
{
    // Use int as config file type to have an arbitrary number of files
    struct ConfigFileType : OrxEnum<ConfigFileType>
    {
        OrxEnumConstructors(ConfigFileType);

        static const int NoType              = 0;
        static const int Settings            = 1;
        static const int JoyStickCalibration = 2;

        static const int numberOfReservedTypes = 1024;
    };

    bool config(const std::string& classname, const std::string& varname, const std::string& value);
    bool tconfig(const std::string& classname, const std::string& varname, const std::string& value);
    void reloadConfig();
    void saveConfig();
    void cleanConfig();
    void loadSettings(const std::string& filename);


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
            inline ConfigFileEntryValue(const std::string& name, const std::string& value = "", bool bString = false, const std::string& additionalComment = "") : name_(name), value_(value), bString_(bString), additionalComment_(additionalComment) {}
            inline virtual ~ConfigFileEntryValue() {}

            inline virtual const std::string& getName() const
                { return this->name_; }

            inline virtual void setComment(const std::string& comment)
                { this->additionalComment_ = comment; }

            virtual void setValue(const std::string& value);
            virtual std::string getValue() const;

            inline bool isString() const
                { return this->bString_; }
            inline void setString(bool bString)
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
            inline ConfigFileEntryVectorValue(const std::string& name, unsigned int index, const std::string& value = "", bool bString = false, const std::string& additionalComment = "") : ConfigFileEntryValue(name, value, bString, additionalComment), index_(index) {}
            inline virtual ~ConfigFileEntryVectorValue() {}

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

            inline virtual void setComment(const std::string& comment)
                { this->comment_ = comment; }

            inline virtual void setValue(const std::string& value)
                {}
            inline virtual std::string getValue() const
                { return this->comment_; }

            inline void setString(bool bString) {}

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

            inline void setComment(const std::string& comment)
                { this->additionalComment_ = comment; }

            inline void setValue(const std::string& name, const std::string& value, bool bString)
                { this->getEntry(name, value, bString)->setValue(value); }
            inline std::string getValue(const std::string& name, const std::string& fallback, bool bString)
                { return this->getEntry(name, fallback, bString)->getValue(); }

            inline void setValue(const std::string& name, unsigned int index, const std::string& value, bool bString)
                { this->getEntry(name, index, value, bString)->setValue(value); }
            inline std::string getValue(const std::string& name, unsigned int index, const std::string& fallback, bool bString)
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

            inline ConfigFileEntry* getEntry(const std::string& name, const std::string& fallback, bool bString)
                { return (*this->getEntryIterator(name, fallback, bString)); }
            inline ConfigFileEntry* getEntry(const std::string& name, unsigned int index, const std::string& fallback, bool bString)
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
            inline ConfigFile(const std::string& filename, ConfigFileType type)
                : filename_(filename)
                , type_(type)
                , bUpdated_(false)
            { }
            ~ConfigFile();

            void load(bool bCreateIfNotExisting = true);
            void save() const;
            void saveAs(const std::string& filename);
            void clean(bool bCleanComments = false);
            void clear();

            const std::string& getFilename() { return this->filename_; }

            inline void setValue(const std::string& section, const std::string& name, const std::string& value, bool bString)
                { this->getSection(section)->setValue(name, value, bString); this->save(); }
            inline std::string getValue(const std::string& section, const std::string& name, const std::string& fallback, bool bString)
                { std::string output = this->getSection(section)->getValue(name, fallback, bString); this->saveIfUpdated(); return output; }

            inline void setValue(const std::string& section, const std::string& name, unsigned int index, const std::string& value, bool bString)
                { this->getSection(section)->setValue(name, index, value, bString); this->save(); }
            inline std::string getValue(const std::string& section, const std::string& name, unsigned int index, const std::string& fallback, bool bString)
                { std::string output = this->getSection(section)->getValue(name, index, fallback, bString); this->saveIfUpdated(); return output; }

            inline void deleteVectorEntries(const std::string& section, const std::string& name, unsigned int startindex = 0)
                { this->getSection(section)->deleteVectorEntries(name, startindex); }
            inline unsigned int getVectorSize(const std::string& section, const std::string& name)
                { return this->getSection(section)->getVectorSize(name); }

            void updateConfigValues();

        private:
            ConfigFileSection* getSection(const std::string& section);
            void saveIfUpdated();

            std::string filename_;
            ConfigFileType type_;
            std::list<ConfigFileSection*> sections_;
            bool bUpdated_;
    };


    ///////////////////////
    // ConfigFileManager //
    ///////////////////////
    class _CoreExport ConfigFileManager
    {
        public:
            ConfigFileManager();
            ~ConfigFileManager();

            void load();
            void save();
            void clean(bool bCleanComments = false);

            void setFilename(ConfigFileType type, const std::string& filename);
            const std::string& getFilename(ConfigFileType type);

            ConfigFileType getNewConfigFileType() { return mininmalFreeType_++; }

            void load(ConfigFileType type);
            void save(ConfigFileType type);
            void saveAs(ConfigFileType type, const std::string& saveFilename);
            void clean(ConfigFileType type, bool bCleanComments = false);

            inline void setValue(ConfigFileType type, const std::string& section, const std::string& name, const std::string& value, bool bString)
                { this->getFile(type)->setValue(section, name, value, bString); }
            inline std::string getValue(ConfigFileType type, const std::string& section, const std::string& name, const std::string& fallback, bool bString)
                { return this->getFile(type)->getValue(section, name, fallback, bString); }

            inline void setValue(ConfigFileType type, const std::string& section, const std::string& name, unsigned int index, const std::string& value, bool bString)
                { this->getFile(type)->setValue(section, name, index, value, bString); }
            inline std::string getValue(ConfigFileType type, const std::string& section, const std::string& name, unsigned int index, const std::string& fallback, bool bString)
                { return this->getFile(type)->getValue(section, name, index, fallback, bString); }

            inline void deleteVectorEntries(ConfigFileType type, const std::string& section, const std::string& name, unsigned int startindex = 0)
                { this->getFile(type)->deleteVectorEntries(section, name, startindex); }
            inline unsigned int getVectorSize(ConfigFileType type, const std::string& section, const std::string& name)
                { return this->getFile(type)->getVectorSize(section, name); }

            void updateConfigValues();
            void updateConfigValues(ConfigFileType type);

            static ConfigFileManager& getInstance() { assert(singletonRef_s); return *singletonRef_s; }

        private:
            ConfigFileManager(const ConfigFileManager&);

            ConfigFile* getFile(ConfigFileType type);

            std::map<ConfigFileType, ConfigFile*> configFiles_;
            unsigned int mininmalFreeType_;

            static ConfigFileManager* singletonRef_s;
    };
}

#endif /* _ConfigFileManager_H__ */
