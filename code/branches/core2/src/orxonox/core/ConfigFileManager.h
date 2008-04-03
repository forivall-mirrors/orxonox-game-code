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

#include <list>

#include "CorePrereqs.h"

namespace orxonox
{
    class ConfigFileEntry
    {
        public:
            ConfigFileEntry(const std::string& name, const std::string& value = "");

            void setValue(const std::string& value);
            const std::string& getValue() const;
            const std::string& getName() const;

        private:
            std::string name_;
            std::string value_;
    };

    class ConfigFileSection
    {
        public:
            void addEntry(const std::string& name);
            ConfigFileEntry* getEntry(const std::string& name) const;

            void setValue(const std::string& name, const std::string& value);
            const std::string& getValue(const std::string& name) const;

        private:
            std::string name_;
            std::list<ConfigFileEntry> entries_;
    };

    class ConfigFile
    {
        public:
            enum Type
            {
                Settings,
                Keybindings
            };

        public:
            void load();
            void save();

            void addSection(const std::string& name);
            ConfigFileSection* getSection(const std::string& name) const;

            void setValue(const std::string& section, const std::string& name, const std::string& value);
            const std::string& getValue(const std::string& section, const std::string& name) const;

        private:
            std::string name_;
            Type type_;
            std::list<ConfigFileSection> sections_;
    };

    class ConfigFileManager
    {
        public:
            static ConfigFileManater* getInstance();

            void load(const std::string& filename);
            void save(const std::string& filename) const;

            void setFile(const std::string& filename);
            ConfigFile& getFile(const std::string& filename) const;

            void setValue(const std::string& filename, const std::string& section, const std::string& name, const std::string& value);
            const std::string& getValue(const std::string& filename, const std::string& section, const std::string& name) const;

        private:
            std::list<ConfigFile> configFiles_;
    };
}

#endif /* _ConfigFileManager_H__ */
