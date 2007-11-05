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

#include "OgreSceneManager.h"

#include "weapon.h"
#include "weapon_manager.h"


namespace orxonox {
  using namespace Ogre;

  Weapon** WeaponManager::weaponList_s = NULL;

  WeaponManager::WeaponManager(SceneManager *sceneMgr, SceneNode *node,
        int slotSize)
        : sceneMgr_(sceneMgr), node_(node), slotSize_(slotSize), slotIndex_(0)
  {
  	slots_ = new Weapon*[slotSize];
  }


  WeaponManager::~WeaponManager()
  {
    if (slots_)
      delete slots_;
  }


  bool WeaponManager::addWeapon(const Ogre::String &name)
  {
    if (name == weaponList_s[0]->name_)
    {
      // this is ugly, but for the time being, it has to fit.
      slots_[slotIndex_++] = weaponList_s[0];
      return true;
    }
    else
      return false;
  }


  // static
  bool WeaponManager::loadWeapons()
  {
    weaponList_s[0] = new Weapon("Barrel Gun", 10, 2);
    return true;
  }


  // static
  void WeaponManager::destroyWeapons()
  {
    delete weaponList_s[0];
  }

}
