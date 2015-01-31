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
            Pawn(Context* context);
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

            virtual void setShieldHealth(float shieldHealth);

            inline float getShieldHealth()
                { return this->shieldHealth_; }

            inline void addShieldHealth(float amount)
                { this->setShieldHealth(this->shieldHealth_ + amount); }

            inline bool hasShield()
                { return (this->getShieldHealth() > 0); }

            virtual void setMaxShieldHealth(float maxshieldhealth);
            inline float getMaxShieldHealth() const
                { return this->maxShieldHealth_; }

            inline void setInitialShieldHealth(float initialshieldhealth)
                { this->initialShieldHealth_ = initialshieldhealth; this->setShieldHealth(initialshieldhealth); }
            inline float getInitialShieldHealth() const
                { return this->initialShieldHealth_; }

            inline void restoreInitialShieldHealth()
                { this->setShieldHealth(this->initialShieldHealth_); }
            inline void restoreMaxShieldHealth()
                { this->setShieldHealth(this->maxShieldHealth_); }

            inline void setShieldAbsorption(float shieldAbsorption)
                { this->shieldAbsorption_ = shieldAbsorption; }
            inline float getShieldAbsorption()
                { return this->shieldAbsorption_; }

            // TODO: Rename to shieldRechargeRate
            virtual void setReloadRate(float reloadrate);
            inline float getReloadRate() const
                { return this->reloadRate_; }

            virtual void setReloadWaitTime(float reloadwaittime);
            inline float getReloadWaitTime() const
                { return this->reloadWaitTime_; }

            inline void resetReloadCountdown()
                { this->reloadWaitCountdown_ = 0; }

            inline void startReloadCountdown()
                { this->reloadWaitCountdown_ = this->getReloadWaitTime(); } // TODO: Implement in Projectile.cc

            virtual void decreaseReloadCountdownTime(float dt);

            inline ControllableEntity* getLastHitOriginator() const
                { return this->lastHitOriginator_; }

            //virtual void hit(Pawn* originator, const Vector3& force, float damage);
            //virtual void hit(Pawn* originator, btManifoldPoint& contactpoint, float damage);
            virtual void hit(Pawn* originator, const Vector3& force, const btCollisionShape* cs, float damage, float healthdamage = 0.0f, float shielddamage = 0.0f);
            virtual void hit(Pawn* originator, btManifoldPoint& contactpoint, const btCollisionShape* cs, float damage, float healthdamage = 0.0f, float shielddamage = 0.0f);

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

            // These are used with the Damage Boost Pickup to use the damage multiplier.
            inline void setDamageMultiplier(float multiplier)
                { this->damageMultiplier_ = multiplier; }
            inline float getDamageMultiplier() const
                { return this->damageMultiplier_; }


            virtual void startLocalHumanControl();

            void setAimPosition( Vector3 position )
                { this->aimPosition_ = position; }
            Vector3 getAimPosition()
                { return this->aimPosition_; }

            virtual const Vector3& getCarrierPosition(void) const
                { return this->getWorldPosition(); };

            virtual void changedVisibility();

            void setExplosionSound(const std::string& engineSound);
            const std::string& getExplosionSound();

        protected:
            virtual void preDestroy();

            virtual void setPlayer(PlayerInfo* player);
            virtual void removePlayer();

            virtual void death();
            virtual bool hasSlaves();
            virtual Controller* getSlave();
            virtual void goWithStyle();
            virtual void deatheffect();
            virtual void spawneffect();

            //virtual void damage(float damage, Pawn* originator = 0);
            virtual void damage(float damage, float healthdamage = 0.0f, float shielddamage = 0.0f, Pawn* originator = NULL, const btCollisionShape* cs = NULL);

            bool bAlive_;

            virtual std::vector<PickupCarrier*>* getCarrierChildren(void) const
                { return new std::vector<PickupCarrier*>(); }
            virtual PickupCarrier* getCarrierParent(void) const
                { return NULL; }


            float health_;
            float maxHealth_;
            float initialHealth_;

            float shieldHealth_;
            float maxShieldHealth_;
            float initialShieldHealth_;
            float shieldAbsorption_; ///< Has to be between 0 and 1
            float reloadRate_;
            float reloadWaitTime_;
            float reloadWaitCountdown_;

            float damageMultiplier_; ///< Used by the Damage Boost Pickup.

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

            WorldSound* explosionSound_; // TODO: Does this really belong here? Maybe move it to BigExplosion?

    }; // tolua_export
} // tolua_export

#endif /* _Pawn_H__ */
