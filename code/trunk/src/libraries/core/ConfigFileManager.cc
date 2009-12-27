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
#include "ConsoleCommand.h"
#include "ConfigValueContainer.h"
#include "PathConfig.h"

namespace orxonox
{
    SetConsoleCommandShortcutExtern(config).argumentCompleter(0, autocompletion::configvalueclasses()).argumentCompleter(1, autocompletion::configvalues()).argumentCompleter(2, autocompletion::configvalue());
    SetConsoleCommandShortcutExtern(tconfig).argumentCompleter(0, autocompletion::configvalueclasses()).argumentCompleter(1, autocompletion::configvalues()).argumentCompleter(2, autocompletion::configvalue());
    SetConsoleCommandShortcutExtern(reloadConfig);
    SetConsoleCommandShortcutExtern(cleanConfig);
    SetConsoleCommandShortcutExtern(loadSettings).argumentCompleter(0, autocompletion::files());

    bool config(const std::string& classname, const std::string& varname, const std::string& value)
    {
        std::map<std::string, Identifier*>::const_iterator identifier = Identifier::getLowercaseStringIdentifierMap().find(getLowercase(classname));
        if (identifier != Identifier::getLowercaseStringIdentifierMapEnd())
        {
            std::map<std::string, ConfigValueContainer*>::const_iterator variable = identifier->second->getLowercaseConfigValueMap().find(getLowercase(varname));
            if (variable != identifier->second->getLowercaseConfigValueMapEnd())
                return variable->second->set(value);
        }
        return false;
    }

    const std::string& getConfig(const std::string& classname, const std::string& varname)
    {
        return ConfigFileManager::getInstance().getValue(ConfigFileType::Settings, classname, varname, "", true);
    }

    bool tconfig(const std::string& classname, const std::string& varname, const std::string& value)
    {
        std::map<std::string, Identifier*>::const_iterator identifier = Identifier::getLowercaseStringIdentifierMap().find(getLowercase(classname));
        if (identifier != Identifier::getLowercaseStringIdentifierMapEnd())
        {
            std::map<std::string, ConfigValueContainer*>::const_iterator variable = identifier->second->getLowercaseConfigValueMap().find(getLowercase(varname));
            if (variable != identifier->second->getLowercaseConfigValueMapEnd())
                return variable->second->tset(value);
        }
        return false;
    }

    void reloadConfig()
    {
        ConfigFileManager::getInstance().load();
    }

    void cleanConfig()
    {
        ConfigFileManager::getInstance().clean(false);
    }

    void loadSettings(const std::string& filename)
    {
        ConfigFileManager::getInstance().setFilename(ConfigFileType::Settings, filename);
    }

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
        if (this->bString_)
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

    unsigned int ConfigFileSection::getVectorSize(const std::string& name)
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

        return this->entries_.insert(this->entries_.end(), static_cast<ConfigFileEntry*>(new ConfigFileEntryValue(name, fallback, bString)));
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
            return this->entries_.insert(this->entries_.end(), static_cast<ConfigFileEntry*>(new ConfigFileEntryVectorValue(name, index, fallback, bString)));
        else
            return this->entries_.insert(++this->getEntryIterator(name, index - 1, "", bString), static_cast<ConfigFileEntry*>(new ConfigFileEntryVectorValue(name, index, fallback, bString)));
    }


    ////////////////
    // ConfigFile //
    ////////////////
    ConfigFile::~ConfigFile()
    {
        this->clear();
    }

    void ConfigFile::load(bool bCreateIfNotExisting)
    {
        // Be sure we start from new in the memory
        this->clear();

        // Get default file if necessary and available
        boost::filesystem::path filepath(PathConfig::getConfigPath() / this->filename_);
        if (!boost::filesystem::exists(filepath))
        {
            // Try to get default one from the data folder
            boost::filesystem::path defaultFilepath(PathConfig::getDataPath() / "defaultConfig" / this->filename_);
            if (boost::filesystem::exists(defaultFilepath))
            {
                COUT(3) << "Copied " << this->filename_ << " from the defaultConfig folder." << std::endl;
                boost::filesystem::copy_file(defaultFilepath, filepath);
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

            // Update all ConfigValueContainers
            this->updateConfigValues();
        } // end file.is_open()
    }

    void ConfigFile::save() const
    {
        std::ofstream file;
        file.open((PathConfig::getConfigPathString() + filename_).c_str(), std::fstream::out);
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

    void ConfigFile::saveAs(const std::string& filename)
    {
        std::string temp = this->filename_;
        this->filename_ = filename;
        this->save();
        this->filename_ = temp;
    }

    void ConfigFile::clean(bool bCleanComments)
    {
        for (std::list<ConfigFileSection*>::iterator it1 = this->sections_.begin(); it1 != this->sections_.end(); )
        {
            std::map<std::string, Identifier*>::const_iterator it2 = Identifier::getStringIdentifierMap().find((*it1)->getName());
            if (it2 != Identifier::getStringIdentifierMapEnd() && it2->second->hasConfigValues())
            {
                // The section exists, delete comment
                if (bCleanComments)
                    (*it1)->setComment("");
                for (std::list<ConfigFileEntry*>::iterator it3 = (*it1)->entries_.begin(); it3 != (*it1)->entries_.end(); )
                {
                    std::map<std::string, ConfigValueContainer*>::const_iterator it4 = it2->second->getConfigValueMap().find((*it3)->getName());
                    if (it4 != it2->second->getConfigValueMapEnd())
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

    void ConfigFile::clear()
    {
        for (std::list<ConfigFileSection*>::iterator it = this->sections_.begin(); it != this->sections_.end(); )
            delete (*(it++));
        this->sections_.clear();
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

    void ConfigFile::updateConfigValues()
    {
        if (this->type_ == ConfigFileType::Settings)
        {
            for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getStringIdentifierMapBegin(); it != Identifier::getStringIdentifierMapEnd(); ++it)
            {
                if (it->second->hasConfigValues())
                {
                    for (std::map<std::string, ConfigValueContainer*>::const_iterator it2 = it->second->getConfigValueMapBegin(); it2 != it->second->getConfigValueMapEnd(); ++it2)
                        it2->second->update();

                    it->second->updateConfigValues();
                }
            }
        }
    }


    ///////////////////////
    // ConfigFileManager //
    ///////////////////////

    ConfigFileManager* ConfigFileManager::singletonPtr_s = 0;

    std::string ConfigFileManager::DEFAULT_CONFIG_FILE = "default.ini";

    ConfigFileManager::ConfigFileManager()
         : mininmalFreeType_(ConfigFileType::numberOfReservedTypes)
    {
    }

    ConfigFileManager::~ConfigFileManager()
    {
        for (std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); )
            delete (it++)->second;
    }

    void ConfigFileManager::setFilename(ConfigFileType type, const std::string& filename)
    {
        std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.find(type);
        if (it != this->configFiles_.end())
        {
            assert(it->second);
            delete it->second;
        }
        this->configFiles_[type] = new ConfigFile(filename, type);
        this->load(type);
    }

    void ConfigFileManager::load()
    {
        for (std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            it->second->load();
    }

    void ConfigFileManager::save()
    {
        for (std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            it->second->save();
    }

    void ConfigFileManager::clean(bool bCleanComments)
    {
        for (std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            this->clean(it->first, bCleanComments);
    }

    void ConfigFileManager::load(ConfigFileType type)
    {
        this->getFile(type)->load();
    }

    void ConfigFileManager::save(ConfigFileType type)
    {
        this->getFile(type)->save();
    }

    void ConfigFileManager::saveAs(ConfigFileType type, const std::string& saveFilename)
    {
        this->getFile(type)->saveAs(saveFilename);
    }

    void ConfigFileManager::clean(ConfigFileType type, bool bCleanComments)
    {
        this->getFile(type)->clean(bCleanComments);
    }

    void ConfigFileManager::updateConfigValues()
    {
        for (std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            it->second->updateConfigValues();
    }

    void ConfigFileManager::updateConfigValues(ConfigFileType type)
    {
        this->getFile(type)->updateConfigValues();
    }

    const std::string& ConfigFileManager::getFilename(ConfigFileType type)
    {
        std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.find(type);
        if (it != this->configFiles_.end())
            return it->second->getFilename();
        else
            return BLANKSTRING;
    }

    ConfigFile* ConfigFileManager::getFile(ConfigFileType type)
    {
        std::map<ConfigFileType, ConfigFile*>::const_iterator it = this->configFiles_.find(type);
        if (it != this->configFiles_.end())
            return it->second;
        else
        {
            COUT(1) << "ConfigFileManager: Can't find a config file for type with ID " << static_cast<int>(type) << std::endl;
            COUT(1) << "Using " << DEFAULT_CONFIG_FILE << " file." << std::endl;
            this->setFilename(type, DEFAULT_CONFIG_FILE);
            return getFile(type);
        }
    }
}
