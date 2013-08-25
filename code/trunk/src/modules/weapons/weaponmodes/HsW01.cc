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
    @file HsW01.h
    @brief Implementation of the HsW01 class.
*/

#include "HsW01.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/command/Executor.h"

#include "graphics/Model.h"
#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSystem.h"
#include "worldentities/WorldEntity.h"
#include "worldentities/pawns/Pawn.h"

#include "weapons/projectiles/Projectile.h"
#include "weapons/MuzzleFlash.h"

namespace orxonox
{
    RegisterClass(HsW01);

    HsW01::HsW01(Context* context) : WeaponMode(context)
    {
        RegisterObject(HsW01);

        this->reloadTime_ = 0.25f;
        this->damage_ = 0.0f; //default 15
        this->speed_ = 2500.0f;
        this->delay_ = 0.0f;
        this->setMunitionName("LaserMunition");
        this->mesh_ = "laserbeam.mesh";
        this->sound_ = "sounds/Weapon_HsW01.ogg";


        this->delayTimer_.setTimer(1.0f, false, createExecutor(createFunctor(&HsW01::shot, this)));
        this->delayTimer_.stopTimer();

        this->setDefaultSound(this->sound_);
    }

    HsW01::~HsW01()
    {
    }

    void HsW01::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(HsW01, XMLPort, xmlelement, mode);

        XMLPortParam(HsW01, "delay", setDelay, getDelay, xmlelement, mode);
        XMLPortParam(HsW01, "material", setMaterial, getMaterial, xmlelement, mode);
        XMLPortParam(HsW01, "projectileMesh", setMesh, getMesh, xmlelement, mode);
	XMLPortParam(HsW01, "sound", setSound, getSound, xmlelement, mode);
    }

    /**
    @brief
        Set the firing delay.
    @param delay
        The firing delay in seconds.
    */
    void HsW01::setDelay(float delay)
    {
        this->delay_ = delay;
        this->delayTimer_.setInterval(this->delay_);
    }

    void HsW01::fire()
    {
        this->delayTimer_.startTimer();
    }

    /**
    @brief
        Fires the weapon. Creates a projectile and fires it.
    */
    void HsW01::shot()
    {
        assert( this->getWeapon() && this->getWeapon()->getWeaponPack() && this->getWeapon()->getWeaponPack()->getWeaponSystem() && this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn() );

        // Create the projectile.
        Projectile* projectile = new Projectile(this->getContext());
        Model* model = new Model(projectile->getContext());
        model->setMeshSource(mesh_);
        model->setCastShadows(false);
        projectile->attach(model);
        model->setScale(5);

        this->computeMuzzleParameters(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getAimPosition());
        projectile->setOrientation(this->getMuzzleOrientation());
        projectile->setPosition(this->getMuzzlePosition());
        projectile->setVelocity(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn()->getVelocity() + this->getMuzzleDirection() * this->speed_);

        projectile->setShooter(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        projectile->setDamage(this->getDamage());
        projectile->setShieldDamage(this->getShieldDamage());
        projectile->setHealthDamage(this->getHealthDamage());

        // Display the muzzle flash.
        this->HsW01::muzzleflash();
    }

    /**
    @brief
        Displays the muzzle flash.
    */
    void HsW01::muzzleflash()
    {
        MuzzleFlash *muzzleFlash = new MuzzleFlash(this->getContext());
        this->getWeapon()->attach(muzzleFlash);
        muzzleFlash->setPosition(this->getMuzzleOffset());
        muzzleFlash->setMaterial(this->material_);
    }
}
