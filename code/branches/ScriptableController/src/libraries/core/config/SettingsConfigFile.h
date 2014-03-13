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

#ifndef _SettingsConfigFile_H__
#define _SettingsConfigFile_H__

#include "core/CorePrereqs.h"

#include <set>
#include <map>
#include "ConfigFile.h"
#include "util/Singleton.h"

namespace orxonox // tolua_export
{ // tolua_export

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
} // tolua_export

#endif /* _SettingsConfigFile_H__ */
