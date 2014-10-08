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

#ifndef _ConfigFileEntryVectorValue_H__
#define _ConfigFileEntryVectorValue_H__

#include "core/CorePrereqs.h"

#include "ConfigFileEntryValue.h"

namespace orxonox
{
    ////////////////////////////////
    // ConfigFileEntryVectorValue //
    ////////////////////////////////
    /**
        @brief Subclass of ConfigFileEntryValue, represents an element of a vector.
    */
    class _CoreExport ConfigFileEntryVectorValue : public ConfigFileEntryValue
    {
        public:
            /**
                @brief Constructor: Initializes the entry.

                @param name                 The name of the vector
                @param index                The index of the element in the vector
                @param value                The value of the element
                @param bString              If true, the value is treated as string which means some special treatment of special characters.
                @param additionalComment    An optional comment that will be placed behind the value in the config file
            */
            inline ConfigFileEntryVectorValue(const std::string& name, unsigned int index, const std::string& value = "", bool bString = false, const std::string& additionalComment = "")
                : ConfigFileEntryValue(name, value, bString, additionalComment)
                , index_(index)
                { this->update(); /*No virtual calls in base class ctor*/ }

            /// Destructor
            inline ~ConfigFileEntryVectorValue() {}

            inline unsigned int getIndex() const
                { return this->index_; }

            /// Returns the "key" of the value (the name of the vector plus the index of the element)
            inline const std::string& getKeyString() const
                { return this->keyString_; }

        private:
            void update();

            unsigned int index_;        ///< The index of the element in the vector
            std::string keyString_;     ///< The full name of the entry (the name of the vector plus the index of the element)
    };
}

#endif /* _ConfigFileEntryVectorValue_H__ */
