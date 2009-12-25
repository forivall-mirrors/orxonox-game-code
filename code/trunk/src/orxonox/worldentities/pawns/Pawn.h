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

#include <string>
#include "interfaces/RadarViewable.h"
#include "worldentities/ControllableEntity.h"
#include "pickup/PickupCollection.h"

namespace orxonox
{
    class _OrxonoxExport Pawn : public ControllableEntity, public RadarViewable
    {
        friend class WeaponSystem;

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

            virtual void hit(Pawn* originator, const Vector3& force, float damage);
            virtual void hit(Pawn* originator, btManifoldPoint& contactpoint, float damage);
            virtual void kill();

            virtual void fired(unsigned int firemode);
            virtual void reload();
            virtual void postSpawn();

            void addWeaponSlot(WeaponSlot * wSlot);
            WeaponSlot * getWeaponSlot(unsigned int index) const;
            void addWeaponSet(WeaponSet * wSet);
            WeaponSet * getWeaponSet(unsigned int index) const;
            void addWeaponPack(WeaponPack * wPack);
            void addWeaponPackXML(WeaponPack * wPack);
            WeaponPack * getWeaponPack(unsigned int index) const;

            inline const WorldEntity* getWorldEntity() const
                { return const_cast<Pawn*>(this); }

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

            virtual void dropItems();
            inline PickupCollection& getPickups()
                { return this->pickups_; }
            virtual void useItem()
                { this->pickups_.useItem(); }

            virtual void startLocalHumanControl();

            void setAimPosition( Vector3 position )
                { this->aimPosition_ = position; }
            Vector3 getAimPosition()
                { return this->aimPosition_; }

        protected:
            virtual void setPlayer(PlayerInfo* player);
            virtual void removePlayer();

            virtual void death();
            virtual void goWithStyle();
            virtual void deatheffect();
            virtual void spawneffect();

            virtual void damage(float damage, Pawn* originator = 0);

            bool bAlive_;

            PickupCollection pickups_;

            float health_;
            float maxHealth_;
            float initialHealth_;

            Pawn* lastHitOriginator_;

            WeaponSystem* weaponSystem_;
            bool bReload_;

            std::string spawnparticlesource_;
            float spawnparticleduration_;
            unsigned int numexplosionchunks_;

        private:
            inline void setWeaponSystem(WeaponSystem* weaponsystem)
                { this->weaponSystem_ = weaponsystem; }

            Vector3 aimPosition_;
    };
}

#endif /* _Pawn_H__ */
