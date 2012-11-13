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
 *      Reto Grieder (physics)
 *   Co-authors:
 *      ...
 *
 */

/**
  @file LensFlare.h
  @brief Definition of the LensFlare class.
*/

#ifndef _LensFlare_H__
#define _LensFlare_H__

#include "OrxonoxPrereqs.h"

#include "OgreBillboardSet.h"

#include "util/Math.h"
#include "worldentities/StaticEntity.h"
#include "graphics/Billboard.h"

namespace orxonox
{
      /**
    @brief
        This class adds a configurable LensFlare effect by adding multiple billboards for the several components of lens flare.
        It uses orxonox::Billboard to render this effect, the used images can be changed in lensflare.material

    @author
        David 'davidsa' Salvisberg
    */
    class _OrxonoxExport LensFlare : public StaticEntity, public Tickable
    {
        public:
            LensFlare(BaseObject* creator);
            virtual ~LensFlare();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void tick(float dt);

            virtual void changedVisibility();

        private:
            void registerVariables();
            
            void createBillboards();
            
            void updateBillboardPositions();
    };
}

#endif /* _LensFlare_H__ */