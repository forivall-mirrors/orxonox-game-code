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
 *      Hagen Seifert
 *   Co-authors:
 *      simonmie
 *
 */

/**
    @file EnergyDrink.h
    @brief Implementation of the EnergyDrink class.
*/

#include "EnergyDrink.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/command/Executor.h"

#include "graphics/Model.h"
#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSystem.h"
#include "worldentities/pawns/Pawn.h"

#include "weapons/projectiles/Projectile.h"
#include "weapons/MuzzleFlash.h"

namespace orxonox
{
    RegisterClass(EnergyDrink);

    EnergyDrink::EnergyDrink(Context* context) : WeaponMode(context)
    {
        RegisterObject(EnergyDrink);

        this->reloadTime_ = 0.25f;
        this->damage_ = 0.0f;
        this->speed_ = 2500.0f;
        this->delay_ = 0.0f;
        this->setMunitionName("FusionMunition");

        this->delayTimer_.setTimer(1.0f, false, createExecutor(createFunctor(&EnergyDrink::shot, this)));
        this->delayTimer_.stopTimer();
    }

    void EnergyDrink::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EnergyDrink, XMLPort, xmlelement, mode);

        XMLPortParam(EnergyDrink, "delay", setDelay, getDelay, xmlelement, mode);
        XMLPortParam(EnergyDrink, "material", setMaterial, getMaterial, xmlelement, mode);
    }

    /**
    @brief
        Sets the delay with which is fired.
    @param delay
        The delay in seconds.
    */
    void EnergyDrink::setDelay(float delay)
    {
        this->delay_ = delay;
        this->delayTimer_.setInterval(this->delay_);
    }

    /**
    @brief
        Fires the weapon.
    */
    void EnergyDrink::fire()
    {
        this->delayTimer_.startTimer();
    }

    /**
    @brief
        Executes the shot, be creating the projectile and sending it on its way.
    */
    void EnergyDrink::shot()
    {
        // Create the projectile
        Projectile* projectile = new Projectile(this->getContext());
        Model* model = new Model(projectile->getContext());
        model->setMeshSource("can.mesh");
        model->setCastShadows(false);
        projectile->attach(model);
        model->setScale(5);

        projectile->setOrientation(this->getMuzzleOrientation());
        projectile->setPosition(this->getMuzzlePosition());
        projectile->setVelocity(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getVelocity() + this->getMuzzleDirection() * this->speed_);

        projectile->setShooter(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        projectile->setDamage(this->getDamage());
        projectile->setShieldDamage(this->getShieldDamage());
        projectile->setHealthDamage(this->getHealthDamage());

        // Display a muzzle flash.
        this->muzzleflash();
    }

    /**
    @brief
        Displays a muzzle flash.
    */
    void EnergyDrink::muzzleflash()
    {
        MuzzleFlash *muzzleFlash = new MuzzleFlash(this->getContext());
        this->getWeapon()->attach(muzzleFlash);
        muzzleFlash->setPosition(this->getMuzzleOffset());
        muzzleFlash->setMaterial(this->material_);
    }
}
