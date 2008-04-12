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

/**
    @file Language.cc
    @brief Implementation of the Language and the LanguageEntry class.
*/

#include <fstream>

#include "CoreIncludes.h"
#include "Language.h"
#include "ConfigValueIncludes.h"

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
        RegisterRootObject(LanguageEntry);

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
        if ((localisation != "") && (localisation.size() > 0))
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
    /**
        @brief Constructor: Reads the default language file and sets some values.
    */
    Language::Language()
    {
        RegisterRootObject(Language);

        this->defaultLanguage_ = "default";
        this->defaultLocalisation_ = "ERROR: LANGUAGE ENTRY DOESN'T EXIST!";

        // Read the default language file to create all known LanguageEntry objects
        this->readDefaultLanguageFile();
    }

    /**
        @brief Function to collect the SetConfigValue-macro calls.
    */
    void Language::setConfigValues()
    {
        SetConfigValue(language_, this->defaultLanguage_).description("The language of the ingame text");

        // Read the translation file after the language was configured
        this->readTranslatedLanguageFile();
    }

    /**
        @brief Returns a reference to the only existing instance of the Language class and calls the setConfigValues() function.
        @return The reference to the only existing instance
    */
    Language& Language::getLanguage()
    {
        // Use static variables to avoid conflicts while executing this code before main()
        static Language theOnlyLanguageObject = Language();
        static bool bCreatingTheOnlyLanguageObject = true;

        // This workaround is used to set a description of the own config value without creating an infinite recursion
        if (bCreatingTheOnlyLanguageObject)
        {
            bCreatingTheOnlyLanguageObject = false;
            theOnlyLanguageObject.setConfigValues();
        }

        return theOnlyLanguageObject;
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

        COUT(2) << "Warning: Language entry " << label << " is duplicate in " << getFileName(this->defaultLanguage_) << "!" << std::endl;
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
    const std::string Language::getFileName(const std::string& language)
    {
        return std::string("translation_" + language + ".lang");
    }

    /**
        @brief Reads the default language file and creates a LanguageEntry objects for every entry.
    */
    void Language::readDefaultLanguageFile()
    {
        COUT(4) << "Read default language file." << std::endl;

        // This creates the file if it's not existing
        std::ofstream createFile;
        createFile.open(getFileName(this->defaultLanguage_).c_str(), std::fstream::app);
        createFile.close();

        // Open the file
        std::ifstream file;
        file.open(getFileName(this->defaultLanguage_).c_str(), std::fstream::in);

        if (!file.is_open())
        {
            COUT(1) << "An error occurred in Language.cc:" << std::endl;
            COUT(1) << "Error: Couldn't open file " << getFileName(this->defaultLanguage_) << " to read the default language entries!" << std::endl;
            return;
        }

        char line[1024];

        // Iterate through the file and create the LanguageEntries
        while (file.good() && !file.eof())
        {
            file.getline(line, 1024);
            std::string lineString = std::string(line);

            // Check if the line is empty
            if ((lineString != "") && (lineString.size() > 0))
            {
                unsigned int pos = lineString.find('=');

                // Check if the length is at least 3 and if there's an entry before and behind the =
                if (pos > 0 && pos < (lineString.size() - 1) && lineString.size() >= 3)
                    this->createEntry(lineString.substr(0, pos), lineString.substr(pos + 1));
                else
                    COUT(2) << "Warning: Invalid language entry \"" << lineString << "\" in " << getFileName(this->defaultLanguage_) << std::endl;
            }
        }

        file.close();
    }

    /**
        @brief Reads the language file of the configured language and assigns the localisation to the corresponding LanguageEntry object.
    */
    void Language::readTranslatedLanguageFile()
    {
        COUT(4) << "Read translated language file (" << this->language_ << ")." << std::endl;

        // Open the file
        std::ifstream file;
        file.open(getFileName(this->language_).c_str(), std::fstream::in);

        if (!file.is_open())
        {
            COUT(1) << "An error occurred in Language.cc:" << std::endl;
            COUT(1) << "Error: Couldn't open file " << getFileName(this->language_) << " to read the translated language entries!" << std::endl;
            ResetConfigValue(language_);
            COUT(3) << "Info: Reset language to " << this->defaultLanguage_ << "." << std::endl;
            return;
        }

        char line[1024];

        // Iterate through the file and create the LanguageEntries
        while (file.good() && !file.eof())
        {
            file.getline(line, 1024);
            std::string lineString = std::string(line);

            // Check if the line is empty
            if ((lineString != "") && (lineString.size() > 0))
            {
                unsigned int pos = lineString.find('=');

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
                    COUT(2) << "Warning: Invalid language entry \"" << lineString << "\" in " << getFileName(this->language_) << std::endl;
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

        // Open the file
        std::ofstream file;
        file.open(getFileName(this->defaultLanguage_).c_str(), std::fstream::out);

        if (!file.is_open())
        {
            COUT(1) << "An error occurred in Language.cc:" << std::endl;
            COUT(1) << "Error: Couldn't open file " << getFileName(this->defaultLanguage_) << " to write the default language entries!" << std::endl;
            return;
        }

        // Iterate through the list an write the lines into the file
        for (Iterator<LanguageEntry> it = ObjectList<LanguageEntry>::start(); it; ++it)
        {
            file << it->getLabel() << "=" << it->getDefault() << std::endl;
        }

        file.close();
    }
}
