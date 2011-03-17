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
 *      Maurus Kaufmann
 *   Co-authors:
 *      ...
 *
 */

#include "SpaceBoundaries.h"

/* Folgender Block ist Copy-Paste und somit teilweise wohl unnoetig */
#include "core/CoreIncludes.h"
#include "core/Template.h"
#include "core/XMLPort.h"
#include "gametypes/Gametype.h"
#include "worldentities/pawns/Pawn.h"

/* Eigene, spezifische include-Statements*/

namespace orxonox
{
    CreateFactory(SpaceBoundaries);

    SpaceBoundaries::SpaceBoundaries(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(SpaceBoundaries);
    }
    
    SpaceBoundaries::~SpaceBoundaries()
    {
    
    }

    void SpaceBoundaries::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
    //    SUPER(SpaceBoundaries, XMLPort, xmlelement, mode);

    //    XMLPortParam(SpaceBoundaries, "spawnclass", setSpawnClassName, getSpawnClassName, xmlelement, mode);
    //    XMLPortParam(SpaceBoundaries, "pawndesign", setTemplateName, getTemplateName, xmlelement, mode);
    }
}
