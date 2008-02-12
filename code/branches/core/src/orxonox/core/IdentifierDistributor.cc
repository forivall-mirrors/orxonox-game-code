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

#include "IdentifierDistributor.h"
#include "Identifier.h"

namespace orxonox
{
    Identifier* IdentifierDistributor::getIdentifier(const std::string& name, Identifier* proposal)
    {
        static IdentifierDistributor theOneAndOnlyInstance = IdentifierDistributor();

        std::map<std::string, Identifier*>::const_iterator it = theOneAndOnlyInstance.identifiers_.find(name);
        if (it != theOneAndOnlyInstance.identifiers_.end())
        {
            return (*it).second;
        }
        else
        {
            theOneAndOnlyInstance.identifiers_[name] = proposal;
            return proposal;
        }
    }
}
