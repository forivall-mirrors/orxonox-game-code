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
 *      Martin Polak
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"


#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Debug.h"

#include "Fusion.h"


namespace orxonox
{
    CreateFactory(Fusion);

    Fusion::Fusion(BaseObject* creator) : Weapon(creator)
    {
        RegisterObject(Fusion);

        this->speed_ = 1250;

    }

    Fusion::~Fusion()
    {
    }

    void Fusion::takeBullets()
    {
//COUT(0) << "Fusion::takeBullets" << std::endl;
        this->munition_->removeBullets(1);
        this->bulletTimer(this->bulletLoadingTime_);
    }

    void Fusion::takeMagazines()
    {
        this->munition_->removeMagazines(1);
        this->magazineTimer(this->magazineLoadingTime_);
    }

    void Fusion::createProjectile()
    {
//COUT(0) << "Fusion::createProjectile" << std::endl;
        BillboardProjectile* projectile = new ParticleProjectile(this);
        projectile->setOrientation(this->getWorldOrientation());
        projectile->setPosition(this->getWorldPosition());
        projectile->setVelocity(this->getWorldOrientation() * WorldEntity::FRONT * this->speed_);
    }
}
