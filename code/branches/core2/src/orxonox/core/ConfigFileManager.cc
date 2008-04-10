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

#include "ConfigFileManager.h"
#include "ConfigValueContainer.h"
#include "ConsoleCommand.h"
#include "Identifier.h"
#include "util/Convert.h"
#include "util/String.h"

#define CONFIG_FILE_MAX_LINELENGHT 1024

namespace orxonox
{
    ConsoleCommandShortcutExtern(reloadConfig, AccessLevel::None);
    ConsoleCommandShortcutExtern(saveConfig, AccessLevel::None);
    ConsoleCommandShortcutExtern(cleanConfig, AccessLevel::None);
    ConsoleCommandShortcutExtern(loadSettings, AccessLevel::None);
    ConsoleCommandShortcutExtern(loadKeybindings, AccessLevel::None);

    void reloadConfig()
    {
        ConfigFileManager::getSingleton()->load();
    }

    void saveConfig()
    {
        ConfigFileManager::getSingleton()->save();
    }

    void cleanConfig()
    {
        ConfigFileManager::getSingleton()->clean();
    }

    void loadSettings(const std::string& filename)
    {
        ConfigFileManager::getSingleton()->setFile(CFT_Settings, filename);
    }

    void loadKeybindings(const std::string& filename)
    {
        ConfigFileManager::getSingleton()->setFile(CFT_Keybindings, filename);
    }


    //////////////////////////
    // ConfigFileEntryValue //
    //////////////////////////
    std::string ConfigFileEntryValue::getFileEntry() const
    {
        if (this->additionalComment_ == "" || this->additionalComment_.size() == 0)
            return (this->name_ + "=" + this->value_);
        else
            return (this->name_ + "=" + this->value_ + " " + this->additionalComment_);
    }


    ///////////////////////////////
    // ConfigFileEntryArrayValue //
    ///////////////////////////////
    std::string ConfigFileEntryArrayValue::getFileEntry() const
    {
        if (this->additionalComment_ == "" || this->additionalComment_.size() == 0)
            return (this->name_ + "[" + getConvertedValue<unsigned int, std::string>(this->index_, 0) + "]" + "=" + this->value_);
        else
            return (this->name_ + "[" + getConvertedValue<unsigned int, std::string>(this->index_, 0) + "]=" + this->value_ + " " + this->additionalComment_);
    }


    ///////////////////////
    // ConfigFileSection //
    ///////////////////////
    ConfigFileSection::~ConfigFileSection()
    {
        for (std::list<ConfigFileEntry*>::iterator it = this->entries_.begin(); it != this->entries_.end(); )
            delete (*(it++));
    }

    std::string ConfigFileSection::getFileEntry() const
    {
        if (this->additionalComment_ == "" || this->additionalComment_.size() == 0)
            return ("[" + this->name_ + "]");
        else
            return ("[" + this->name_ + "] " + this->additionalComment_);
    }

    std::list<ConfigFileEntry*>::iterator ConfigFileSection::getEntryIterator(const std::string& name, const std::string& fallback)
    {
        for (std::list<ConfigFileEntry*>::iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
            if ((*it)->getName() == name)
                return it;

        this->bUpdated_ = true;

        return this->entries_.insert(this->entries_.end(), (ConfigFileEntry*)(new ConfigFileEntryValue(name, fallback)));
    }

    std::list<ConfigFileEntry*>::iterator ConfigFileSection::getEntryIterator(const std::string& name, unsigned int index, const std::string& fallback)
    {
        for (std::list<ConfigFileEntry*>::iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
            if (((*it)->getName() == name) && ((*it)->getIndex() == index))
                return it;

        this->bUpdated_ = true;

        if (index == 0)
            return this->entries_.insert(this->entries_.end(), (ConfigFileEntry*)(new ConfigFileEntryArrayValue(name, index, fallback)));
        else
            return this->entries_.insert(this->getEntryIterator(name, index - 1), (ConfigFileEntry*)(new ConfigFileEntryArrayValue(name, index, fallback)));
    }


    ////////////////
    // ConfigFile //
    ////////////////
    ConfigFile::~ConfigFile()
    {
        for (std::list<ConfigFileSection*>::iterator it = this->sections_.begin(); it != this->sections_.end(); )
            delete (*(it++));
    }

    void ConfigFile::load()
    {
        // This creates the file if it's not existing
        std::ofstream createFile;
        createFile.open(this->filename_.c_str(), std::fstream::app);
        createFile.close();

        // Open the file
        std::ifstream file;
        file.open(this->filename_.c_str(), std::fstream::in);

        if (!file.is_open())
        {
            COUT(1) << "An error occurred in ConfigFileManager.cc:" << std::endl;
            COUT(1) << "Error: Couldn't open config-file \"" << this->filename_ << "\"." << std::endl;
            return;
        }

        char linearray[CONFIG_FILE_MAX_LINELENGHT];

        ConfigFileSection* newsection = 0;

        while (file.good() && !file.eof())
        {
            file.getline(linearray, CONFIG_FILE_MAX_LINELENGHT);

            std::string line = std::string(linearray);

            std::string temp = getStripped(line);
            if (!isEmpty(temp) && !isComment(temp))
            {
                unsigned int pos1 = line.find('[');
                unsigned int pos2 = line.find(']');

                if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1 + 1)
                {
                    // New section
                    std::string comment = temp.substr(pos2 + 1);
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
                    unsigned int pos1 = line.find('=');

                    if (pos1 != std::string::npos && pos1 > 0)
                    {
                        // New entry
                        unsigned int pos2 = line.find('[');
                        unsigned int pos3 = line.find(']');

                        std::string value = removeTrailingWhitespaces(line.substr(pos1 + 1));
                        std::string comment = "";

                        std::string betweenQuotes = getStringBetweenQuotes(value);
                        if (value.size() > 0 && value[0] == '"' && betweenQuotes != "" && betweenQuotes.size() > 0)
                        {
                            value = betweenQuotes;
                            if (line.size() > pos1 + 1 + betweenQuotes.size() + 2)
                                comment = removeTrailingWhitespaces(getComment(line.substr(pos1 + 1 + betweenQuotes.size() + 2)));
                        }
                        else
                        {
                            unsigned int pos4 = getCommentPosition(line);
                            value = removeTrailingWhitespaces(line.substr(pos1 + 1, pos4 - pos1 - 1));
                            if (pos4 != std::string::npos)
                                comment = removeTrailingWhitespaces(line.substr(pos4));
                        }

                        if (pos2 != std::string::npos && pos3 != std::string::npos && pos3 > pos2 + 1)
                        {
                            // There might be an array index
                            unsigned int index = 0;
                            if (ConvertValue(&index, line.substr(pos2 + 1, pos3 - pos2 - 1)))
                            {
                                // New array
                                newsection->getEntries().insert(newsection->getEntries().end(), new ConfigFileEntryArrayValue(getStripped(line.substr(0, pos2)), index, value, comment));
                                continue;
                            }
                        }

                        // New value
                        newsection->getEntries().insert(newsection->getEntries().end(), new ConfigFileEntryValue(getStripped(line.substr(0, pos1)), value, comment));
                        continue;
                    }
                }
            }
        }

        file.close();

        COUT(3) << "Loaded config file \"" << this->filename_ << "\"." << std::endl;

        // Save the file in case something changed (like stripped whitespaces)
        this->save();
    }

    void ConfigFile::save() const
    {
        std::ofstream file;
        file.open(this->filename_.c_str(), std::fstream::out);
        file.setf(std::ios::fixed, std::ios::floatfield);
        file.precision(6);

        if (!file.is_open())
        {
            COUT(1) << "An error occurred in ConfigFileManager.cc:" << std::endl;
            COUT(1) << "Error: Couldn't open config-file \"" << this->filename_ << "\"." << std::endl;
            return;
        }

        for (std::list<ConfigFileSection*>::const_iterator it = this->sections_.begin(); it != this->sections_.end(); ++it)
        {
            file << (*it)->getFileEntry() << std::endl;

            for (std::list<ConfigFileEntry*>::const_iterator it_entries = (*it)->getEntriesBegin(); it_entries != (*it)->getEntriesEnd(); ++it_entries)
            {
                file << (*it_entries)->getFileEntry() << std::endl;
            }

            file << std::endl;
        }

        file.close();

        COUT(4) << "Saved config file \"" << this->filename_ << "\"." << std::endl;
    }

    void ConfigFile::clean()
    {
    }

    ConfigFileSection* ConfigFile::getSection(const std::string& section)
    {
        for (std::list<ConfigFileSection*>::iterator it = this->sections_.begin(); it != this->sections_.end(); ++it)
            if ((*it)->getName() == section)
                return (*it);

        this->bUpdated_ = true;

        return (*this->sections_.insert(this->sections_.begin(), new ConfigFileSection(section)));
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


    ///////////////////////
    // ConfigFileManager //
    ///////////////////////
    ConfigFileManager::ConfigFileManager()
    {
        this->setFile(CFT_Settings, DEFAULT_CONFIG_FILE);
    }

    ConfigFileManager::~ConfigFileManager()
    {
        for(std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); )
            delete (*(it++)).second;
    }

    ConfigFileManager* ConfigFileManager::getSingleton()
    {
        static ConfigFileManager instance;
        return (&instance);
    }

    void ConfigFileManager::setFile(ConfigFileType type, const std::string& filename)
    {
        std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.find(type);
        if (it != this->configFiles_.end())
            if ((*it).second != 0)
                delete (*it).second;

        this->configFiles_[type] = new ConfigFile(this->getFilePath(filename));
        this->load(type);
    }

    void ConfigFileManager::load()
    {
        for(std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            (*it).second->load();

        this->updateConfigValues();
    }

    void ConfigFileManager::save()
    {
        for(std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            (*it).second->save();
    }

    void ConfigFileManager::clean()
    {
        for(std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            this->clean((*it).first);
    }

    void ConfigFileManager::load(ConfigFileType type)
    {
        this->getFile(type)->load();
        this->updateConfigValues(type);
    }

    void ConfigFileManager::save(ConfigFileType type)
    {
        this->getFile(type)->save();
    }

    void ConfigFileManager::clean(ConfigFileType type)
    {
        this->getFile(type)->clean();
        this->getFile(type)->save();
    }

    void ConfigFileManager::updateConfigValues() const
    {
        for(std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            this->updateConfigValues((*it).first);
    }

    void ConfigFileManager::updateConfigValues(ConfigFileType type) const
    {
        if (type == CFT_Settings)
        {
            for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getIdentifierMapBegin(); it != Identifier::getIdentifierMapEnd(); ++it)
            {
                if ((*it).second->hasConfigValues() /* && (*it).second != ClassManager<KeyBinder>::getIdentifier()*/)
                {
                    for (std::map<std::string, ConfigValueContainer*>::const_iterator it2 = (*it).second->getConfigValueMapBegin(); it2 != (*it).second->getConfigValueMapEnd(); ++it2)
                        (*it2).second->update();

                    (*it).second->updateConfigValues();
                }
            }
        }
        else if (type == CFT_Keybindings)
        {
            // todo
        }
    }

    ConfigFile* ConfigFileManager::getFile(ConfigFileType type)
    {
        std::map<ConfigFileType, ConfigFile*>::iterator it = this->configFiles_.find(type);
        if (it != this->configFiles_.end())
            return (*it).second;

        if (type == CFT_Settings)
            return this->configFiles_[type] = new ConfigFile(DEFAULT_CONFIG_FILE);
        else
            return this->configFiles_[type] = new ConfigFile("");
    }

    std::string ConfigFileManager::getFilePath(const std::string& name) const
    {
        return name;
    }
}
