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

#ifndef _Language_H__
#define _Language_H__

#include <string>
#include <map>

#include "CorePrereqs.h"

#include "OrxonoxClass.h"

namespace orxonox
{
    typedef std::string LanguageEntryName;

    class _CoreExport LanguageEntry : public OrxonoxClass
    {
        public:
            explicit LanguageEntry(const std::string& fallbackEntry);
            void setTranslation(const std::string& translation);
            void setDefault(const std::string& fallbackEntry);

            inline const std::string& getTranslation()
                { return this->translatedEntry_; }

            inline const std::string& getDefault()
                { return this->fallbackEntry_; }

        private:
            std::string fallbackEntry_;
            std::string translatedEntry_;
    };

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

            std::string language_;
            std::string defaultLanguage_;
            std::string defaultTranslation_;
            std::map<std::string, LanguageEntry*> languageEntries_;
    };
}

#endif /* _Language_H__ */
