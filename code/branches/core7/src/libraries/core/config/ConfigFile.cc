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
    @brief Implementation of ConfigFile.
*/

#include "ConfigFile.h"

#include <boost/filesystem.hpp>

#include "util/Convert.h"
#include "util/StringUtils.h"
#include "core/ConfigurablePaths.h"
#include "ConfigFileEntryComment.h"
#include "ConfigFileEntryValue.h"

namespace orxonox
{
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
            filepath = ConfigurablePaths::getConfigPath() / filepath;
            if (this->bCopyFallbackFile_)
            {
                // Look for default file in the data folder
                if (!boost::filesystem::exists(filepath))
                {
                    boost::filesystem::path defaultFilepath(ConfigurablePaths::getDataPath() / DEFAULT_CONFIG_FOLDER / this->filename_);
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
            filepath = ConfigurablePaths::getConfigPath() / filename;
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
}
