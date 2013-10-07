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

#ifndef _ConfigFileEntry_H__
#define _ConfigFileEntry_H__

#include "core/CorePrereqs.h"

namespace orxonox
{
    /////////////////////
    // ConfigFileEntry //
    /////////////////////
    /**
        @brief This class represents an entry in the config file.

        This class is pure virtual. Use one of the derived classes to define the type of the entry.
    */
    class _CoreExport ConfigFileEntry
    {
        public:
            /// Destructor
            virtual ~ConfigFileEntry() {};

            /// Changes the value of the entry.
            virtual void setValue(const std::string& value) = 0;
            /// Returns the value of the entry.
            virtual const std::string& getValue() const = 0;

            /// Returns the name of the entry
            virtual const std::string& getName() const = 0;

            /// Changes the comment of the entry (will be placed after the value)
            virtual void setComment(const std::string& comment) = 0;

            /// Returns the index of the entry in a vector (used only if it is a vector)
            virtual unsigned int getIndex() const { return 0; }

            /// Defines if this entry is treated as string which means some special treatment of special characters.
            virtual void setString(bool bString) = 0;

            /// Returns the line as it will be stored in the config file.
            virtual const std::string& getFileEntry() const = 0;
    };
}

#endif /* _ConfigFileEntry_H__ */
