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

#include "OrxonoxStableHeaders.h"
#include "Pawn.h"

#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Math.h"
#include "PawnManager.h"
#include "objects/infos/PlayerInfo.h"
#include "objects/gametypes/Gametype.h"
#include "objects/worldentities/ParticleSpawner.h"
#include "objects/worldentities/ExplosionChunk.h"
#include "objects/worldentities/BigExplosion.h"

namespace orxonox
{
    CreateFactory(Pawn);

    Pawn::Pawn(BaseObject* creator) : ControllableEntity(creator)
    {
        RegisterObject(Pawn);

        PawnManager::touch();
        this->bAlive_ = true;
        this->fire_ = 0x0;
        this->firehack_ = 0x0;

        this->health_ = 0;
        this->maxHealth_ = 0;
        this->initialHealth_ = 0;

        this->lastHitOriginator_ = 0;

        this->spawnparticleduration_ = 3.0f;

        this->getPickUp().setPlayer(this);

        if (Core::isMaster())
        {
            this->weaponSystem_ = new WeaponSystem(this);
            this->weaponSystem_->setParentPawn(this);
        }
        else
            this->weaponSystem_ = 0;

        this->setRadarObjectColour(ColourValue::Red);
        this->setRadarObjectShape(RadarViewable::Dot);

        this->registerVariables();
    }

    Pawn::~Pawn()
    {
        if (this->isInitialized())
        {
            for (ObjectList<PawnListener>::iterator it = ObjectList<PawnListener>::begin(); it != ObjectList<PawnListener>::end(); ++it)
                it->destroyedPawn(this);

            if (this->weaponSystem_)
                delete this->weaponSystem_;
        }
    }

    void Pawn::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Pawn, XMLPort, xmlelement, mode);

        XMLPortParam(Pawn, "health", setHealth, getHealth, xmlelement, mode).defaultValues(100);
        XMLPortParam(Pawn, "maxhealth", setMaxHealth, getMaxHealth, xmlelement, mode).defaultValues(200);
        XMLPortParam(Pawn, "initialhealth", setInitialHealth, getInitialHealth, xmlelement, mode).defaultValues(100);
        XMLPortParam(Pawn, "spawnparticlesource", setSpawnParticleSource, getSpawnParticleSource, xmlelement, mode);
        XMLPortParam(Pawn, "spawnparticleduration", setSpawnParticleDuration, getSpawnParticleDuration, xmlelement, mode).defaultValues(3.0f);
        XMLPortParam(Pawn, "explosionchunks", setExplosionChunks, getExplosionChunks, xmlelement, mode).defaultValues(7);

        XMLPortObject(Pawn, WeaponSlot, "weaponslots", setWeaponSlot, getWeaponSlot, xmlelement, mode);
        XMLPortObject(Pawn, WeaponSet, "weaponsets", setWeaponSet, getWeaponSet, xmlelement, mode);
        XMLPortObject(Pawn, WeaponPack, "weapons", setWeaponPack, getWeaponPack, xmlelement, mode);
    }

    void Pawn::registerVariables()
    {
        registerVariable(this->bAlive_,        variableDirection::toclient);
        registerVariable(this->health_,        variableDirection::toclient);
        registerVariable(this->initialHealth_, variableDirection::toclient);
        registerVariable(this->fire_,          variableDirection::toserver);
    }

    void Pawn::tick(float dt)
    {
        SUPER(Pawn, tick, dt);

        if (this->weaponSystem_)
        {
            if (this->fire_ & WeaponMode::fire)
                this->weaponSystem_->fire(WeaponMode::fire);
            if (this->fire_ & WeaponMode::altFire)
                this->weaponSystem_->fire(WeaponMode::altFire);
            if (this->fire_ & WeaponMode::altFire2)
                this->weaponSystem_->fire(WeaponMode::altFire2);
        }
        this->fire_ = this->firehack_;
        this->firehack_ = 0x0;

        if (this->health_ <= 0 && bAlive_){
            this->death();
        }
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
        this->health_ = min(health, this->maxHealth_);
    }

    void Pawn::damage(float damage, Pawn* originator)
    {
        if (this->getGametype() && this->getGametype()->allowPawnDamage(this, originator))
        {
            this->setHealth(this->health_ - damage);
            this->lastHitOriginator_ = originator;

            // play damage effect
        }
    }

    void Pawn::hit(Pawn* originator, const Vector3& force, float damage)
    {
        if (this->getGametype() && this->getGametype()->allowPawnHit(this, originator))
        {
            this->damage(damage, originator);
            this->setVelocity(this->getVelocity() + force);

            // play hit effect
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
        if (this->spawnparticlesource_ != "")
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
        if (this->getGametype() && this->getGametype()->allowPawnDeath(this, this->lastHitOriginator_))
        {
            // Set bAlive_ to false and wait for PawnManager to do the destruction
            this->bAlive_ = false;

            this->setDestroyWhenPlayerLeft(false);

            if (this->getGametype())
                this->getGametype()->pawnKilled(this, this->lastHitOriginator_);

            if (this->getPlayer())
                this->getPlayer()->stopControl(this);

            if (Core::isMaster())
            // this->deathEffect();
                this->goWithStyle();
        }
        else
            this->setHealth(1);
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

    void Pawn::fire(WeaponMode::Enum fireMode)
    {
        this->firehack_ |= fireMode;
    }

    void Pawn::postSpawn()
    {
        this->setHealth(this->initialHealth_);
        if (Core::isMaster())
            this->spawneffect();
    }

    void Pawn::dropItems()
    {
	pickUp.eraseAll();
    }

    void Pawn::setWeaponSlot(WeaponSlot * wSlot)
    {
        this->attach(wSlot);
        if (this->weaponSystem_)
            this->weaponSystem_->attachWeaponSlot(wSlot);
    }

    WeaponSlot * Pawn::getWeaponSlot(unsigned int index) const
    {
        if (this->weaponSystem_)
            return this->weaponSystem_->getWeaponSlotPointer(index);
        else
            return 0;
    }

    void Pawn::setWeaponPack(WeaponPack * wPack)
    {
        if (this->weaponSystem_)
        {
            wPack->setParentWeaponSystem(this->weaponSystem_);
            wPack->setParentWeaponSystemToAllWeapons(this->weaponSystem_);
            this->weaponSystem_->attachWeaponPack( wPack,wPack->getFireMode() );
            wPack->attachNeededMunitionToAllWeapons();
        }
    }

    WeaponPack * Pawn::getWeaponPack(unsigned int firemode) const
    {
        if (this->weaponSystem_)
            return this->weaponSystem_->getWeaponPackPointer(firemode);
        else
            return 0;
    }

    void Pawn::setWeaponSet(WeaponSet * wSet)
    {
        if (this->weaponSystem_)
            this->weaponSystem_->attachWeaponSet(wSet);
    }

    WeaponSet * Pawn::getWeaponSet(unsigned int index) const
    {
        if (this->weaponSystem_)
            return this->weaponSystem_->getWeaponSetPointer(index);
        else
            return 0;
    }


    ///////////////////
    // Pawn Listener //
    ///////////////////
    PawnListener::PawnListener()
    {
        RegisterRootObject(PawnListener);
    }
}
