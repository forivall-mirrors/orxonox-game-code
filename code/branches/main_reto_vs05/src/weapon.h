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


#ifndef WEAPON_H
#define WEAPON_H

#include "OgreString.h"

#include "orxonox_prerequisites.h"

namespace orxonox {

  class Weapon
  {
  public:
    Weapon(const Ogre::String &name, int firePower, int firingRate)
          : name_(name), firePower_(firePower), firingRate_(firingRate) { }
    virtual ~Weapon() { }

  public:
    Ogre::String name_;
    int firePower_;
    int firingRate_;
  };

}

#endif /* WEAPON_H */
