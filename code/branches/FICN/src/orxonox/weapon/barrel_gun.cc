/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "OgreMath.h"
#include "OgreVector3.h"
#include "OgreStringConverter.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreSceneManager.h"

#include "orxonox/inertial_node.h"

#include "bullet.h"
#include "bullet_manager.h"
#include "ammunition_dump.h"
#include "base_weapon.h"

#include "barrel_gun.h"


namespace orxonox {
namespace weapon {
  using namespace Ogre;

  BarrelGun::BarrelGun(InertialNode *node, AmmunitionDump *ammoDump)
        : BaseWeapon(node, ammoDump)
  {
    name_ = "Barrel Gun";
    primaryFirePower_ = 100;
    secondaryFirePower_ = 500;
    primaryFiringRate_ = 10;
    secondaryFiringRate_ = 2;
    primaryBulletSpeed_ = 1000;
    secondaryBulletSpeed_ = 500;
    magazineSize_ = 25;
  }


  BarrelGun::~BarrelGun()
  {
  }


  void BarrelGun::primaryFire()
  {
    if (leftAmmo_ < 1)
    {
      currentState_ = IDLE;
      return;
    }

    SceneNode *temp = sceneMgr_->getRootSceneNode()->createChildSceneNode(
          node_->getSceneNode()->getWorldPosition(),
          node_->getSceneNode()->getWorldOrientation());

    Entity* bulletEntity = sceneMgr_->createEntity("BulletEntity"
          + StringConverter::toString(bulletCounter_++), "Barrel.mesh");

    Vector3 speed = (temp->getOrientation() * Vector3(0, 0, -1))
          .normalisedCopy() * primaryBulletSpeed_;
    speed += node_->getWorldSpeed();

	  temp->setScale(Vector3(1, 1, 1) * 4);
	  temp->yaw(Degree(-90));

	  bulletManager_->addBullet(new Bullet(temp, bulletEntity, speed));

    --leftAmmo_;
  }


  void BarrelGun::primaryFiring(unsigned int time)
  {
    if (time > (unsigned int)1000/primaryFiringRate_)
    {
      currentState_ = IDLE;
    }
  }



  void BarrelGun::secondaryFire()
  {
    if (leftAmmo_ < 5)
    {
      currentState_ = IDLE;
      return;
    }

    SceneNode *temp = sceneMgr_->getRootSceneNode()->createChildSceneNode(
          node_->getSceneNode()->getWorldPosition(),
          node_->getSceneNode()->getWorldOrientation());

    Entity* bulletEntity = sceneMgr_->createEntity("BulletEntity"
          + StringConverter::toString(bulletCounter_++), "Barrel.mesh");

    Vector3 speed = (temp->getOrientation() * Vector3(0, 0, -1))
          .normalisedCopy() * secondaryBulletSpeed_*0.5;
    speed += node_->getWorldSpeed();

	  temp->setScale(Vector3(1, 1, 1) * 10);
	  temp->yaw(Degree(-90));

	  bulletManager_->addBullet(new Bullet(temp, bulletEntity, speed));

    leftAmmo_ -= 5;
  }


  void BarrelGun::secondaryFiring(unsigned int time)
  {
    if (time > (unsigned int)1000/secondaryFiringRate_)
      currentState_ = IDLE;
  }

}
}
