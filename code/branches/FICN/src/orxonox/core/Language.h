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

/*!
    @file Language.h
    @brief Definition of the Language and the LanguageEntry class.

    The Language class is used, to get a translation of a string in the configured language.
    The string is identified by another string, the name of the entry.
    If the translation in the configured language isn't available, the default entry, defined in the code, is used.

    Usage:
     - Set the entry with the default string:
       Language::getLanguage()->addEntry("name of the entry", "the string to translate");

     - Get the translation of the entry in the configured language:
       std::cout << Language::getLanguage()->getTranslation("name of the entry") << std::endl;
*/

#ifndef _Language_H__
#define _Language_H__

#include <map>
#include <string>

#include "CorePrereqs.h"
#include "OrxonoxClass.h"

namespace orxonox
{
    typedef std::string LanguageEntryName;

    //! The LanguageEntry class stores the default- and the translated string of a given entry in the language file.
    class _CoreExport LanguageEntry : public OrxonoxClass
    {
        public:
            explicit LanguageEntry(const std::string& fallbackEntry);
            void setTranslation(const std::string& translation);
            void setDefault(const std::string& fallbackEntry);

            /** @brief Returns the translated entry in the configured language. @return The translated entry */
            inline const std::string& getTranslation()
                { return this->translatedEntry_; }

            /** @brief Returns the default entry. @return The default entry */
            inline const std::string& getDefault()
                { return this->fallbackEntry_; }

        private:
            std::string fallbackEntry_;                             //!< The default entry: Used, if no translation is available or no language configured
            std::string translatedEntry_;                           //!< The translated entry in the configured language
    };

    //! The Language class manges the language files and entries and stores the LanguageEntry objects in a map.
    class _CoreExport Language : public OrxonoxClass
    {
        public:
            static Language& getLanguage();
            void setConfigValues();
            void addEntry(const LanguageEntryName& name, const std::string& entry);
            const std::string& getTranslation(const LanguageEntryName& name) const;

        private:
            Language();
            Language(const Language& language) {}   // don't copy
            virtual ~Language() {};                 // don't delete

            void readDefaultLanguageFile();
            void readTranslatedLanguageFile();
            void writeDefaultLanguageFile() const;
            static const std::string getFileName(const std::string& language);
            void createEntry(const LanguageEntryName& name, const std::string& entry);

            std::string language_;                                  //!< The configured language
            std::string defaultLanguage_;                           //!< The default language
            std::string defaultTranslation_;                        //!< The returned string, if an entry unavailable entry is requested
            std::map<std::string, LanguageEntry*> languageEntries_; //!< A map to store all LanguageEntry objects and their name
    };
}

#endif /* _Language_H__ */
