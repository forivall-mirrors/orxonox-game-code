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
 *      ...
 *
 */

#include "EnergyDrink.h"

#include "core/CoreIncludes.h"
#include "core/Executor.h"
#include "core/XMLPort.h"
#include "graphics/Model.h"

#include "weapons/projectiles/Projectile.h"
#include "weapons/MuzzleFlash.h"
#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/WeaponSystem.h"

namespace orxonox
{
    CreateFactory(EnergyDrink);

    EnergyDrink::EnergyDrink(BaseObject* creator) : WeaponMode(creator)
    {
        RegisterObject(EnergyDrink);

        this->reloadTime_ = 0.25;
        this->damage_ = 15;
        this->speed_ = 2500;
        this->delay_ = 0;
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

    void EnergyDrink::setMaterial(const std::string& material)
    {
        this->material_ = material;
    }

    void EnergyDrink::setDelay(float d)
    {
        this->delay_ = d;
        this->delayTimer_.setInterval(this->delay_);
    }

    float EnergyDrink::getDelay() const
    {
        return this->delay_;
    }

    void EnergyDrink::fire()
    {
        this->delayTimer_.startTimer();
    }

    void EnergyDrink::muendungsfeuer()
    {
        MuzzleFlash *muzzleFlash = new MuzzleFlash(this);
        this->getWeapon()->attach(muzzleFlash);
        muzzleFlash->setPosition(this->getMuzzleOffset());
        muzzleFlash->setMaterial(this->material_);
    }

    void EnergyDrink::shot()
    {
        Projectile* projectile = new Projectile(this);
        Model* model = new Model(projectile);
        model->setMeshSource("can.mesh");
        model->setCastShadows(false);
        projectile->attach(model);
        model->setScale(5);

        projectile->setOrientation(this->getMuzzleOrientation());
        projectile->setPosition(this->getMuzzlePosition());
        projectile->setVelocity(this->getMuzzleDirection() * this->speed_);

        projectile->setOwner(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        projectile->setDamage(this->getDamage());

        EnergyDrink::muendungsfeuer();
    }
}
