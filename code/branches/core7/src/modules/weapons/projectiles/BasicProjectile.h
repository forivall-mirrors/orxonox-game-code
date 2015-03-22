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

/**
    @file BasicProjectile.h
    @brief Definition of the BasicProjectile class.
*/

#ifndef _BasicProjectile_H__
#define _BasicProjectile_H__

#include "weapons/WeaponsPrereqs.h"

#include "worldentities/pawns/Pawn.h"

#include "core/class/OrxonoxInterface.h"

namespace orxonox
{

    /**
    @brief
        Baseclass of all projectiles. Defines the damage the projectile does.

    @author
        Simon Miescher
    @ingroup WeaponsProjectiles
    */
    class _WeaponsExport BasicProjectile : public virtual OrxonoxInterface
    {
        public:
            BasicProjectile();
            virtual ~BasicProjectile();

            /**
            @brief Set the normal damage done by this projectile.
                   Normal damage can be (partially) absorbed by shields.
            @param damage The amount of damage. Must be non-negative.
            */
            inline void setDamage(float damage)
                { if(damage >= 0.0f)  { this->damage_ = damage; return; } orxout(internal_warning) << "The input projectile damage must be non-negative. Ignoring..." << endl; }
            /**
            @brief Get the normal damage done by this projectile.
                   Normal damage can be (partially) absorbed by shields.
            @return Returns the amount of damage. Is non-negative.
            */
            inline float getDamage() const
                { return this->damage_; }

            /**
            @brief Set the health-damage done by this projectile.
                   Health-damage cannot be absorbed by shields.
            @param healthdamage The amount of damage. Must be non-negative.
            */
            inline void setHealthDamage(float healthdamage)
                { if(healthdamage >= 0.0f)  { this->healthdamage_ = healthdamage; return; } orxout(internal_warning) << "The input projectile health-damage must be non-negative. Ignoring..." << endl; }
            /**
            @brief Get the health-damage done by this projectile.
                   Health-damage cannot be absorbed by shields.
            @return healthdamage The amount of damage. Is non-negative.
            */
            inline float getHealthDamage() const
                { return this->healthdamage_; }

            /**
            @brief Set the shield-damage done by this projectile.
                   Shield-damage only reduces shield health.
            @param shielddamage The amount of damage. Must be non-negative.
            */
            inline void setShieldDamage(float shielddamage)
                { if(shielddamage >= 0.0f)  { this->shielddamage_ = shielddamage; return; } orxout(internal_warning) << "The input projectile shield-damage must be non-negative. Ignoring..." << endl; }
            /**
            @brief Get the shield-damage done by this projectile.
                   Shield-damage only reduces shield health.
            @param shielddamage The amount of damage. Is non-negative.
            */
            inline float getShieldDamage() const
                { return this->shielddamage_; }

            /**
            @brief Set the entity that fired the projectile.
            @param shooter A pointer to the Pawn that fired the projectile.
            */
            virtual void setShooter(Pawn* shooter)
                { this->shooter_ = shooter; }
            /**
            @brief Get the entity that fired the projectile.
            @return Returns a pointer to the Pawn that fired the projectile.
            */
            inline Pawn* getShooter(void)
                { return this->shooter_; }

            virtual void destroyObject(void);

        protected:
            bool processCollision(WorldEntity* otherObject, btManifoldPoint& contactPoint, const btCollisionShape* cs);
            void destroyCheck(void);

        private:
            bool isObjectRelatedToShooter(WorldEntity* otherObject);

            WeakPtr<Pawn> shooter_; //!< The entity that fired the projectile.

            float damage_; //!< The amount of normal damage. Normal damage can be (partially) absorbed by shields.
            float healthdamage_; //!< The amount of health-damage. Health-damage cannot be absorbed by shields.
            float shielddamage_; //!< The amount of shield-damage. Shield-damage only reduces shield health.

            bool bDestroy_; //!< Boolean, to check whether a projectile should be destroyed.
    };
}

#endif /* _BasicProjectile_H__ */
