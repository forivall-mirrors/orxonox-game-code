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
    @brief Implementation of ConfigFileManager and its helper classes.
*/

#include "ConfigFileManager.h"

#include <boost/filesystem.hpp>

#include "util/Convert.h"
#include "util/Math.h"
#include "util/StringUtils.h"
#include "ConfigValueContainer.h"
#include "PathConfig.h"
#include "command/ConsoleCommand.h"

namespace orxonox
{
    //////////////////////////
    // ConfigFileEntryValue //
    //////////////////////////
    /**
        @brief Updates the string that will be stored in the file after one of it's components (name, value, comment) has changed.
    */
    void ConfigFileEntryValue::update()
    {
        // Make sure we remove the quotes when bString changes
        if (this->bString_)
            this->value_ = stripEnclosingQuotes(this->value_);
        // Assemble the entry line
        this->fileEntry_ = this->getKeyString() + " = ";
        if (this->bString_ && !this->value_.empty())
            this->fileEntry_ += '"' + addSlashes(this->value_) + '"';
        else
            this->fileEntry_ += this->value_;
        if (!this->additionalComment_.empty())
            this->fileEntry_ += ' ' + this->additionalComment_;
    }


    ////////////////////////////////
    // ConfigFileEntryVectorValue //
    ////////////////////////////////
    /**
        @brief Updates the string that will be stored in the file after one of it's components (name, value, index, comment) has changed.
    */
    void ConfigFileEntryVectorValue::update()
    {
        this->keyString_ = this->name_ + '[' + multi_cast<std::string>(this->index_) + ']';
        ConfigFileEntryValue::update();
    }


    ///////////////////////
    // ConfigFileSection //
    ///////////////////////
    /**
        @brief Destructor: Deletes all entries.
    */
    ConfigFileSection::~ConfigFileSection()
    {
        for (std::list<ConfigFileEntry*>::iterator it = this->entries_.begin(); it != this->entries_.end(); )
            delete (*(it++));
    }

    /**
        @brief Deletes all elements of a config vector if their index is greater or equal to @a startindex.

        @param name         The name of the vector
        @param startindex   The index of the first element that will be deleted
    */
    void ConfigFileSection::deleteVectorEntries(const std::string& name, unsigned int startindex)
    {
        for (std::list<ConfigFileEntry*>::iterator it = this->entries_.begin(); it != this->entries_.end(); )
        {
            if (((*it)->getName() == name) && ((*it)->getIndex() >= startindex))
            {
                delete (*it);
                this->entries_.erase(it++);
            }
            else
            {
                ++it;
            }
        }
    }

    /**
        @brief Returns the size of a config vector.
        @param name     The name of the vector
    */
    unsigned int ConfigFileSection::getVectorSize(const std::string& name) const
    {
        unsigned int size = 0;
        for (std::list<ConfigFileEntry*>::const_iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
            if ((*it)->getName() == name)
                if ((*it)->getIndex() >= size)
                    size = (*it)->getIndex() + 1;
        return size;
    }

    /**
        @brief Returns the title and comment of the section as it will be stored in the file.
    */
    std::string ConfigFileSection::getFileEntry() const
    {
        if (this->additionalComment_.empty())
            return ('[' + this->name_ + ']');
        else
            return ('[' + this->name_ + "] " + this->additionalComment_);
    }

    /**
        @brief Returns the entry with given name (or NULL if it doesn't exist).

        @param name     The name of the entry
    */
    ConfigFileEntry* ConfigFileSection::getEntry(const std::string& name) const
    {
        for (std::list<ConfigFileEntry*>::const_iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
        {
            if ((*it)->getName() == name)
                return *it;
        }
        return NULL;
    }

    /**
        @brief Returns the entry of a vector element with given name and index (or NULL if it doesn't exist).

        @param name     The name of the vector
        @param index    The index of the element in the vector
    */
    ConfigFileEntry* ConfigFileSection::getEntry(const std::string& name, unsigned int index) const
    {
        for (std::list<ConfigFileEntry*>::const_iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
        {
            if (((*it)->getName() == name) && ((*it)->getIndex() == index))
                return *it;
        }
        return NULL;
    }

    /**
        @brief Returns the iterator to the entry with given name. If the entry doesn't exist, it is created using the fallback value.

        @param name     The name of the entry
        @param fallback The value that will be used if the entry doesn't exist
        @param bString  If true, the value is treated as string which means some special treatment of special characters.
    */
    std::list<ConfigFileEntry*>::iterator ConfigFileSection::getOrCreateEntryIterator(const std::string& name, const std::string& fallback, bool bString)
    {
        for (std::list<ConfigFileEntry*>::iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
        {
            if ((*it)->getName() == name)
            {
                (*it)->setString(bString);
                return it;
            }
        }

        this->bUpdated_ = true;

        return this->entries_.insert(this->entries_.end(), new ConfigFileEntryValue(name, fallback, bString));
    }

    /**
        @brief Returns the iterator to the entry of a vector element with given name and index. If the entry doesn't exist, it is created using the fallback value.

        @param name     The name of the vector
        @param index    The index of the element in the vector
        @param fallback The value that will be used if the entry doesn't exist
        @param bString  If true, the value is treated as string which means some special treatment of special characters.
    */
    std::list<ConfigFileEntry*>::iterator ConfigFileSection::getOrCreateEntryIterator(const std::string& name, unsigned int index, const std::string& fallback, bool bString)
    {
        for (std::list<ConfigFileEntry*>::iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
        {
            if (((*it)->getName() == name) && ((*it)->getIndex() == index))
            {
                (*it)->setString(bString);
                return it;
            }
        }

        this->bUpdated_ = true;

        if (index == 0)
            return this->entries_.insert(this->entries_.end(), new ConfigFileEntryVectorValue(name, index, fallback, bString));
        else
            return this->entries_.insert(++this->getOrCreateEntryIterator(name, index - 1, "", bString), new ConfigFileEntryVectorValue(name, index, fallback, bString));
    }


    ////////////////
    // ConfigFile //
    ////////////////

    const char* ConfigFile::DEFAULT_CONFIG_FOLDER = "defaultConfig";

    /**
        @brief Constructor: Initializes the config file.
        @param filename The file-name of this config file
        @param bCopyFallbackFile If true, the default config file is copied into the config-directory before loading the file
    */
    ConfigFile::ConfigFile(const std::string& filename, bool bCopyFallbackFile)
        : filename_(filename)
        , bCopyFallbackFile_(bCopyFallbackFile)
        , bUpdated_(false)
    {
    }

    /**
        @brief Destructor: Deletes all sections and entries.
    */
    ConfigFile::~ConfigFile()
    {
        this->clear();
    }

    /**
        @brief Loads the config file from the hard-disk and reads the sections and their values.
    */
    void ConfigFile::load()
    {
        // Be sure we start from new in the memory
        this->clear();

        boost::filesystem::path filepath(this->filename_);
        if (!filepath.is_complete())
        {
            filepath = PathConfig::getConfigPath() / filepath;
            if (this->bCopyFallbackFile_)
            {
                // Look for default file in the data folder
                if (!boost::filesystem::exists(filepath))
                {
                    boost::filesystem::path defaultFilepath(PathConfig::getDataPath() / DEFAULT_CONFIG_FOLDER / this->filename_);
                    if (boost::filesystem::exists(defaultFilepath))
                    {
                        // Try to copy default file from the data folder
                        try
                        {
                            boost::filesystem::copy_file(defaultFilepath, filepath);
                            orxout(internal_info, context::config) << "Copied " << this->filename_ << " from the default config folder." << endl;
                        }
                        catch (const boost::filesystem::filesystem_error& ex)
                        { orxout(user_error, context::config) << "Error in ConfigFile: " << ex.what() << endl; }
                    }
                }
            }
        }

        // Open the file
        std::ifstream file;
        file.open(filepath.string().c_str(), std::fstream::in);
        if (file.is_open())
        {
            ConfigFileSection* newsection = 0;

            while (file.good() && !file.eof())
            {
                std::string line;
                std::getline(file, line);

                const std::string& temp = getStripped(line);
                if (!isEmpty(temp) && !isComment(temp))
                {
                    size_t   pos1 = temp.find('[');
                    if (pos1 == 0) pos1 = line.find('['); else pos1 = std::string::npos;
                    size_t   pos2 = line.find(']');

                    if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1 + 1)
                    {
                        // New section
                        const std::string& comment = line.substr(pos2 + 1);
                        if (isComment(comment))
                            newsection = new ConfigFileSection(line.substr(pos1 + 1, pos2 - pos1 - 1), comment);
                        else
                            newsection = new ConfigFileSection(line.substr(pos1 + 1, pos2 - pos1 - 1));
                        this->sections_.insert(this->sections_.end(), newsection);
                        continue;
                    }
                }

                if (newsection != 0)
                {
                    if (isComment(line))
                    {
                        // New comment
                        newsection->getEntries().insert(newsection->getEntries().end(), new ConfigFileEntryComment(removeTrailingWhitespaces(line)));
                        continue;
                    }
                    else
                    {
                        size_t pos1 = line.find('=');

                        if (pos1 != std::string::npos && pos1 > 0)
                        {
                            // New entry
                            size_t pos2 = line.find('[');
                            size_t pos3 = line.find(']');
                            size_t commentposition = getNextCommentPosition(line, pos1 + 1);
                            while (isBetweenQuotes(line, commentposition))
                            {
                                commentposition = getNextCommentPosition(line, commentposition + 1);
                            }
                            std::string value, comment;
                            if (commentposition == std::string::npos)
                            {
                                value = line.substr(pos1 + 1);
                            }
                            else
                            {
                                value = line.substr(pos1 + 1, commentposition - pos1 - 1);
                                comment = removeTrailingWhitespaces(line.substr(commentposition));
                            }

                            value = removeTrailingWhitespaces(value);
                            value = removeSlashes(value);

                            if (pos2 != std::string::npos && pos3 != std::string::npos && pos3 > pos2 + 1)
                            {
                                // There might be an array index
                                unsigned int index = 0;
                                if (convertValue(&index, line.substr(pos2 + 1, pos3 - pos2 - 1)))
                                {
                                    // New array
                                    std::list<ConfigFileEntry*>::iterator it = newsection->getOrCreateEntryIterator(getStripped(line.substr(0, pos2)), index, value, false);
                                    (*it)->setValue(value);
                                    (*it)->setComment(comment);
                                    continue;
                                }
                            }

                            // New value
                            newsection->getEntries().insert(newsection->getEntries().end(), new ConfigFileEntryValue(getStripped(line.substr(0, pos1)), value, false, comment));
                            continue;
                        }
                    }
                }
            }

            file.close();

            orxout(internal_info, context::config) << "Loaded config file \"" << this->filename_ << "\"." << endl;

            // DO NOT save the file --> we can open supposedly read only config files
        } // end file.is_open()
    }

    /**
        @brief Writes the sections and values to the hard-disk.
    */
    void ConfigFile::save() const
    {
        this->saveAs(this->filename_);
    }

    /**
        @brief Writes the sections and values to a given file on the hard-disk.
    */
    void ConfigFile::saveAs(const std::string& filename) const
    {
        boost::filesystem::path filepath(filename);
        if (!filepath.is_complete())
            filepath = PathConfig::getConfigPath() / filename;
        std::ofstream file;
        file.open(filepath.string().c_str(), std::fstream::out);
        file.setf(std::ios::fixed, std::ios::floatfield);
        file.precision(6);

        if (!file.is_open())
        {
            orxout(user_error, context::config) << "Couldn't open config-file \"" << filename << "\"." << endl;
            return;
        }

        for (std::list<ConfigFileSection*>::const_iterator it = this->sections_.begin(); it != this->sections_.end(); ++it)
        {
            file << (*it)->getFileEntry() << endl;

            for (std::list<ConfigFileEntry*>::const_iterator it_entries = (*it)->getEntriesBegin(); it_entries != (*it)->getEntriesEnd(); ++it_entries)
                file << (*it_entries)->getFileEntry() << endl;

            file << endl;
        }

        file.close();

        orxout(verbose, context::config) << "Saved config file \"" << filename << "\"." << endl;
    }

    /**
        @brief Deletes all sections (which again delete all their values) and clears the list of sections.
    */
    void ConfigFile::clear()
    {
        for (std::list<ConfigFileSection*>::iterator it = this->sections_.begin(); it != this->sections_.end(); )
            delete (*(it++));
        this->sections_.clear();
    }

    /**
        @brief Deletes all elements of a config vector if their index is greater or equal to @a startindex.

        @param section      The name of the section
        @param name         The name of the vector
        @param startindex   The index of the first element that will be deleted
    */
    void ConfigFile::deleteVectorEntries(const std::string& section, const std::string& name, unsigned int startindex)
    {
        if (ConfigFileSection* sectionPtr = this->getSection(section))
        {
            sectionPtr->deleteVectorEntries(name, startindex);
            this->save();
        }
    }

    /**
        @brief Returns a pointer to the section with given name (or NULL if the section doesn't exist).
    */
    ConfigFileSection* ConfigFile::getSection(const std::string& section) const
    {
        for (std::list<ConfigFileSection*>::const_iterator it = this->sections_.begin(); it != this->sections_.end(); ++it)
            if ((*it)->getName() == section)
                return (*it);
        return NULL;
    }

    /**
        @brief Returns a pointer to the section with given name. If it doesn't exist, the section is created.
    */
    ConfigFileSection* ConfigFile::getOrCreateSection(const std::string& section)
    {
        for (std::list<ConfigFileSection*>::iterator it = this->sections_.begin(); it != this->sections_.end(); ++it)
            if ((*it)->getName() == section)
                return (*it);

        this->bUpdated_ = true;

        return (*this->sections_.insert(this->sections_.end(), new ConfigFileSection(section)));
    }

    /**
        @brief Saves the config file if it was updated (or if any of its sections were updated).
    */
    void ConfigFile::saveIfUpdated()
    {
        bool sectionsUpdated = false;

        for (std::list<ConfigFileSection*>::iterator it = this->sections_.begin(); it != this->sections_.end(); ++it)
        {
            if ((*it)->bUpdated_)
            {
                sectionsUpdated = true;
                (*it)->bUpdated_ = false;
            }
        }

        if (this->bUpdated_ || sectionsUpdated)
        {
            this->bUpdated_ = false;
            this->save();
        }
    }


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
                it->second.second->getValue<std::string, OrxonoxClass>(&value, NULL);
                return value;
            }
        }
        return "";
    }


    ///////////////////////
    // ConfigFileManager //
    ///////////////////////

    ConfigFileManager* ConfigFileManager::singletonPtr_s = 0;

    /// Constructor: Initializes the array of config files with NULL.
    ConfigFileManager::ConfigFileManager()
    {
        this->configFiles_.assign(NULL);
    }

    /// Destructor: Deletes the config files.
    ConfigFileManager::~ConfigFileManager()
    {
        for (boost::array<ConfigFile*, 3>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            if (*it)
                delete (*it);
    }

    /// Defines the file-name for the config file of a given type (settings, calibration, etc.).
    void ConfigFileManager::setFilename(ConfigFileType::Value type, const std::string& filename)
    {
        if (this->getConfigFile(type))
            delete this->configFiles_[type];
        // Create and load config file
        switch (type)
        {
        case ConfigFileType::Settings:
            this->configFiles_[type] = new SettingsConfigFile(filename);
            break;
        case ConfigFileType::JoyStickCalibration:
        case ConfigFileType::CommandHistory:
            this->configFiles_[type] = new ConfigFile(filename);
            break;
        }
        this->configFiles_[type]->load();
    }
}
