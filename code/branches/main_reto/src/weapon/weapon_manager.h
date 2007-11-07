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
namespace weapon {

  class WeaponManager
  {
  public:
    enum Action {
      NOTHING  = 0,
      RELOAD   = 1,
      ZOOM_IN  = 2,
      ZOOM_OUT = 3
    };

  protected:
    enum State {
      IDLE = 0,
      PRIMARY_FIRE = 1,
      SECONDARY_FIRE = 2,
      RELOADING = 4
    };

  public:
    WeaponManager(Ogre::SceneManager*, InertialNode*, BulletManager*, int);
	  virtual ~WeaponManager();

    bool addWeapon(const Ogre::String&);

    bool addAction(const Action);

    void primaryFireRequest();

    void secondaryFireRequest();

    bool tick(unsigned long, Ogre::Real);

    bool static loadWeapons();

    void static destroyWeapons();

  protected:
    void primaryFire();

    void secondaryFire();

  public:

  protected:
    Ogre::SceneManager *sceneMgr_;
    //Ogre::SceneNode *node_;
    InertialNode* node_;

    Weapon **slots_;
    int slotSize_;
    int slotIndex_;
    int selectedWeapon_;

    int bulletCounter_;
    BulletManager *bulletManager_;

    bool primaryFireRequest_;
    bool secondaryFireRequest_;

    State currentState_;

    Action *actionList_;
    int actionListReadIndex_;
    int actionListWriteIndex_;

    static Weapon **weaponList_s;

  };

}
}

#endif /* WEAPON_MANAGER_H */
