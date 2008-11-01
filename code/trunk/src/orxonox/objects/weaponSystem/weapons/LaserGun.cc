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


namespace orxonox
{
    LaserGun::LaserGun()
    {
        RegisterObject(LaserGun);

        projectileColor_ = ColourValue(1.0, 1.0, 0.5)
    }

    LaserGun::~LaserGun()
    {
    }

    LaserGun::fire()
    {
            BillboardProjectile* projectile = new ParticleProjectile(this);
            projectile->setColour(this->projectileColor_);
            projectile->create();
            if (projectile->getClassID() == 0)
            {
              COUT(3) << "generated projectile with classid 0" <<  std::endl; // TODO: remove this output
            }

            projectile->setObjectMode(0x3);
    }

    LaserGun::addMunition()
    {
        //this->munition_ = ;
    }

    void LaserGun::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {

    }

    ColorValue LaserGun::getProjectileColor()
    {
        return projectileColor_;
    }
}
