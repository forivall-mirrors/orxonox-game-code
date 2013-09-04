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

#ifndef _ConfigFileEntryValue_H__
#define _ConfigFileEntryValue_H__

#include "core/CorePrereqs.h"

#include "ConfigFileEntry.h"

namespace orxonox
{
    //////////////////////////
    // ConfigFileEntryValue //
    //////////////////////////
    /**
        @brief This class represents a normal value in the config file.
    */
    class _CoreExport ConfigFileEntryValue : public ConfigFileEntry
    {
        public:
            /**
                @brief Constructor: Initializes the entry.

                @param name                 The name of the entry
                @param value                The value of the entry
                @param bString              If true, the value is treated as string which means some special treatment of special characters.
                @param additionalComment    An optional comment that will be placed behind the value in the config file
            */
            inline ConfigFileEntryValue(const std::string& name, const std::string& value = "", bool bString = false, const std::string& additionalComment = "")
                : name_(name)
                , value_(value)
                , additionalComment_(additionalComment)
                , bString_(bString)
                { this->update(); }

            /// Destructor
            inline virtual ~ConfigFileEntryValue() {}

            inline virtual const std::string& getName() const
                { return this->name_; }

            inline virtual void setComment(const std::string& comment)
                { this->additionalComment_ = comment; this->update(); }

            inline virtual void setValue(const std::string& value)
                { this->value_ = value; this->update(); }
            inline virtual const std::string& getValue() const
                { return this->value_; }

            inline void virtual setString(bool bString)
                { this->bString_ = bString; this->update(); }

            inline virtual const std::string& getFileEntry() const
                { return this->fileEntry_; }

            /// Returns the "key" of the value (in this case it's just the name of the entry, but for vectors it's different)
            inline virtual const std::string& getKeyString() const
                { return this->name_; }

        protected:
            virtual void update();

            const std::string name_;            ///< The name of the value
            std::string value_;                 ///< The value
            std::string additionalComment_;     ///< The additional comment
            std::string fileEntry_;             ///< The string as it will be stored in the config file
            bool bString_;                      ///< If true, the value is treated as string which means some special treatment of special characters.
    };
}

#endif /* _ConfigFileEntryValue_H__ */
