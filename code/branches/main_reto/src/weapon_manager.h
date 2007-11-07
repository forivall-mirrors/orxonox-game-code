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


#ifndef WEAPON_MANAGER_H
#define WEAPON_MANAGER_H

#include "OgrePrerequisites.h"

#include "orxonox_prerequisites.h"


namespace orxonox {

  class WeaponManager
  {
  public:
    WeaponManager(Ogre::SceneManager*, Ogre::SceneNode*, int);
	  virtual ~WeaponManager();

    bool addWeapon(const Ogre::String&);

    bool static loadWeapons();

    void static destroyWeapons();

  protected:
    Ogre::SceneManager *sceneMgr_;
    Ogre::SceneNode *node_;

    Weapon **slots_;
    int slotSize_;
    int slotIndex_;

    static Weapon **weaponList_s;

  };

}

#endif /* WEAPON_MANAGER_H */
