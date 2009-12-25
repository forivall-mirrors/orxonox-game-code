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
    @brief Implementation of the Language and the LanguageEntry classes.
*/

#include "Language.h"

#include <fstream>
#include "util/Debug.h"
#include "Core.h"
#include "PathConfig.h"

namespace orxonox
{
    // ###############################
    // ###      LanguageEntry      ###
    // ###############################
    /**
        @brief Constructor: Sets the default entry.
        @param fallbackEntry The default entry
    */
    LanguageEntry::LanguageEntry(const std::string& fallbackEntry)
    {
        this->fallbackEntry_ = fallbackEntry;
        this->localisedEntry_ = fallbackEntry; // Set the localisation to the fallback entry, for the case that no translation gets assigned
        this->bLocalisationSet_ = false;
    }

    /**
        @brief Sets the localisation of the entry.
        @param localisation The localisation
    */
    void LanguageEntry::setLocalisation(const std::string& localisation)
    {
        // Check if the translation is more than just an empty string
        if (!localisation.empty())
        {
            this->localisedEntry_ = localisation;
            this->bLocalisationSet_ = true;
        }
        else
            this->localisedEntry_ = this->fallbackEntry_;
    }

    /**
        @brief Sets the default entry.
        @param fallbackEntry The default entry
    */
    void LanguageEntry::setDefault(const std::string& fallbackEntry)
    {
        // If the default entry changes and the translation wasn't set yet, use the new default entry as translation
        if (!this->bLocalisationSet_)
            this->localisedEntry_ = fallbackEntry;

        this->fallbackEntry_ = fallbackEntry;
    }

    // ###############################
    // ###        Language         ###
    // ###############################

    Language* Language::singletonPtr_s = 0;

    /**
        @brief Constructor: Reads the default language file and sets some values.
    */
    Language::Language()
    {
        this->defaultLanguage_ = "default";
        this->defaultLocalisation_ = "ERROR: LANGUAGE ENTRY DOESN'T EXIST!";

        // Read the default language file to create all known LanguageEntry objects
        this->readDefaultLanguageFile();
    }

    /**
        @brief Destructor: Deletes all language entries.
    */
    Language::~Language()
    {
        for (std::map<std::string, LanguageEntry*>::iterator it = this->languageEntries_.begin(); it != this->languageEntries_.end(); ++it)
            delete (it->second);
    }

    /**
        @brief Creates a new LanguageEntry with a given label and a given default entry.
        @param label The label of the entry
        @param entry The default entry
        @return The created LanguageEntry object
    */
    LanguageEntry* Language::createEntry(const LanguageEntryLabel& label, const std::string& entry)
    {
        std::map<std::string, LanguageEntry*>::const_iterator it = this->languageEntries_.find(label);

        // Make sure we don't create a duplicate entry
        if (it == this->languageEntries_.end())
        {
            LanguageEntry* newEntry = new LanguageEntry(entry);
            newEntry->setLabel(label);
            this->languageEntries_[label] = newEntry;
            return newEntry;
        }

        COUT(2) << "Warning: Language entry " << label << " is duplicate in " << getFilename(this->defaultLanguage_) << '!' << std::endl;
        return it->second;
    }

    /**
        @brief Adds a new LanguageEntry, if it's not already existing.
        @param label The label of the entry
        @param entry The default entry
    */
    void Language::addEntry(const LanguageEntryLabel& label, const std::string& entry)
    {
        COUT(5) << "Language: Called addEntry with\n  label: " << label << "\n  entry: " <<  entry << std::endl;
        std::map<std::string, LanguageEntry*>::const_iterator it = this->languageEntries_.find(label);
        if (it == this->languageEntries_.end())
        {
            // The entry isn't available yet, meaning it's new, so create it
            this->createEntry(label, entry);
        }
        else if (it->second->getDefault().compare(entry) == 0)
        {
            // The entry is available and the default string is the same, so return because everything is fine
            return;
        }
        else
        {
            // The defined default entry is not the same as in the default language file - change it to the new entry
            it->second->setDefault(entry);
        }

        // Write the default language file because either a new entry was created or an existing entry has changed
        this->writeDefaultLanguageFile();

    }

    /**
        @brief Returns the localisation of a given entry.
        @param label The label of the entry
        @return The localisation
    */
    const std::string& Language::getLocalisation(const LanguageEntryLabel& label) const
    {
        std::map<std::string, LanguageEntry*>::const_iterator it = this->languageEntries_.find(label);
        if (it != this->languageEntries_.end())
            return it->second->getLocalisation();
        else
        {
            // Uh, oh, an undefined entry was requested: return the default string
            COUT(2) << "Warning: Language entry \"" << label << "\" not found!" << std::endl;
            return this->defaultLocalisation_;
        }
    }

    /**
        @brief Creates the name of the language file out of the languages name.
        @param language The name of the language
        @return The filename
    */
    std::string Language::getFilename(const std::string& language)
    {
        return std::string("translation_" + language + ".lang");
    }

    /**
        @brief Reads the default language file and creates a LanguageEntry objects for every entry.
    */
    void Language::readDefaultLanguageFile()
    {
        COUT(4) << "Read default language file." << std::endl;

        const std::string& filepath = PathConfig::getConfigPathString() + getFilename(this->defaultLanguage_);

        // This creates the file if it's not existing
        std::ofstream createFile;
        createFile.open(filepath.c_str(), std::fstream::app);
        createFile.close();

        // Open the file
        std::ifstream file;
        file.open(filepath.c_str(), std::fstream::in);

        if (!file.is_open())
        {
            COUT(1) << "An error occurred in Language.cc:" << std::endl;
            COUT(1) << "Error: Couldn't open file " << getFilename(this->defaultLanguage_) << " to read the default language entries!" << std::endl;
            return;
        }

        // Iterate through the file and create the LanguageEntries
        while (file.good() && !file.eof())
        {
            std::string lineString;
            std::getline(file, lineString);

            // Check if the line is empty
            if (!lineString.empty())
            {
                size_t pos = lineString.find('=');

                // Check if the length is at least 3 and if there's an entry before and behind the =
                if (pos > 0 && pos < (lineString.size() - 1) && lineString.size() >= 3)
                    this->createEntry(lineString.substr(0, pos), lineString.substr(pos + 1));
                else
                {
                    COUT(2) << "Warning: Invalid language entry \"" << lineString << "\" in " << getFilename(this->defaultLanguage_) << std::endl;
                }
            }
        }

        file.close();
    }

    /**
        @brief Reads the language file of the configured language and assigns the localisation to the corresponding LanguageEntry object.
    */
    void Language::readTranslatedLanguageFile()
    {
        COUT(4) << "Read translated language file (" << Core::getInstance().getLanguage() << ")." << std::endl;

        const std::string& filepath = PathConfig::getConfigPathString() + getFilename(Core::getInstance().getLanguage());

        // Open the file
        std::ifstream file;
        file.open(filepath.c_str(), std::fstream::in);

        if (!file.is_open())
        {
            COUT(1) << "An error occurred in Language.cc:" << std::endl;
            COUT(1) << "Error: Couldn't open file " << getFilename(Core::getInstance().getLanguage()) << " to read the translated language entries!" << std::endl;
            Core::getInstance().resetLanguage();
            COUT(3) << "Info: Reset language to " << this->defaultLanguage_ << '.' << std::endl;
            return;
        }

        // Iterate through the file and create the LanguageEntries
        while (file.good() && !file.eof())
        {
            std::string lineString;
            std::getline(file, lineString);

            // Check if the line is empty
            if (!lineString.empty())
            {
                size_t pos = lineString.find('=');

                // Check if the length is at least 3 and if there's an entry before and behind the =
                if (pos > 0 && pos < (lineString.size() - 1) && lineString.size() >= 3)
                {
                    std::map<std::string, LanguageEntry*>::const_iterator it = this->languageEntries_.find(lineString.substr(0, pos));

                    // Check if the entry exists
                    if (it != this->languageEntries_.end())
                        it->second->setLocalisation(lineString.substr(pos + 1));
                    else
                        this->createEntry(lineString.substr(0, pos), this->defaultLocalisation_)->setLocalisation(lineString.substr(pos + 1));
                }
                else
                {
                    COUT(2) << "Warning: Invalid language entry \"" << lineString << "\" in " << getFilename(Core::getInstance().getLanguage()) << std::endl;
                }
            }
        }

        file.close();
    }

    /**
        @brief Writes all default entries to the default language file.
    */
    void Language::writeDefaultLanguageFile() const
    {
        COUT(4) << "Language: Write default language file." << std::endl;

        const std::string& filepath = PathConfig::getConfigPathString() + getFilename(this->defaultLanguage_);

        // Open the file
        std::ofstream file;
        file.open(filepath.c_str(), std::fstream::out);

        if (!file.is_open())
        {
            COUT(1) << "An error occurred in Language.cc:" << std::endl;
            COUT(1) << "Error: Couldn't open file " << getFilename(this->defaultLanguage_) << " to write the default language entries!" << std::endl;
            return;
        }

        // Iterate through the list an write the lines into the file
        for (std::map<std::string, LanguageEntry*>::const_iterator it = this->languageEntries_.begin(); it != this->languageEntries_.end(); ++it)
        {
            file << it->second->getLabel() << '=' << it->second->getDefault() << std::endl;
        }

        file.close();
    }
}
