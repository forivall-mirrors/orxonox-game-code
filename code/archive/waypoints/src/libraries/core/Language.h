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
    @defgroup Language Language
    @ingroup Core
*/

/**
    @file
    @ingroup Language
    @brief Declaration of the Language and the LanguageEntry class, as well as some helper functions.

    @anchor LanguageExample

    The Language class is used, to get a localisation of a string in the configured language.
    The string is identified by another string, the label of the entry.
    If the translation in the configured language isn't available, the default entry, defined in the code, is used.

    Usage:
     - Set the entry with the default string:
       @code
       Language::getInstance()->addEntry("label of the entry", "the string to translate");
       @endcode

     - Get the localisation of the entry in the configured language:
       @code
       orxout() << Language::getInstance()->getLocalisation("name of the entry") << endl;
       @endcode

    Example:
    @code
    int age = 20;
    AddLanguageEntry("user_age", "Age");
    orxout() << GetLocalisation("user_age") << ": " << age << endl;
    @endcode

    Resulting output:
    @code
    Age: 20
    @endcode

    The language entry is now defined in @a translation_default.lang:
    @code
    user_age=Age
    @endcode

    We can add a translation for another language, for example @a translation_german.lang:
    @code
    user_age=Alter
    @endcode

    Now change the language in @a orxonox.ini to "german":
    @code
    language_ = "german"
    @endcode

    Now you will see the translated language entry in the resulting output of the above code:
    @code
    Alter: 20
    @endcode
*/

#ifndef _Language_H__
#define _Language_H__

#include "CorePrereqs.h"

#include <map>
#include <string>
#include <cassert>
#include "util/Singleton.h"

namespace orxonox
{
    // ###############################
    // ###      LanguageEntry      ###
    // ###############################
    /**
        @brief The LanguageEntry class stores the default- and the translated string of a given entry in the language file.

        This class belongs to the Language class.
    */
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
    /**
        @brief The Language class manges the language files and entries and stores the LanguageEntry objects in a map.

        @see See @ref LanguageExample "Language.h" for some examples.
    */
    class _CoreExport Language : public Singleton<Language>
    {
        friend class Singleton<Language>;
        friend class Core;

        public:
            Language();
            ~Language();

            void addEntry(const LanguageEntryLabel& label, const std::string& entry);
            const std::string& getLocalisation(const LanguageEntryLabel& label, bool bError = true) const;

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

    /// Shortcut function for Language::addEntry
    inline void AddLanguageEntry(const LanguageEntryLabel& label, const std::string& fallbackString)
    {
        Language::getInstance().addEntry(label, fallbackString);
    }

    /// Shortcut function for Language::getLocalisation
    inline const std::string& GetLocalisation(const LanguageEntryLabel& label)
    {
        return Language::getInstance().getLocalisation(label);
    }

    /// Shortcut function for Language::getLocalisation without printing an error in case the label doesn't exist
    inline const std::string& GetLocalisation_noerror(const LanguageEntryLabel& label)
    {
        return Language::getInstance().getLocalisation(label, false);
    }
}

#endif /* _Language_H__ */
