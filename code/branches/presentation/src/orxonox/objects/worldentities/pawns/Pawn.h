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

#ifndef _Pawn_H__
#define _Pawn_H__

#include "OrxonoxPrereqs.h"
#include "objects/pickup/ShipEquipment.h"
#include "objects/worldentities/ControllableEntity.h"
#include "objects/RadarViewable.h"
#include "objects/weaponSystem/WeaponSystem.h"

namespace orxonox
{
    class _OrxonoxExport Pawn : public ControllableEntity, public RadarViewable
    {
        public:
            Pawn(BaseObject* creator);
            virtual ~Pawn();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            void registerVariables();

            inline bool isAlive() const
                { return this->bAlive_; }

            virtual void setHealth(float health);
            inline void addHealth(float health)
                { this->setHealth(this->health_ + health); }
            inline void removeHealth(float health)
                { this->setHealth(this->health_ - health); }
            inline float getHealth() const
                { return this->health_; }

            inline void setMaxHealth(float maxhealth)
                { this->maxHealth_ = maxhealth; this->setHealth(this->health_); }
            inline float getMaxHealth() const
                { return this->maxHealth_; }

            inline void setInitialHealth(float initialhealth)
                { this->initialHealth_ = initialhealth; this->setHealth(initialhealth); }
            inline float getInitialHealth() const
                { return this->initialHealth_; }

            inline ControllableEntity* getLastHitOriginator() const
                { return this->lastHitOriginator_; }

            virtual void damage(float damage, Pawn* originator = 0);
            virtual void hit(Pawn* originator, const Vector3& force, float damage);
            virtual void kill();

            virtual void fire(WeaponMode::Enum fireMode);
            virtual void postSpawn();

            void setWeaponSlot(WeaponSlot * wSlot);
            WeaponSlot * getWeaponSlot(unsigned int index) const;
            void setWeaponPack(WeaponPack * wPack);
            WeaponPack * getWeaponPack(unsigned int firemode) const;
            void setWeaponSet(WeaponSet * wSet);
            WeaponSet * getWeaponSet(unsigned int index) const;

            inline const WorldEntity* getWorldEntity() const
                { return (WorldEntity*)this; }

            inline void setSpawnParticleSource(const std::string& source)
                { this->spawnparticlesource_ = source; }
            inline const std::string& getSpawnParticleSource() const
                { return this->spawnparticlesource_; }

            inline void setSpawnParticleDuration(float duration)
                { this->spawnparticleduration_ = duration; }
            inline float getSpawnParticleDuration() const
                { return this->spawnparticleduration_; }

            inline void setExplosionChunks(unsigned int chunks)
                { this->numexplosionchunks_ = chunks; }
            inline unsigned int getExplosionChunks() const
                { return this->numexplosionchunks_; }

            inline ShipEquipment& getPickUp()
                {return this->pickUp;}

            virtual void dropItems();

        protected:
            virtual void death();
            virtual void deatheffect();
            virtual void spawneffect();

            ShipEquipment pickUp;
            bool bAlive_;


            float health_;
            float maxHealth_;
            float initialHealth_;

            Pawn* lastHitOriginator_;

            WeaponSystem* weaponSystem_;
            unsigned int fire_;
            unsigned int firehack_;

            std::string spawnparticlesource_;
            float spawnparticleduration_;
            unsigned int numexplosionchunks_;
    };

    class _OrxonoxExport PawnListener : virtual public OrxonoxClass
    {
        friend class Pawn;

        public:
            PawnListener();
            virtual ~PawnListener() {}

        protected:
            virtual void destroyedPawn(Pawn* pawn) = 0;
    };
}

#endif /* _Pawn_H__ */
