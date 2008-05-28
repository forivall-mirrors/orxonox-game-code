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

#ifndef _Projectile_H__
#define _Projectile_H__

#include "OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "../tools/BillboardSet.h"
#include "../tools/Timer.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport Projectile : public WorldEntity
    {
        public:
            Projectile(SpaceShip* owner = 0);
            virtual ~Projectile();
            void setConfigValues();
            void destroyObject();
            virtual void tick(float dt);
            virtual bool create(){return WorldEntity::create();}
            void setColour(const ColourValue& colour);

        private:
            SpaceShip* owner_;
            BillboardSet billboard_;
            static float speed_;
            float lifetime_;
            Timer<Projectile> destroyTimer_;
    };
}

#endif /* _Projectile_H__ */
