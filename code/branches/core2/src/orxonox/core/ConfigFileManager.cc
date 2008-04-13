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
    ConsoleCommandShortcutExtern(cleanConfig, AccessLevel::None);
    ConsoleCommandShortcutExtern(loadSettings, AccessLevel::None);
    ConsoleCommandShortcutExtern(loadKeybindings, AccessLevel::None);

    void reloadConfig()
    {
        ConfigFileManager::getSingleton()->load();
    }

    void cleanConfig()
    {
        ConfigFileManager::getSingleton()->clean(false);
    }

    void loadSettings(const std::string& filename)
    {
        ConfigFileManager::getSingleton()->setFile(CFT_Settings, filename, false);
    }

    void loadKeybindings(const std::string& filename)
    {
        ConfigFileManager::getSingleton()->setFile(CFT_Keybindings, filename);
    }


    //////////////////////////
    // ConfigFileEntryValue //
    //////////////////////////

    void ConfigFileEntryValue::setValue(const std::string& value)
    {
        if (!this->bString_)
            this->value_ = value;
        else
            this->value_ = "\"" + addSlashes(value) + "\"";
    }

    std::string ConfigFileEntryValue::getValue() const
    {
        if (!this->bString_)
            return this->value_;
        else
            return removeSlashes(stripEnclosingQuotes(this->value_));
    }

    std::string ConfigFileEntryValue::getFileEntry() const
    {
        if (this->additionalComment_ == "" || this->additionalComment_.size() == 0)
            return (this->name_ + "=" + this->value_);
        else
            return (this->name_ + "=" + this->value_ + " " + this->additionalComment_);
    }


    ///////////////////////////////
    // ConfigFileEntryVectorValue //
    ///////////////////////////////
    std::string ConfigFileEntryVectorValue::getFileEntry() const
    {
        if (this->additionalComment_ == "" || this->additionalComment_.size() == 0)
            return (this->name_ + "[" + getConvertedValue<unsigned int, std::string>(this->index_, "0") + "]" + "=" + this->value_);
        else
            return (this->name_ + "[" + getConvertedValue<unsigned int, std::string>(this->index_, "0") + "]=" + this->value_ + " " + this->additionalComment_);
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

    unsigned int ConfigFileSection::getVectorSize(const std::string& name)
    {
        unsigned int size = 0;
        for (std::list<ConfigFileEntry*>::const_iterator it = this->entries_.begin(); it != this->entries_.end(); ++it)
            if ((*it)->getName() == name)
                if ((*it)->getIndex() > size)
                    size = (*it)->getIndex();
        return (size + 1);
    }

    std::string ConfigFileSection::getFileEntry() const
    {
        if (this->additionalComment_ == "" || this->additionalComment_.size() == 0)
            return ("[" + this->name_ + "]");
        else
            return ("[" + this->name_ + "] " + this->additionalComment_);
    }

    std::list<ConfigFileEntry*>::iterator ConfigFileSection::getEntryIterator(const std::string& name, const std::string& fallback, bool bString)
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

        return this->entries_.insert(this->entries_.end(), (ConfigFileEntry*)(new ConfigFileEntryValue(name, fallback, bString)));
    }

    std::list<ConfigFileEntry*>::iterator ConfigFileSection::getEntryIterator(const std::string& name, unsigned int index, const std::string& fallback, bool bString)
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
            return this->entries_.insert(this->entries_.end(), (ConfigFileEntry*)(new ConfigFileEntryVectorValue(name, index, fallback, bString)));
        else
            return this->entries_.insert(++this->getEntryIterator(name, index - 1, "", bString), (ConfigFileEntry*)(new ConfigFileEntryVectorValue(name, index, fallback, bString)));
    }


    ////////////////
    // ConfigFile //
    ////////////////
    ConfigFile::~ConfigFile()
    {
        for (std::list<ConfigFileSection*>::iterator it = this->sections_.begin(); it != this->sections_.end(); )
            delete (*(it++));
    }

    void ConfigFile::load(bool bCreateIfNotExisting)
    {
        if (bCreateIfNotExisting)
        {
            // This creates the default config file if it's not existing
            std::ofstream createFile;
            createFile.open(this->filename_.c_str(), std::fstream::app);
            createFile.close();
        }

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
                unsigned int   pos1 = temp.find('[');
                if (pos1 == 0) pos1 = line.find('['); else pos1 = std::string::npos;
                unsigned int   pos2 = line.find(']');

                if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1 + 1)
                {
                    // New section
                    std::string comment = line.substr(pos2 + 1);
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
                        unsigned int commentposition = getNextCommentPosition(line, pos1 + 1);
                        while (isBetweenQuotes(line, commentposition))
                        {
                            commentposition = getNextCommentPosition(line, commentposition + 1);
                        }
                        std::string value = "", comment = "";
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
                            if (ConvertValue(&index, line.substr(pos2 + 1, pos3 - pos2 - 1)))
                            {
                                // New array
                                std::list<ConfigFileEntry*>::iterator it = newsection->getEntryIterator(getStripped(line.substr(0, pos2)), index, value, false);
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

    void ConfigFile::clean(bool bCleanComments)
    {
        for (std::list<ConfigFileSection*>::iterator it1 = this->sections_.begin(); it1 != this->sections_.end(); )
        {
            std::map<std::string, Identifier*>::const_iterator it2 = Identifier::getIdentifierMap().find((*it1)->getName());
            if (it2 != Identifier::getIdentifierMapEnd() && (*it2).second->hasConfigValues())
            {
                // The section exists, delete comment
                if (bCleanComments)
                    (*it1)->setComment("");
                for (std::list<ConfigFileEntry*>::iterator it3 = (*it1)->entries_.begin(); it3 != (*it1)->entries_.end(); )
                {
                    std::map<std::string, ConfigValueContainer*>::const_iterator it4 = (*it2).second->getConfigValueMap().find((*it3)->getName());
                    if (it4 != (*it2).second->getConfigValueMapEnd())
                    {
                        // The config-value exists, delete comment
                        if (bCleanComments)
                            (*it3)->setComment("");
                        ++it3;
                    }
                    else
                    {
                        // The config-value doesn't exist
                        delete (*it3);
                        (*it1)->entries_.erase(it3++);
                    }
                }
                ++it1;
            }
            else
            {
                // The section doesn't exist
                delete (*it1);
                this->sections_.erase(it1++);
            }
        }

        // Save the file
        this->save();
    }

    ConfigFileSection* ConfigFile::getSection(const std::string& section)
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

    void ConfigFileManager::setFile(ConfigFileType type, const std::string& filename, bool bCreateIfNotExisting)
    {
        std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.find(type);
        if (it != this->configFiles_.end())
            if ((*it).second != 0)
                delete (*it).second;

        this->configFiles_[type] = new ConfigFile(this->getFilePath(filename));
        this->load(type, bCreateIfNotExisting);
    }

    void ConfigFileManager::load(bool bCreateIfNotExisting)
    {
        for(std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            (*it).second->load(bCreateIfNotExisting);

        this->updateConfigValues();
    }

    void ConfigFileManager::save()
    {
        for(std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            (*it).second->save();
    }

    void ConfigFileManager::clean(bool bCleanComments)
    {
        for(std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            this->clean((*it).first, bCleanComments);
    }

    void ConfigFileManager::load(ConfigFileType type, bool bCreateIfNotExisting)
    {
        this->getFile(type)->load(bCreateIfNotExisting);
        this->updateConfigValues(type);
    }

    void ConfigFileManager::save(ConfigFileType type)
    {
        this->getFile(type)->save();
    }

    void ConfigFileManager::clean(ConfigFileType type, bool bCleanComments)
    {
        this->getFile(type)->clean(bCleanComments);
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
