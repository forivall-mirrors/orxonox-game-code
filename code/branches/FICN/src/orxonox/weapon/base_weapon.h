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


#ifndef BASE_WEAPON_H
#define BASE_WEAPON_H

#include "OgrePrerequisites.h"

#include "orxonox/orxonox_prerequisites.h"


namespace orxonox {
namespace weapon {

  class BaseWeapon
  {
  public:
    enum Action {
      NOTHING  = 0,
      RELOAD   = 1,
      CHANGE_AMMO  = 2,
      SPECIAL  = 3
    };

  protected:
    enum State {
      IDLE = 0,
      PRIMARY_FIRE = 1,
      SECONDARY_FIRE = 2,
      RELOADING = 3,
      CHANGING_AMMO = 4,
    };

  public:
    BaseWeapon(InertialNode*, AmmunitionDump*);
	  virtual ~BaseWeapon();

    bool addAction(const Action);

    void primaryFireRequest();

    void secondaryFireRequest();

    int getAmmoState();

    bool tick(unsigned long, Ogre::Real);

  protected:
    virtual void primaryFire() = 0;

    virtual void primaryFiring(unsigned int) = 0;

    virtual void secondaryFire() = 0;

    virtual void secondaryFiring(unsigned int) = 0;

  public:

  protected:
    Ogre::SceneManager *sceneMgr_;
    //Ogre::SceneNode *node_;
    InertialNode* node_;

    int bulletCounter_;
    BulletManager *bulletManager_;

    AmmunitionDump *ammoDump_;

    bool primaryFireRequest_;
    bool secondaryFireRequest_;

    unsigned long actionStartTime_;

    State currentState_;
    bool secondaryFired_;

    Action nextAction_;
    bool actionAdded_;
    unsigned long timeSinceNextActionAdded_;
    static const unsigned long nextActionValidityPeriod_ = 500;

    // weapon properties
    int leftAmmo_;
    Ogre::String name_;
    int primaryFirePower_;
    int secondaryFirePower_;
    int primaryFiringRate_;
    int secondaryFiringRate_;
    Ogre::Real primaryBulletSpeed_;
    Ogre::Real secondaryBulletSpeed_;

    int magazineSize_;

  };

}
}

#endif /* BASE_WEAPON_H */
