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

#include "OrxonoxStableHeaders.h"
#include "HsW01.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "objects/weaponsystem/projectiles/ParticleProjectile.h"
#include "objects/worldentities/Model.h"
#include "objects/worldentities/MuzzleFlash.h"

#include "objects/weaponsystem/Weapon.h"
#include "objects/weaponsystem/WeaponPack.h"
#include "objects/weaponsystem/WeaponSystem.h"

namespace orxonox
{
    CreateFactory(HsW01);

    HsW01::HsW01(BaseObject* creator) : WeaponMode(creator)
    {
        RegisterObject(HsW01);

        this->reloadTime_ = 0.25;
        this->damage_ = 15;
        this->speed_ = 2500;
        this->delay_ = 0;
        this->setMunitionName("HsW01");

        this->delayTimer_.setTimer(1.0f, false, this, createExecutor(createFunctor(&HsW01::shot)));
        this->delayTimer_.stopTimer();
    }

    void HsW01::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(HsW01, XMLPort, xmlelement, mode);

        XMLPortParam(HsW01, "delay", setDelay, getDelay, xmlelement, mode);
        XMLPortParam(HsW01, "material", setMaterial, getMaterial, xmlelement, mode);

    }

    void HsW01::setMaterial(const std::string& material)
    {
        this->material_ = material;
    }

    std::string& HsW01::getMaterial()
    {
        return this->material_;
    }

    void HsW01::setDelay(float d)
    {
        this->delay_ = d;
        this->delayTimer_.setInterval(this->delay_);
    }

    float HsW01::getDelay() const
    {
        return this->delay_;
    }

    void HsW01::fire()
    {
        this->delayTimer_.startTimer();
    }

    void HsW01::muendungsfeuer()
    {
        MuzzleFlash *muzzleFlash = new MuzzleFlash(this);
        this->getWeapon()->attach(muzzleFlash);
        muzzleFlash->setPosition(this->getMuzzleOffset());
        muzzleFlash->setMaterial(this->material_);
    }

    void HsW01::shot()
    {
        Projectile* projectile = new Projectile(this);
	Model* model = new Model(projectile);
	model->setMeshSource("laserbeam.mesh");
	model->setCastShadows(false);
	projectile->attach(model);
	model->setScale(5);

        projectile->setOrientation(this->getMuzzleOrientation());
        projectile->setPosition(this->getMuzzlePosition());
        projectile->setVelocity(this->getMuzzleDirection() * this->speed_);

        projectile->setOwner(this->getWeapon()->getWeaponPack()->getWeaponSystem()->getPawn());
        projectile->setDamage(this->getDamage());

        HsW01::muendungsfeuer();
    }
}
