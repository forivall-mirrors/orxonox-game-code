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

#ifndef _ConfigFileEntryComment_H__
#define _ConfigFileEntryComment_H__

#include "core/CorePrereqs.h"

#include "ConfigFileEntry.h"

namespace orxonox
{
    ////////////////////////////
    // ConfigFileEntryComment //
    ////////////////////////////
    /**
        @brief This class represents a line in the config file which contains only a comment.
    */
    class _CoreExport ConfigFileEntryComment : public ConfigFileEntry
    {
        public:
            /// Constructor: Initializes the object.
            inline ConfigFileEntryComment(const std::string& comment) : comment_(comment) {}

            /// Destructor
            inline virtual ~ConfigFileEntryComment() {}

            inline virtual const std::string& getName() const
                { return this->comment_; }

            inline virtual void setComment(const std::string& comment)
                { this->comment_ = comment; }

            inline virtual void setValue(const std::string& value)
                {}
            inline virtual const std::string& getValue() const
                { return BLANKSTRING; }

            inline void setString(bool bString)
                {}

            inline virtual const std::string& getFileEntry() const
                { return this->comment_; }

        private:
            std::string comment_;   ///< The comment
    };
}

#endif /* _ConfigFileEntryComment_H__ */
