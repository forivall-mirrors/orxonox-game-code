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
#include "interfaces/PickupCarrier.h"
#include "interfaces/RadarViewable.h"
#include "worldentities/ControllableEntity.h"

namespace orxonox // tolua_export
{ // tolua_export
    class _OrxonoxExport Pawn // tolua_export
        : public ControllableEntity, public RadarViewable, public PickupCarrier
    { // tolua_export
        friend class WeaponSystem;

        public:
            Pawn(BaseObject* creator);
            virtual ~Pawn();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);

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

            inline void setShieldHealth(float shieldHealth)
            { this->shieldHealth_ = shieldHealth; }
            inline float getShieldHealth()
            { return this->shieldHealth_; }

            inline void setShieldAbsorption(float shieldAbsorption)
            { this->shieldAbsorption_ = shieldAbsorption; }
            inline float getShieldAbsorption()
            { return this->shieldAbsorption_; }

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

            virtual void addedWeaponPack(WeaponPack* wPack) {}

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

            virtual void startLocalHumanControl();

            void setAimPosition( Vector3 position )
                { this->aimPosition_ = position; }
            Vector3 getAimPosition()
                { return this->aimPosition_; }

            virtual const Vector3& getCarrierPosition(void) const
                { return this->getWorldPosition(); };

        protected:
            virtual void preDestroy();

            virtual void setPlayer(PlayerInfo* player);
            virtual void removePlayer();

            virtual void death();
            virtual void goWithStyle();
            virtual void deatheffect();
            virtual void spawneffect();

            virtual void damage(float damage, Pawn* originator = 0);

            bool bAlive_;

            virtual std::vector<PickupCarrier*>* getCarrierChildren(void) const
                { return new std::vector<PickupCarrier*>(); }
            virtual PickupCarrier* getCarrierParent(void) const
                { return NULL; }

            float health_;
            float maxHealth_;
            float initialHealth_;
            float shieldHealth_;
            float shieldAbsorption_; // Has to be between 0 and 1

            WeakPtr<Pawn> lastHitOriginator_;

            WeaponSystem* weaponSystem_;
            bool bReload_;

            std::string spawnparticlesource_;
            float spawnparticleduration_;
            unsigned int numexplosionchunks_;

        private:
            void registerVariables();
            inline void setWeaponSystem(WeaponSystem* weaponsystem)
                { this->weaponSystem_ = weaponsystem; }

            Vector3 aimPosition_;
    }; // tolua_export
} // tolua_export

#endif /* _Pawn_H__ */
