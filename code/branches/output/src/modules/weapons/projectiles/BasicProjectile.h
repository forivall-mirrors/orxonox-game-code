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
 *      simonmie
 *   Co-authors:
 *      ...
 *
 */

#ifndef _BasicProjectile_H__
#define _BasicProjectile_H__

#include "weapons/WeaponsPrereqs.h"

#include "tools/Timer.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{
    class _WeaponsExport BasicProjectile : public virtual OrxonoxClass
    {
        public:
            BasicProjectile();

            virtual ~BasicProjectile();

            static bool basicCollidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint, Pawn* owner, BasicProjectile* this_);

            void basicDestroyObject();

            inline void setDamage(float damage)
                { this->damage_ = damage;  }
            inline float getDamage() const
                { return this->damage_; }

            inline void setHealthDamage(float healthdamage)
                { this->healthdamage_ = healthdamage; }
            inline float getHealthDamage() const
                { return this->healthdamage_; }

            inline void setShieldDamage(float shielddamage)
                { this->shielddamage_ = shielddamage;  } //ShieldDamage wird korrekt gesettet vom XML-File
            inline float getShieldDamage() const
                { return this->shielddamage_; }


            inline void setBDestroy(bool bDestroy)
                { this->bDestroy_ = bDestroy;  }
            inline float getBDestroy() const
                { return this->bDestroy_; }


        private:
//            WeakPtr<Pawn> owner_; //owner cannot be set in BasicProjectile, because it's already defined in MobileEntity and Movable Entity

            float damage_;
            float healthdamage_;
            float shielddamage_;

            bool bDestroy_;
    };
}

#endif /* _BasicProjectile_H__ */
