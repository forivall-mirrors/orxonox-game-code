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

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Math.h"
#include "PawnManager.h"
#include "objects/infos/PlayerInfo.h"
#include "objects/gametypes/Gametype.h"
#include "objects/weaponSystem/WeaponSystem.h"

namespace orxonox
{
    CreateFactory(Pawn);

    Pawn::Pawn(BaseObject* creator) : ControllableEntity(creator)
    {
        RegisterObject(Pawn);

        PawnManager::touch();

        this->bAlive_ = true;

        this->health_ = 0;
        this->maxHealth_ = 0;
        this->initialHealth_ = 0;

        this->lastHitOriginator_ = 0;
        this->weaponSystem_ = 0;

        /*
        //WeaponSystem
        weaponSystem_ = new WeaponSystem();
        WeaponSet * weaponSet1 = new WeaponSet(1);
        this->weaponSystem_->attachWeaponSet(weaponSet1);
        this->weaponSystem_->getWeaponSetPointer(0)->getWeaponSlotPointer(0)->setAmmoType(true);
        */

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
        }
    }

    void Pawn::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Pawn, XMLPort, xmlelement, mode);

        XMLPortParam(Pawn, "health", setHealth, getHealth, xmlelement, mode).defaultValues(100);
        XMLPortParam(Pawn, "maxhealth", setMaxHealth, getMaxHealth, xmlelement, mode).defaultValues(200);
        XMLPortParam(Pawn, "initialhealth", setInitialHealth, getInitialHealth, xmlelement, mode).defaultValues(100);
    }

    void Pawn::registerVariables()
    {
        REGISTERDATA(this->bAlive_, direction::toclient);
        REGISTERDATA(this->health_, direction::toclient);
        REGISTERDATA(this->initialHealth_, direction::toclient);
    }

    void Pawn::tick(float dt)
    {
        SUPER(Pawn, tick, dt);

        this->health_ -= 15 * dt * rnd();

        if (this->health_ <= 0)
            this->death();
    }

    void Pawn::setHealth(float health)
    {
        this->health_ = min(health, this->maxHealth_);
    }

    void Pawn::damage(float damage, Pawn* originator)
    {
        this->setHealth(this->health_ - damage);
        this->lastHitOriginator_ = originator;

        // play damage effect
    }

    void Pawn::hit(Pawn* originator, const Vector3& force, float damage)
    {
        this->damage(damage, originator);
        this->setVelocity(this->getVelocity() + force);

        // play hit effect
    }

    void Pawn::kill()
    {
        this->damage(this->health_);
        this->death();
    }

    void Pawn::spawn()
    {
        // play spawn effect
    }

    void Pawn::death()
    {
        // Set bAlive_ to false and wait for PawnManager to do the destruction
        this->bAlive_ = false;

        if (this->getGametype())
            this->getGametype()->pawnKilled(this, this->lastHitOriginator_);

        this->setDestroyWhenPlayerLeft(false);

        if (this->getPlayer())
            this->getPlayer()->stopControl(this);

        // play death effect
    }

    void Pawn::fire()
    {
        if (this->weaponSystem_)
            this->weaponSystem_->fire();
    }

    void Pawn::postSpawn()
    {
        this->setHealth(this->initialHealth_);
        this->spawn();
    }

    ///////////////////
    // Pawn Listener //
    ///////////////////
    PawnListener::PawnListener()
    {
        RegisterRootObject(PawnListener);
    }
}
