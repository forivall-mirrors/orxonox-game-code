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
    void ConfigFileEntryVectorValue::update()
    {
        this->keyString_ = this->name_ + '[' + multi_cast<std::string>(this->index_) + ']';
        ConfigFileEntryValue::update();
    }


    ///////////////////////
    // ConfigFileSection //
    ///////////////////////
    ConfigFileSection::~ConfigFileSection()
    {
        for (std::list<ConfigFileEntry*>::iterator it = this->entries_.begin(); it != this->entries_.end(); )
            delete (*(it++));
    }

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

    unsigned int ConfigFileSection::getVectorSize(const std::string& name) const
    {
        unsigned int size = 0;
        for (std::list<ConfigFileEntry*>::const_iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
            if ((*it)->getName() == name)
                if ((*it)->getIndex() > size)
                    size = (*it)->getIndex();
        if (size == 0)
            return 0;
        else
            return (size + 1);
    }

    std::string ConfigFileSection::getFileEntry() const
    {
        if (this->additionalComment_.empty())
            return ('[' + this->name_ + ']');
        else
            return ('[' + this->name_ + "] " + this->additionalComment_);
    }

    ConfigFileEntry* ConfigFileSection::getEntry(const std::string& name) const
    {
        for (std::list<ConfigFileEntry*>::const_iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
        {
            if ((*it)->getName() == name)
                return *it;
        }
        return NULL;
    }

    ConfigFileEntry* ConfigFileSection::getEntry(const std::string& name, unsigned int index) const
    {
        for (std::list<ConfigFileEntry*>::const_iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
        {
            if (((*it)->getName() == name) && ((*it)->getIndex() == index))
                return *it;
        }
        return NULL;
    }

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

    ConfigFile::ConfigFile(const std::string& filename, bool bCopyFallbackFile)
        : filename_(filename)
        , bCopyFallbackFile_(bCopyFallbackFile)
        , bUpdated_(false)
    {
    }

    ConfigFile::~ConfigFile()
    {
        this->clear();
    }

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
                            COUT(3) << "Copied " << this->filename_ << " from the default config folder." << std::endl;
                        }
                        catch (const boost::filesystem::filesystem_error& ex)
                        { COUT(1) << "Error in ConfigFile: " << ex.what() << std::endl; }
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
                                value = removeTrailingWhitespaces(line.substr(pos1 + 1));
                            }
                            else
                            {
                                value = removeTrailingWhitespaces(line.substr(pos1 + 1, commentposition - pos1 - 1));
                                comment = removeTrailingWhitespaces(line.substr(commentposition));
                            }

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

            COUT(3) << "Loaded config file \"" << this->filename_ << "\"." << std::endl;

            // DO NOT save the file --> we can open supposedly read only config files
        } // end file.is_open()
    }

    void ConfigFile::save() const
    {
        this->saveAs(this->filename_);
    }

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
            COUT(1) << "Error: Couldn't open config-file \"" << filename << "\"." << std::endl;
            return;
        }

        for (std::list<ConfigFileSection*>::const_iterator it = this->sections_.begin(); it != this->sections_.end(); ++it)
        {
            file << (*it)->getFileEntry() << std::endl;

            for (std::list<ConfigFileEntry*>::const_iterator it_entries = (*it)->getEntriesBegin(); it_entries != (*it)->getEntriesEnd(); ++it_entries)
                file << (*it_entries)->getFileEntry() << std::endl;

            file << std::endl;
        }

        file.close();

        COUT(4) << "Saved config file \"" << filename << "\"." << std::endl;
    }

    void ConfigFile::clear()
    {
        for (std::list<ConfigFileSection*>::iterator it = this->sections_.begin(); it != this->sections_.end(); )
            delete (*(it++));
        this->sections_.clear();
    }

    const std::string& ConfigFile::getOrCreateValue(const std::string& section, const std::string& name, const std::string& fallback, bool bString)
    {
        const std::string& output = this->getOrCreateSection(section)->getOrCreateValue(name, fallback, bString);
        this->saveIfUpdated();
        return output;
    }

    const std::string& ConfigFile::getOrCreateValue(const std::string& section, const std::string& name, unsigned int index, const std::string& fallback, bool bString)
    {
        const std::string& output = this->getOrCreateSection(section)->getOrCreateValue(name, index, fallback, bString);
        this->saveIfUpdated();
        return output;
    }

    void ConfigFile::deleteVectorEntries(const std::string& section, const std::string& name, unsigned int startindex)
    {
        if (ConfigFileSection* sectionPtr = this->getSection(section))
        {
            sectionPtr->deleteVectorEntries(name, startindex);
            this->save();
        }
    }

    ConfigFileSection* ConfigFile::getSection(const std::string& section) const
    {
        for (std::list<ConfigFileSection*>::const_iterator it = this->sections_.begin(); it != this->sections_.end(); ++it)
            if ((*it)->getName() == section)
                return (*it);
        return NULL;
    }

    ConfigFileSection* ConfigFile::getOrCreateSection(const std::string& section)
    {
        for (std::list<ConfigFileSection*>::iterator it = this->sections_.begin(); it != this->sections_.end(); ++it)
            if ((*it)->getName() == section)
                return (*it);

        this->bUpdated_ = true;

        return (*this->sections_.insert(this->sections_.end(), new ConfigFileSection(section)));
    }

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

    SettingsConfigFile* SettingsConfigFile::singletonPtr_s = 0;

    SettingsConfigFile::SettingsConfigFile(const std::string& filename)
        : ConfigFile(filename)
    {
        ConsoleCommand* command = createConsoleCommand(createFunctor(&ConfigFile::load, this), "reloadSettings");
        CommandExecutor::addConsoleCommandShortcut(command);
        command = createConsoleCommand(createFunctor(&SettingsConfigFile::setFilename, this), "setSettingsFile");
        CommandExecutor::addConsoleCommandShortcut(command);
        command = createConsoleCommand(createFunctor(&SettingsConfigFile::config, this), "config");
        CommandExecutor::addConsoleCommandShortcut(command).argumentCompleter(0, autocompletion::settingssections()).argumentCompleter(1, autocompletion::settingsentries()).argumentCompleter(2, autocompletion::settingsvalue());
        command = createConsoleCommand(createFunctor(&SettingsConfigFile::tconfig, this), "tconfig");
        CommandExecutor::addConsoleCommandShortcut(command).argumentCompleter(0, autocompletion::settingssections()).argumentCompleter(1, autocompletion::settingsentries()).argumentCompleter(2, autocompletion::settingsvalue());
        command = createConsoleCommand(createFunctor(&SettingsConfigFile::getConfig, this), "getConfig");
        CommandExecutor::addConsoleCommandShortcut(command).argumentCompleter(0, autocompletion::settingssections()).argumentCompleter(1, autocompletion::settingsentries());
    }

    SettingsConfigFile::~SettingsConfigFile()
    {
    }

    void SettingsConfigFile::load()
    {
        ConfigFile::load();
        this->updateConfigValues();
    }

    void SettingsConfigFile::setFilename(const std::string& filename)
    {
        ConfigFileManager::getInstance().setFilename(ConfigFileType::Settings, filename);
    }

    void SettingsConfigFile::addConfigValueContainer(ConfigValueContainer* container)
    {
        if (container == NULL)
            return;
        std::pair<std::string, ConfigValueContainer*> second(getLowercase(container->getName()), container);
        this->containers_.insert(std::make_pair(getLowercase(container->getSectionName()), second));
        this->sectionNames_.insert(container->getSectionName());
    }

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

    void SettingsConfigFile::updateConfigValues()
    {
        for (ContainerMap::const_iterator it = this->containers_.begin(); it != this->containers_.end(); ++it)
        {
            it->second.second->update();
            it->second.second->getIdentifier()->updateConfigValues();
        }
    }

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

    void SettingsConfigFile::config(const std::string& section, const std::string& entry, const std::string& value)
    {
        if (!this->configImpl(section, entry, value, &ConfigValueContainer::set))
            COUT(1) << "Error: Config value \"" << entry << "\" in section \"" << section << "\" doesn't exist." << std::endl;
    }

    void SettingsConfigFile::tconfig(const std::string& section, const std::string& entry, const std::string& value)
    {
        if (!this->configImpl(section, entry, value, &ConfigValueContainer::tset))
            COUT(1) << "Error: Config value \"" << entry << "\" in section \"" << section << "\" doesn't exist." << std::endl;
    }

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

    ConfigFileManager::ConfigFileManager()
    {
        this->configFiles_.assign(NULL);
    }

    ConfigFileManager::~ConfigFileManager()
    {
        for (boost::array<ConfigFile*, 3>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            if (*it)
                delete (*it);
    }

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
