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
 *      Simon Miescher
 *
 */

#include "Pawn.h"

#include <algorithm>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "network/NetworkFunction.h"

#include "infos/PlayerInfo.h"
#include "controllers/Controller.h"
#include "gametypes/Gametype.h"
#include "graphics/ParticleSpawner.h"
#include "worldentities/ExplosionChunk.h"
#include "worldentities/BigExplosion.h"
#include "weaponsystem/WeaponSystem.h"
#include "weaponsystem/WeaponSlot.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSet.h"

#include "controllers/FormationController.h"

namespace orxonox
{
    CreateFactory(Pawn);

    Pawn::Pawn(BaseObject* creator)
        : ControllableEntity(creator)
        , RadarViewable(creator, static_cast<WorldEntity*>(this))
    {
        RegisterObject(Pawn);

        this->bAlive_ = true;
        this->bReload_ = false;

        this->health_ = 0;
        this->maxHealth_ = 0;
        this->initialHealth_ = 0;

        this->shieldHealth_ = 0;
        this->initialShieldHealth_ = 0;
        this->maxShieldHealth_ = 100; //otherwise shield might increase to float_max
        this->shieldAbsorption_ = 0.5;

        this->reloadRate_ = 0;
        this->reloadWaitTime_ = 1.0f;
        this->reloadWaitCountdown_ = 0;

        this->lastHitOriginator_ = 0;

        // set damage multiplier to default value 1, meaning nominal damage
        this->damageMultiplier_ = 1;

        this->spawnparticleduration_ = 3.0f;

        this->aimPosition_ = Vector3::ZERO;

        if (GameMode::isMaster())
        {
            this->weaponSystem_ = new WeaponSystem(this);
            this->weaponSystem_->setPawn(this);
        }
        else
            this->weaponSystem_ = 0;

        this->setRadarObjectColour(ColourValue::Red);
        this->setRadarObjectShape(RadarViewable::Dot);

        this->registerVariables();

        this->isHumanShip_ = this->hasLocalController();

        this->setSyncMode(ObjectDirection::Bidirectional); // needed to synchronise e.g. aimposition
    }

    Pawn::~Pawn()
    {
        if (this->isInitialized())
        {
            if (this->weaponSystem_)
                this->weaponSystem_->destroy();
        }
    }

    void Pawn::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Pawn, XMLPort, xmlelement, mode);

        XMLPortParam(Pawn, "health", setHealth, getHealth, xmlelement, mode).defaultValues(100);
        XMLPortParam(Pawn, "maxhealth", setMaxHealth, getMaxHealth, xmlelement, mode).defaultValues(200);
        XMLPortParam(Pawn, "initialhealth", setInitialHealth, getInitialHealth, xmlelement, mode).defaultValues(100);

        XMLPortParam(Pawn, "shieldhealth", setShieldHealth, getShieldHealth, xmlelement, mode).defaultValues(0);
        XMLPortParam(Pawn, "initialshieldhealth", setInitialShieldHealth, getInitialShieldHealth, xmlelement, mode).defaultValues(0);
        XMLPortParam(Pawn, "maxshieldhealth", setMaxShieldHealth, getMaxShieldHealth, xmlelement, mode).defaultValues(100);
        XMLPortParam(Pawn, "shieldabsorption", setShieldAbsorption, getShieldAbsorption, xmlelement, mode).defaultValues(0);

        XMLPortParam(Pawn, "spawnparticlesource", setSpawnParticleSource, getSpawnParticleSource, xmlelement, mode);
        XMLPortParam(Pawn, "spawnparticleduration", setSpawnParticleDuration, getSpawnParticleDuration, xmlelement, mode).defaultValues(3.0f);
        XMLPortParam(Pawn, "explosionchunks", setExplosionChunks, getExplosionChunks, xmlelement, mode).defaultValues(7);

        XMLPortObject(Pawn, WeaponSlot, "weaponslots", addWeaponSlot, getWeaponSlot, xmlelement, mode);
        XMLPortObject(Pawn, WeaponSet, "weaponsets", addWeaponSet, getWeaponSet, xmlelement, mode);
        XMLPortObject(Pawn, WeaponPack, "weapons", addWeaponPackXML, getWeaponPack, xmlelement, mode);

        XMLPortParam(Pawn, "reloadrate", setReloadRate, getReloadRate, xmlelement, mode).defaultValues(0);
        XMLPortParam(Pawn, "reloadwaittime", setReloadWaitTime, getReloadWaitTime, xmlelement, mode).defaultValues(1.0f);

        XMLPortParam ( RadarViewable, "radarname", setRadarName, getRadarName, xmlelement, mode );
    }

    void Pawn::registerVariables()
    {
        registerVariable(this->bAlive_,           VariableDirection::ToClient);
        registerVariable(this->health_,           VariableDirection::ToClient);
        registerVariable(this->maxHealth_,        VariableDirection::ToClient);
        registerVariable(this->shieldHealth_,     VariableDirection::ToClient);
        registerVariable(this->maxShieldHealth_,  VariableDirection::ToClient);
        registerVariable(this->shieldAbsorption_, VariableDirection::ToClient);
        registerVariable(this->bReload_,          VariableDirection::ToServer);
        registerVariable(this->aimPosition_,      VariableDirection::ToServer);  // For the moment this variable gets only transfered to the server
    }

    void Pawn::tick(float dt)
    {
        SUPER(Pawn, tick, dt);

        this->bReload_ = false;

        // TODO: use the existing timer functions instead
        if(this->reloadWaitCountdown_ > 0)
        {
            this->decreaseReloadCountdownTime(dt);
        }
        else
        {
            this->addShieldHealth(this->getReloadRate() * dt);
            this->resetReloadCountdown();
        }

        if (GameMode::isMaster())
        {
            if (this->health_ <= 0 && bAlive_)
            {
                this->fireEvent(); // Event to notify anyone who wants to know about the death.
                this->death();
            }
        }
    }

    void Pawn::preDestroy()
    {
        // yay, multiple inheritance!
        this->ControllableEntity::preDestroy();
        this->PickupCarrier::preDestroy();
    }

    void Pawn::setPlayer(PlayerInfo* player)
    {
        ControllableEntity::setPlayer(player);

        if (this->getGametype())
            this->getGametype()->playerStartsControllingPawn(player, this);
    }

    void Pawn::removePlayer()
    {
        if (this->getGametype())
            this->getGametype()->playerStopsControllingPawn(this->getPlayer(), this);

        ControllableEntity::removePlayer();
    }


    void Pawn::setHealth(float health)
    {
        this->health_ = std::min(health, this->maxHealth_); //Health can't be set to a value bigger than maxHealth, otherwise it will be reduced at first hit
    }

    void Pawn::setShieldHealth(float shieldHealth)
    {
        this->shieldHealth_ = std::min(shieldHealth, this->maxShieldHealth_);
    }

    void Pawn::setMaxShieldHealth(float maxshieldhealth)
    {
        this->maxShieldHealth_ = maxshieldhealth;
    }

    void Pawn::setReloadRate(float reloadrate)
    {
        this->reloadRate_ = reloadrate;
    }

    void Pawn::setReloadWaitTime(float reloadwaittime)
    {
        this->reloadWaitTime_ = reloadwaittime;
    }

    void Pawn::decreaseReloadCountdownTime(float dt)
    {
        this->reloadWaitCountdown_ -= dt;
    }

    void Pawn::damage(float damage, float healthdamage, float shielddamage, Pawn* originator)
    {
        // Applies multiplier given by the DamageBoost Pickup.
        if (originator)
            damage *= originator->getDamageMultiplier();

        if (this->getGametype() && this->getGametype()->allowPawnDamage(this, originator))
        {
            if (shielddamage >= this->getShieldHealth())
            {
                this->setShieldHealth(0);
                this->setHealth(this->health_ - (healthdamage + damage));
            }
            else
            {
                this->setShieldHealth(this->shieldHealth_ - shielddamage);

                // remove remaining shieldAbsorpton-Part of damage from shield
                shielddamage = damage * this->shieldAbsorption_;
                shielddamage = std::min(this->getShieldHealth(),shielddamage);
                this->setShieldHealth(this->shieldHealth_ - shielddamage);

                // set remaining damage to health
                this->setHealth(this->health_ - (damage - shielddamage) - healthdamage);
            }

            this->lastHitOriginator_ = originator;
        }
    }

// TODO: Still valid?
/* HIT-Funktionen
    Die hit-Funktionen muessen auch in src/orxonox/controllers/Controller.h angepasst werden! (Visuelle Effekte)

*/
    void Pawn::hit(Pawn* originator, const Vector3& force, float damage, float healthdamage, float shielddamage)
    {
        if (this->getGametype() && this->getGametype()->allowPawnHit(this, originator) && (!this->getController() || !this->getController()->getGodMode()) )
        {
            this->damage(damage, healthdamage, shielddamage, originator);
            this->setVelocity(this->getVelocity() + force);
        }
    }


    void Pawn::hit(Pawn* originator, btManifoldPoint& contactpoint, float damage, float healthdamage, float shielddamage)
    {
        if (this->getGametype() && this->getGametype()->allowPawnHit(this, originator) && (!this->getController() || !this->getController()->getGodMode()) )
        {
            this->damage(damage, healthdamage, shielddamage, originator);

            if ( this->getController() )
                this->getController()->hit(originator, contactpoint, damage); // changed to damage, why shielddamage?
        }
    }


    void Pawn::kill()
    {
        this->damage(this->health_);
        this->death();
    }

    void Pawn::spawneffect()
    {
        // play spawn effect
        if (!this->spawnparticlesource_.empty())
        {
            ParticleSpawner* effect = new ParticleSpawner(this->getCreator());
            effect->setPosition(this->getPosition());
            effect->setOrientation(this->getOrientation());
            effect->setDestroyAfterLife(true);
            effect->setSource(this->spawnparticlesource_);
            effect->setLifetime(this->spawnparticleduration_);
        }
    }

    void Pawn::death()
    {
        /* TEST TEST This is used to find out if the current pawn is also 
         * the master of the formulation.
         *
         * NOTE: This does not yet check if the current pawn is actually 
         *       the humanplayer or not!
         */
        for (ObjectList<FormationController>::iterator it = 
          ObjectList<FormationController>::begin(); 
          it != ObjectList<FormationController>::end(); ++it )
        {
          orxout(user_warning) << "Test! Master: " << it->getMaster() 
            << " My controller: " << this->getPlayer()->getController() << endl;
        }
        /* TEST TEST */

            


        this->setHealth(1);
        if (this->getGametype() && this->getGametype()->allowPawnDeath(this, this->lastHitOriginator_))
        {
            // Set bAlive_ to false and wait for PawnManager to do the destruction
            this->bAlive_ = false;

            this->setDestroyWhenPlayerLeft(false);

            if (this->getGametype())
                this->getGametype()->pawnKilled(this, this->lastHitOriginator_);

            if (this->getPlayer() && this->getPlayer()->getControllableEntity() == this)
                this->getPlayer()->stopControl();

            if (GameMode::isMaster())
            {
//                this->deathEffect();
                this->goWithStyle();
            }
        }
    }
    void Pawn::goWithStyle()
    {
        this->bAlive_ = false;
        this->setDestroyWhenPlayerLeft(false);

        BigExplosion* chunk = new BigExplosion(this->getCreator());
        chunk->setPosition(this->getPosition());

    }
    void Pawn::deatheffect()
    {
        // play death effect
        {
            ParticleSpawner* effect = new ParticleSpawner(this->getCreator());
            effect->setPosition(this->getPosition());
            effect->setOrientation(this->getOrientation());
            effect->setDestroyAfterLife(true);
            effect->setSource("Orxonox/explosion2b");
            effect->setLifetime(4.0f);
        }
        {
            ParticleSpawner* effect = new ParticleSpawner(this->getCreator());
            effect->setPosition(this->getPosition());
            effect->setOrientation(this->getOrientation());
            effect->setDestroyAfterLife(true);
            effect->setSource("Orxonox/smoke6");
            effect->setLifetime(4.0f);
        }
        {
            ParticleSpawner* effect = new ParticleSpawner(this->getCreator());
            effect->setPosition(this->getPosition());
            effect->setOrientation(this->getOrientation());
            effect->setDestroyAfterLife(true);
            effect->setSource("Orxonox/sparks");
            effect->setLifetime(4.0f);
        }
        for (unsigned int i = 0; i < this->numexplosionchunks_; ++i)
        {
            ExplosionChunk* chunk = new ExplosionChunk(this->getCreator());
            chunk->setPosition(this->getPosition());
        }
    }

    void Pawn::fired(unsigned int firemode)
    {
        if (this->weaponSystem_)
            this->weaponSystem_->fire(firemode);
    }

    void Pawn::reload()
    {
        this->bReload_ = true;
    }

    void Pawn::postSpawn()
    {
        this->setHealth(this->initialHealth_);
        if (GameMode::isMaster())
            this->spawneffect();
    }

    /* WeaponSystem:
    *   functions load Slot, Set, Pack from XML and make sure all parent-pointers are set.
    *   with setWeaponPack you can not just load a Pack from XML but if a Pack already exists anywhere, you can attach it.
    *       --> e.g. Pickup-Items
    */
    void Pawn::addWeaponSlot(WeaponSlot * wSlot)
    {
        this->attach(wSlot);
        if (this->weaponSystem_)
            this->weaponSystem_->addWeaponSlot(wSlot);
    }

    WeaponSlot * Pawn::getWeaponSlot(unsigned int index) const
    {
        if (this->weaponSystem_)
            return this->weaponSystem_->getWeaponSlot(index);
        else
            return 0;
    }

    void Pawn::addWeaponSet(WeaponSet * wSet)
    {
        if (this->weaponSystem_)
            this->weaponSystem_->addWeaponSet(wSet);
    }

    WeaponSet * Pawn::getWeaponSet(unsigned int index) const
    {
        if (this->weaponSystem_)
            return this->weaponSystem_->getWeaponSet(index);
        else
            return 0;
    }

    void Pawn::addWeaponPack(WeaponPack * wPack)
    {
        if (this->weaponSystem_)
        {
            this->weaponSystem_->addWeaponPack(wPack);
            this->addedWeaponPack(wPack);
        }
    }

    void Pawn::addWeaponPackXML(WeaponPack * wPack)
    {
        if (this->weaponSystem_)
        {
            if (!this->weaponSystem_->addWeaponPack(wPack))
                wPack->destroy();
            else
                this->addedWeaponPack(wPack);
        }
    }

    WeaponPack * Pawn::getWeaponPack(unsigned int index) const
    {
        if (this->weaponSystem_)
            return this->weaponSystem_->getWeaponPack(index);
        else
            return 0;
    }

    //Tell the Map (RadarViewable), if this is a playership
    void Pawn::startLocalHumanControl()
    {
//        SUPER(ControllableEntity, changedPlayer());
        ControllableEntity::startLocalHumanControl();
        this->isHumanShip_ = true;
    }

    void Pawn::changedVisibility(void)
    {
        SUPER(Pawn, changedVisibility);

        // enable proper radarviewability when the visibility is changed
        this->RadarViewable::settingsChanged();
    }

}
