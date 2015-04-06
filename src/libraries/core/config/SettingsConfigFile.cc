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
    @brief Implementation of SettingsConfigFile.
*/

#include "SettingsConfigFile.h"

#include "util/StringUtils.h"
#include "core/command/ConsoleCommandIncludes.h"
#include "ConfigFileManager.h"
#include "ConfigValueContainer.h"

namespace orxonox
{
    ////////////////////////
    // SettingsConfigFile //
    ////////////////////////

    static const std::string __CC_load_name = "reloadSettings";
    static const std::string __CC_setFilename_name = "setSettingsFile";
    static const std::string __CC_config_name = "config";
    static const std::string __CC_tconfig_name = "tconfig";
    static const std::string __CC_getConfig_name = "getConfig";

    SetConsoleCommand(__CC_load_name,            &ConfigFile::load);
    SetConsoleCommand(__CC_setFilename_name,     &SettingsConfigFile::setFilename);
    SetConsoleCommand(__CC_config_name,          &SettingsConfigFile::config).argumentCompleter(0, autocompletion::settingssections()).argumentCompleter(1, autocompletion::settingsentries()).argumentCompleter(2, autocompletion::settingsvalue());
    SetConsoleCommand(__CC_tconfig_name,         &SettingsConfigFile::tconfig).argumentCompleter(0, autocompletion::settingssections()).argumentCompleter(1, autocompletion::settingsentries()).argumentCompleter(2, autocompletion::settingsvalue());
    SetConsoleCommand(__CC_getConfig_name,       &SettingsConfigFile::getConfig).argumentCompleter(0, autocompletion::settingssections()).argumentCompleter(1, autocompletion::settingsentries());

    SettingsConfigFile* SettingsConfigFile::singletonPtr_s = 0;

    /**
        @brief Constructor: Activates the console commands.
    */
    SettingsConfigFile::SettingsConfigFile(const std::string& filename)
        : ConfigFile(filename)
    {
        ModifyConsoleCommand(__CC_load_name).setObject(this);
        ModifyConsoleCommand(__CC_setFilename_name).setObject(this);
        ModifyConsoleCommand(__CC_config_name).setObject(this);
        ModifyConsoleCommand(__CC_tconfig_name).setObject(this);
        ModifyConsoleCommand(__CC_getConfig_name).setObject(this);
    }

    /**
        @brief Destructor: Deactivates the console commands.
    */
    SettingsConfigFile::~SettingsConfigFile()
    {
        ModifyConsoleCommand(__CC_load_name).setObject(0);
        ModifyConsoleCommand(__CC_setFilename_name).setObject(0);
        ModifyConsoleCommand(__CC_config_name).setObject(0);
        ModifyConsoleCommand(__CC_tconfig_name).setObject(0);
        ModifyConsoleCommand(__CC_getConfig_name).setObject(0);
    }

    /**
        @brief Loads the config file and updates the @ref ConfigValueContainer "config value containers".
    */
    void SettingsConfigFile::load()
    {
        ConfigFile::load();
        this->updateConfigValues();
    }

    /**
        @brief Changes the file-name.
    */
    void SettingsConfigFile::setFilename(const std::string& filename)
    {
        ConfigFileManager::getInstance().setFilename(ConfigFileType::Settings, filename);
    }

    /**
        @brief Registers a new @ref ConfigValueContainer "config value container".
    */
    void SettingsConfigFile::addConfigValueContainer(ConfigValueContainer* container)
    {
        if (container == NULL)
            return;
        std::pair<std::string, ConfigValueContainer*> second(getLowercase(container->getName()), container);
        this->containers_.insert(std::make_pair(getLowercase(container->getSectionName()), second));
        this->sectionNames_.insert(container->getSectionName());
    }

    /**
        @brief Unregisters a @ref ConfigValueContainer "config value container".
    */
    void SettingsConfigFile::removeConfigValueContainer(ConfigValueContainer* container)
    {
        if (container == NULL)
            return;
        const std::string& sectionLC = getLowercase(container->getSectionName());
        ContainerMap::iterator upper = this->containers_.upper_bound(sectionLC);
        for (ContainerMap::iterator it = this->containers_.lower_bound(sectionLC); it != upper; ++it)
        {
            if (it->second.second == container)
            {
                // Remove entry from section name set this was the last container for that section
                if (upper == this->containers_.lower_bound(sectionLC))
                    this->sectionNames_.erase(container->getSectionName());
                this->containers_.erase(it);
                break;
            }
        }
    }

    /**
        @brief Updates all @ref ConfigValueContainer "config value containers".
    */
    void SettingsConfigFile::updateConfigValues()
    {
        // todo: can this be done more efficiently? looks like some identifiers will be updated multiple times.

        for (ContainerMap::const_iterator it = this->containers_.begin(); it != this->containers_.end(); ++it)
        {
            it->second.second->update();
            it->second.second->getIdentifier()->updateConfigValues();
        }
    }

    /**
        @brief Removes entries and sections from the file that don't exist anymore (i.e. if there's no corresponding @ref ConfigValueContainer "config value container").
        @param bCleanComments If true, comments are also removed from the file
    */
    void SettingsConfigFile::clean(bool bCleanComments)
    {
        for (std::list<ConfigFileSection*>::iterator itSection = this->sections_.begin(); itSection != this->sections_.end(); )
        {
            const std::string& sectionLC = getLowercase((*itSection)->getName());
            ContainerMap::const_iterator lower = this->containers_.lower_bound(sectionLC);
            ContainerMap::const_iterator upper = this->containers_.upper_bound(sectionLC);
            if (lower != upper)
            {
                // The section exists, delete comment
                if (bCleanComments)
                    (*itSection)->setComment("");
                for (std::list<ConfigFileEntry*>::iterator itEntry = (*itSection)->entries_.begin(); itEntry != (*itSection)->entries_.end(); )
                {
                    const std::string& entryLC = getLowercase((*itEntry)->getName());
                    bool bFound = false;
                    for (ContainerMap::const_iterator itContainer = lower; itContainer != upper; ++itContainer)
                    {
                        if (itContainer->second.first == entryLC)
                        {
                            // The config-value exists, delete comment
                            if (bCleanComments)
                                (*itEntry)->setComment("");
                            ++itEntry;
                            bFound = true;
                            break;
                        }
                    }
                    if (!bFound)
                    {
                        // The config-value doesn't exist
                        delete (*itEntry);
                        (*itSection)->entries_.erase(itEntry++);
                    }
                }
                ++itSection;
            }
            else
            {
                // The section doesn't exist
                delete (*itSection);
                this->sections_.erase(itSection++);
            }
        }

        // Save the file
        this->save();
    }

    /**
        @brief Console-command: Changes the value of an entry and stores it the file.

        @param section  The section of the config value
        @param entry    The name of the config value
        @param value    The new value
    */
    void SettingsConfigFile::config(const std::string& section, const std::string& entry, const std::string& value)
    {
        if (!this->configImpl(section, entry, value, &ConfigValueContainer::set))
            orxout(user_error, context::config) << "Config value \"" << entry << "\" in section \"" << section << "\" doesn't exist." << endl;
    }

    /**
        @brief Console-command: Changes the value of an entry, but doesn't store it in the file (it's only a temporary change).

        @param section  The section of the config value
        @param entry    The name of the config value
        @param value    The new value
    */
    void SettingsConfigFile::tconfig(const std::string& section, const std::string& entry, const std::string& value)
    {
        if (!this->configImpl(section, entry, value, &ConfigValueContainer::tset))
            orxout(user_error, context::config) << "Config value \"" << entry << "\" in section \"" << section << "\" doesn't exist." << endl;
    }

    /**
        @brief Changes the value of an entry, depending on @a function, either by using "set" or "tset"

        @param section  The section of the config value
        @param entry    The name of the config value
        @param value    The new value
        @param function The function ("set" or "tset") that will be used to change the value.
    */
    bool SettingsConfigFile::configImpl(const std::string& section, const std::string& entry, const std::string& value, bool (ConfigValueContainer::*function)(const MultiType&))
    {
        const std::string& sectionLC = getLowercase(section);
        const std::string& entryLC = getLowercase(entry);
        ContainerMap::iterator upper = this->containers_.upper_bound(sectionLC);
        for (ContainerMap::iterator it = this->containers_.lower_bound(sectionLC); it != upper; ++it)
        {
            // Note: Config value vectors cannot be supported
            if (it->second.first == entryLC && !it->second.second->isVector())
            {
                return (it->second.second->*function)(value);
            }
        }
        return false;
    }

    /**
        @brief Console-command: Returns the value of a given entry.

        @param section  The section of the config value
        @param entry    The name of the config value
    */
    std::string SettingsConfigFile::getConfig(const std::string& section, const std::string& entry)
    {
        const std::string& sectionLC = getLowercase(section);
        const std::string& entryLC = getLowercase(entry);
        ContainerMap::iterator upper = this->containers_.upper_bound(sectionLC);
        for (ContainerMap::iterator it = this->containers_.lower_bound(sectionLC); it != upper; ++it)
        {
            // Note: Config value vectors cannot be supported
            if (it->second.first == entryLC && ! it->second.second->isVector())
            {
                std::string value;
                it->second.second->getValue<std::string, void>(&value, NULL);
                return value;
            }
        }
        return "";
    }
}
