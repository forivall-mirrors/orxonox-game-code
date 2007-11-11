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


#ifndef ORXONOX_SHIP_H
#define ORXONOX_SHIP_H

#include "OgrePrerequisites.h"

#include "orxonox_prerequisites.h"


namespace orxonox {

  class OrxonoxShip
  {
  public:
    OrxonoxShip(Ogre::SceneManager*, Ogre::SceneNode*, weapon::BulletManager*);
	  virtual ~OrxonoxShip();

	  virtual bool initialise();

	  void setMainThrust(const Ogre::Real);
	  void setSideThrust(const Ogre::Real);
    void setYThrust(const Ogre::Real);
	  void turnUpAndDown(const Ogre::Radian&);
	  void turnLeftAndRight(const Ogre::Radian&);

    InertialNode* getRootNode();
    Ogre::Vector3 getSpeed();

    weapon::WeaponManager* getMainWeapon();

	  bool tick(unsigned long, Ogre::Real);

  protected:

  protected:
	  Ogre::SceneManager *sceneMgr_;
	  //Ogre::SceneNode *rootNode_;
    InertialNode *rootNode_;
	  Ogre::Entity *shipEntity_;

	  //Ogre::Vector3 currentSpeed_;  // relative to space
	  Ogre::Vector3 currentThrust_; // relative to the ship
    Ogre::Real baseThrust_;
	  int objectCounter_;

    weapon::BulletManager *bulletManager_;
	  //Ogre::Vector3 bulletSpeed_;

    weapon::WeaponManager *mainWeapon_;
  };

}

#endif /* ORXONOX_SHIP_H */
