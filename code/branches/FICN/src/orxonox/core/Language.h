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

#include <map>

#include "CorePrereqs.h"

#include "misc/String.h"
#include "OrxonoxClass.h"

namespace orxonox
{
    typedef String LanguageEntryName;

    class _CoreExport LanguageEntry : public OrxonoxClass
    {
        public:
            explicit LanguageEntry(const String& fallbackEntry);
            void setTranslation(const String& translation);
            void setDefault(const String& fallbackEntry);

            inline const String& getTranslation()
                { return this->translatedEntry_; }

            inline const String& getDefault()
                { return this->fallbackEntry_; }

        private:
            String fallbackEntry_;
            String translatedEntry_;
    };

    class _CoreExport Language : public OrxonoxClass
    {
        public:
            static Language& getLanguage();
            void setConfigValues();
            void addEntry(const LanguageEntryName& name, const String& entry);
            const String& getTranslation(const LanguageEntryName& name) const;

        private:
            Language();
            Language(const Language& language) {}   // don't copy
            virtual ~Language() {};                 // don't delete

            void readDefaultLanguageFile();
            void readTranslatedLanguageFile();
            void writeDefaultLanguageFile() const;
            static const String getFileName(const String& language);
            void createEntry(const LanguageEntryName& name, const String& entry);

            String language_;
            String defaultLanguage_;
            String defaultTranslation_;
            std::map<String, LanguageEntry*> languageEntries_;
    };
}

#endif /* _Language_H__ */
