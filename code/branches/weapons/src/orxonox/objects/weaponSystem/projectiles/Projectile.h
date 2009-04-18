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

#include "objects/worldentities/MovableEntity.h"
#include "objects/worldentities/pawns/Pawn.h"
#include "tools/Timer.h"

namespace orxonox
{
    class _OrxonoxExport Projectile : public MovableEntity, public PawnListener
    {
        public:
            Projectile(BaseObject* creator);
            virtual ~Projectile();

            void setConfigValues();
            void destroyObject();

            virtual void tick(float dt);
            virtual bool collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint);
            virtual void destroyedPawn(Pawn* pawn);

            inline void setDamage(float damage)
                { this->damage_ = damage; }
            inline float getDamage() const
                { return this->damage_; }

            inline void setOwner(Pawn* owner)
                { this->owner_ = owner; }
            inline Pawn* getOwner() const
                { return this->owner_; }

        private:
            Pawn* owner_;
            float lifetime_;
            float damage_;
            bool bDestroy_;
            Timer<Projectile> destroyTimer_;
    };
}

#endif /* _Projectile_H__ */
