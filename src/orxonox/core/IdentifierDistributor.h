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

/**
    @file IdentifierDistributor.h
    @brief Definition of the IdentifierDistributor class

    The IdentifierDistributor makes sure that only one instance of ClassIdentifier for each
    template parameter T exists. All Identifiers are stored in a map with their name.
    IdentifierDistributor is a singleton class, it can't be created or deleted directly.
*/

#ifndef _IdentifierDistributor_H__
#define _IdentifierDistributor_H__

#include <map>

#include "CorePrereqs.h"

namespace orxonox
{
    //! The Identifier Distributor stores all Identifiers and makes sure there are no ambiguities.
    class _CoreExport IdentifierDistributor
    {
        public:
            static Identifier* getIdentifier(const std::string& name, Identifier* proposal);

        private:
            IdentifierDistributor() {};                                         // Don't create
            IdentifierDistributor(const IdentifierDistributor& distributor) {}  // Don't copy
            ~IdentifierDistributor() {}                                         // Don't delete

            std::map<std::string, Identifier*> identifiers_;    //!< The map to store all Identifiers.
    };
}

#endif /* _IdentifierDistributor_H__ */
