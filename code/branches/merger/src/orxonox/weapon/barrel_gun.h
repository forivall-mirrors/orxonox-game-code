/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */


#ifndef BARREL_GUN_H
#define BARREL_GUN_H

#include "OgrePrerequisites.h"

#include "orxonox/orxonox_prerequisites.h"


namespace orxonox {
namespace weapon {

  class BarrelGun : public BaseWeapon
  {
 public:
    BarrelGun(InertialNode*, AmmunitionDump*);
	  virtual ~BarrelGun();

    bool tick(unsigned long, Ogre::Real);

  protected:
    void primaryFire();

    void primaryFiring(unsigned int);

    void secondaryFire();

    void secondaryFiring(unsigned int);

  public:

  protected:

  };
}
}

#endif /* BARREL_GUN_H */
