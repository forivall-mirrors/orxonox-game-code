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

#ifndef _SpaceBoundaries_H__
#define _SpaceBoundaries_H__

/* Folgender Block ist Copy-Paste und somit teilweise wohl unnoetig */
#include "OrxonoxPrereqs.h"
#include <string>
#include "core/SubclassIdentifier.h"

/* Einige, spezifische include-Statements */
#include "core/CoreIncludes.h"
#include "tools/interfaces/Tickable.h"
#include "worldentities/StaticEntity.h"
#include "worldentities/WorldEntity.h"


namespace orxonox
{
    class _OrxonoxExport SpaceBoundaries : public StaticEntity, public Tickable
    {
        public:
            SpaceBoundaries(BaseObject* creator);
            ~SpaceBoundaries();
            
            void setCenter(Vector3 r);
            Vector3 getCenter();
            
            void setMaxDistance(float r);
            float getMaxDistance();
            
            void setWarnDistance(float r);
            float getWarnDistance();

            void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            
            void tick(float dt);

        private:
            Vector3 center;
            float maxDistance;
            float warnDistance;
        
            float computeDistance(WorldEntity *item); //!< Auf den Mittelpunkt 'center' bezogen.
    };
}

#endif /* _SpaceBoundaries_H__ */
