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
    @brief Definition of the Language and the LanguageEntry class.

    The Language class is used, to get a localisation of a string in the configured language.
    The string is identified by another string, the label of the entry.
    If the translation in the configured language isn't available, the default entry, defined in the code, is used.

    Usage:
     - Set the entry with the default string:
       Language::getInstance()->addEntry("label of the entry", "the string to translate");

     - Get the localisation of the entry in the configured language:
       std::cout << Language::getInstance()->getLocalisation("name of the entry") << std::endl;
*/

#ifndef _Language_H__
#define _Language_H__

#include "CorePrereqs.h"

#include <map>
#include <string>
#include <cassert>
#include "util/Singleton.h"

#define AddLanguageEntry(label, fallbackstring) \
    orxonox::Language::getInstance().addEntry(label, fallbackstring)

#define GetLocalisation(label) \
    orxonox::Language::getInstance().getLocalisation(label)


namespace orxonox
{
    // ###############################
    // ###      LanguageEntry      ###
    // ###############################
    //! The LanguageEntry class stores the default- and the translated string of a given entry in the language file.
    class _CoreExport LanguageEntry
    {
        public:
            explicit LanguageEntry(const std::string& fallbackEntry);
            void setLocalisation(const std::string& localisation);
            void setDefault(const std::string& fallbackEntry);

            /**
              @brief Returns the localised entry in the configured language.
              @return The translated entry
            */
            inline const std::string& getLocalisation()
                { return this->localisedEntry_; }

            /**
              @brief Returns the default entry.
              @return The default entry
            */
            inline const std::string& getDefault()
                { return this->fallbackEntry_; }

            /**
                @brief Sets the label of this entry.
                @param label The label
            */
            inline void setLabel(const LanguageEntryLabel& label)
                { this->label_ = label; }

            /**
                @brief Returns the label of this entry.
                @return The label
            */
            inline const LanguageEntryLabel& getLabel() const
                { return this->label_; }

        private:
            LanguageEntryLabel label_;              //!< The label of the entry
            std::string fallbackEntry_;             //!< The default entry: Used, if no translation is available or no language configured
            std::string localisedEntry_;            //!< The localised entry in the configured language
            bool bLocalisationSet_;                 //!< True if the translation was set
    };


    // ###############################
    // ###         Language        ###
    // ###############################
    //! The Language class manges the language files and entries and stores the LanguageEntry objects in a map.
    class _CoreExport Language : public Singleton<Language>
    {
        friend class Singleton<Language>;
        friend class Core;

        public:
            Language();
            ~Language();

            void addEntry(const LanguageEntryLabel& label, const std::string& entry);
            const std::string& getLocalisation(const LanguageEntryLabel& label) const;

        private:
            Language(const Language&);

            void readDefaultLanguageFile();
            void readTranslatedLanguageFile();
            void writeDefaultLanguageFile() const;
            static std::string getFilename(const std::string& language);
            LanguageEntry* createEntry(const LanguageEntryLabel& label, const std::string& entry);

            std::string defaultLanguage_;                           //!< The default language
            std::string defaultLocalisation_;                       //!< The returned string, if an entry unavailable entry is requested
            std::map<std::string, LanguageEntry*> languageEntries_; //!< A map to store all LanguageEntry objects and their labels

            static Language* singletonPtr_s;
    };
}

#endif /* _Language_H__ */
