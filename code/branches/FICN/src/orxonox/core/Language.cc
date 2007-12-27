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

#include <fstream>

#include "Language.h"
#include "CoreIncludes.h"

namespace orxonox
{
    // ###############################
    // ###      LanguageEntry      ###
    // ###############################
    LanguageEntry::LanguageEntry(const std::string& fallbackEntry)
    {
        RegisterRootObject(LanguageEntry);

        this->fallbackEntry_ = fallbackEntry;
        this->translatedEntry_ = fallbackEntry;
    }

    void LanguageEntry::setTranslation(const std::string& translation)
    {
        if (translation.compare("") != 0)
            this->translatedEntry_ = translation;
        else
            this->translatedEntry_ = this->fallbackEntry_;
    }

    void LanguageEntry::setDefault(const std::string& fallbackEntry)
    {
        if (this->translatedEntry_.compare(this->fallbackEntry_) == 0)
            this->translatedEntry_ = fallbackEntry;

        this->fallbackEntry_ = fallbackEntry;
    }

    // ###############################
    // ###        Language         ###
    // ###############################
    Language::Language()
    {
        RegisterRootObject(Language);
        this->defaultLanguage_ = "default";
        this->defaultTranslation_ = "ERROR: LANGUAGE ENTRY DOESN'T EXIST!";
        this->readDefaultLanguageFile();

        this->setConfigValues();
    }

    void Language::setConfigValues()
    {
        SetConfigValue(language_, this->defaultLanguage_);
        this->readTranslatedLanguageFile();
    }

    Language& Language::getLanguage()
    {
        static Language theOnlyLanguageObject = Language();
        return theOnlyLanguageObject;
    }

    void Language::createEntry(const LanguageEntryName& name, const std::string& entry)
    {
        if (!this->languageEntries_[name])
        {
            LanguageEntry* newEntry = new LanguageEntry(entry);
            newEntry->setName(name);
            this->languageEntries_[name] = newEntry;
        }
        else
        {
            COUT(2) << "Warning: Language entry " << name << " is duplicate in " << getFileName(this->defaultLanguage_) << "!" << std::endl;
        }
    }

    void Language::addEntry(const LanguageEntryName& name, const std::string& entry)
    {
        std::map<std::string, LanguageEntry*>::const_iterator it = this->languageEntries_.find(name);
        if (!it->second)
            this->createEntry(name, entry);
        else if (it->second->getDefault().compare(entry) == 0)
            return;
        else
            it->second->setDefault(entry);

        this->writeDefaultLanguageFile();
    }

    const std::string& Language::getTranslation(const LanguageEntryName& name) const
    {
        std::map<std::string, LanguageEntry*>::const_iterator it = this->languageEntries_.find(name);
        if (it->second)
            return it->second->getTranslation();
        else
        {
            COUT(2) << "Error: Language entry \"" << name << "\" not found!" << std::endl;
            return this->defaultTranslation_;
        }
    }

    const std::string Language::getFileName(const std::string& language)
    {
        return std::string("translation_" + language + ".lang");
    }

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
            COUT(1) << "Error: Couldn't open file " << getFileName(this->defaultLanguage_) << " to read the default language entries!" << std::endl;
            return;
        }

        char line[1024];

        // Iterate through the file and create the LanguageEntries
        while (file.good() && !file.eof())
        {
            file.getline(line, 1024);
            std::string lineString = std::string(line);
            if (lineString.compare("") != 0)
            {
                unsigned int pos = lineString.find('=');
                if (pos < lineString.size() && lineString.size() >= 3)
                    this->createEntry(lineString.substr(0, pos), lineString.substr(pos + 1));
                else
                    COUT(2) << "Warning: Invalid language entry \"" << lineString << "\" in " << getFileName(this->defaultLanguage_) << std::endl;
            }
        }

        file.close();
    }

    void Language::readTranslatedLanguageFile()
    {
        COUT(4) << "Read translated language file (" << this->language_ << ")." << std::endl;

        // Open the file
        std::ifstream file;
        file.open(getFileName(this->language_).c_str(), std::fstream::in);

        if (!file.is_open())
        {
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
            if (lineString.compare("") != 0)
            {
                unsigned int pos = lineString.find('=');
                if (pos < lineString.size() && lineString.size() >= 3)
                {
                    std::map<std::string, LanguageEntry*>::const_iterator it = this->languageEntries_.find(lineString.substr(0, pos));
                    if (it->second)
                        it->second->setTranslation(lineString.substr(pos + 1));
                    else
                        COUT(2) << "Warning: Unknown language entry \"" << lineString.substr(0, pos) << "\" in " << getFileName(this->language_) << std::endl;
                }
                else
                    COUT(2) << "Warning: Invalid language entry \"" << lineString << "\" in " << getFileName(this->language_) << std::endl;
            }
        }

        file.close();
    }

    void Language::writeDefaultLanguageFile() const
    {
        COUT(4) << "Write default language file." << std::endl;

        // Open the file
        std::ofstream file;
        file.open(getFileName(this->defaultLanguage_).c_str(), std::fstream::out);

        if (!file.is_open())
        {
            COUT(1) << "Error: Couldn't open file " << getFileName(this->defaultLanguage_) << " to write the default language entries!" << std::endl;
            return;
        }

        // Iterate through the list an write the lines into the file
        for (Iterator<LanguageEntry> it = ObjectList<LanguageEntry>::start(); it; ++it)
        {
            file << it->getName() << "=" << it->getDefault() << std::endl;
        }

        file.close();
    }
}
