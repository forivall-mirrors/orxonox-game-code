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
    @ingroup Config ConfigFile
*/

#ifndef _ConfigFile_H__
#define _ConfigFile_H__

#include "core/CorePrereqs.h"

#include "ConfigFileSection.h"

namespace orxonox
{
    ////////////////
    // ConfigFile //
    ////////////////
    /**
        @brief This class represents a config file, which is stored on the hard-disk and contains config values in different sections.

        It provides an interface to manipulate the sections and values.
    */
    class _CoreExport ConfigFile
    {
        public:
            ConfigFile(const std::string& filename, bool bCopyFallbackFile = true);
            virtual ~ConfigFile();

            virtual void load();
            virtual void save() const;
            virtual void saveAs(const std::string& filename) const;
            virtual void clear();

            /// Returns the file-name of this config file
            inline const std::string& getFilename()
                { return this->filename_; }

            /**
                @brief Stores a value in the config file. If the entry or its section doesn't exist, it's created.

                @param section  The name of the section
                @param name     The name of the entry
                @param value    The new value
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline void setValue(const std::string& section, const std::string& name, const std::string& value, bool bString)
            {
                this->getOrCreateSection(section)->setValue(name, value, bString);
                this->save();
            }
            /**
                @brief Returns the value of a given entry in the config file. Returns a blank string if the value doesn't exist.

                @param section  The name of the section
                @param name     The name of the entry
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline const std::string& getValue(const std::string& section, const std::string& name, bool bString)
            {
                ConfigFileSection* sectionPtr = this->getSection(section);
                return (sectionPtr ? sectionPtr->getValue(name, bString) : BLANKSTRING);
            }
            /**
                @brief Returns the value of a given entry in the config file. If it doesn't exist, the entry is created using the fallback value.

                @param section  The name of the section
                @param name     The name of the entry
                @param fallback The value that will be used if the entry doesn't exist
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline const std::string& getOrCreateValue(const std::string& section, const std::string& name, const std::string& fallback, bool bString)
            {
                const std::string& output = this->getOrCreateSection(section)->getOrCreateValue(name, fallback, bString);
                this->saveIfUpdated();
                return output;
            }

            /**
                @brief Stores the value of an element of a vector in the config file. If the entry or its section doesn't exist, it's created.

                @param section  The name of the section
                @param name     The name of the vector
                @param index    The index of the element in the vector
                @param value    The new value
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline void setValue(const std::string& section, const std::string& name, unsigned int index, const std::string& value, bool bString)
            {
                this->getOrCreateSection(section)->setValue(name, index, value, bString);
                this->save();
            }
            /**
                @brief Returns the value of a given element of a vector in the config file. Returns a blank string if the value doesn't exist.

                @param section  The name of the section
                @param name     The name of the vector
                @param index    The index of the element in the vector
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            inline const std::string& getValue(const std::string& section, const std::string& name, unsigned int index, bool bString)
            {
                ConfigFileSection* sectionPtr = this->getSection(section);
                return (sectionPtr ? sectionPtr->getValue(name, index, bString) : BLANKSTRING);
            }
            /**
                @brief Returns the value of a given element of a vector in the config file. If it doesn't exist, the entry is created using the fallback value.

                @param section  The name of the section
                @param name     The name of the vector
                @param index    The index of the element in the vector
                @param fallback The value that will be used if the entry doesn't exist
                @param bString  If true, the value is treated as string which means some special treatment of special characters.
            */
            const std::string& getOrCreateValue(const std::string& section, const std::string& name, unsigned int index, const std::string& fallback, bool bString)
            {
                const std::string& output = this->getOrCreateSection(section)->getOrCreateValue(name, index, fallback, bString);
                this->saveIfUpdated();
                return output;
            }

            void deleteVectorEntries(const std::string& section, const std::string& name, unsigned int startindex = 0);
            /**
                @brief Returns the size of a config vector.
                @param section  The section of the vector
                @param name     The name of the vector
            */
            inline unsigned int getVectorSize(const std::string& section, const std::string& name) const
            {
                ConfigFileSection* sectionPtr = this->getSection(section);
                return (sectionPtr ? sectionPtr->getVectorSize(name) : 0);
            }

            static const char* DEFAULT_CONFIG_FOLDER;   ///< The folder where the default config files will be stored

        protected:
            ConfigFileSection* getSection(const std::string& section) const;
            ConfigFileSection* getOrCreateSection(const std::string& section);

            std::list<ConfigFileSection*> sections_;    ///< A list of sections in this config file

        private:
            void saveIfUpdated();

            const std::string filename_;                ///< The filename of this config file
            const bool bCopyFallbackFile_;              ///< If true, the default config file is copied into the config-directory before loading the file
            bool bUpdated_;                             ///< Becomes true if a section is added
    };
}

#endif /* _ConfigFile_H__ */
