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

#include <OgreStringConverter.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>

#include "misc/Math.h"
#include "Bullet.h"
#include "BulletManager.h"
#include "AmmunitionDump.h"
#include "BaseWeapon.h"

#include "BarrelGun.h"


namespace orxonox {
  CreateFactory(BarrelGun);

  BarrelGun::BarrelGun()
  {
    RegisterObject(BarrelGun);

    primaryFirePower_ = 100;
    secondaryFirePower_ = 500;
    primaryFiringRate_ = 1.0/7.0;
    secondaryFiringRate_ = 1.0/2.0;
    primaryBulletSpeed_ = 800;
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

    Ogre::SceneNode *temp = sceneMgr_->getRootSceneNode()->createChildSceneNode(
          getNode()->getWorldPosition(),
          getNode()->getWorldOrientation());

    Ogre::Entity* bulletEntity = sceneMgr_->createEntity("BulletEntity"
      + Ogre::StringConverter::toString(bulletCounter_++), "Barrel.mesh");

    Vector3 speed = (temp->getOrientation() * Vector3(1, 0, 0))
          .normalisedCopy() * primaryBulletSpeed_;
    speed += getVelocity();

	  temp->setScale(Vector3(1, 1, 1) * 1.5);
    temp->roll(Degree(90));

    Bullet* bullet = new Bullet();
    bullet->setNode(temp);
    bullet->attachObject(bulletEntity);
    bullet->setVelocity(speed);
	  bulletManager_->addBullet(bullet);

    --leftAmmo_;
  }


  void BarrelGun::primaryFiring(float time)
  {
    if (time > primaryFiringRate_)
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

    Ogre::SceneNode *temp = sceneMgr_->getRootSceneNode()->createChildSceneNode(
          getNode()->getWorldPosition(),
          getNode()->getWorldOrientation());

    Ogre::Entity* bulletEntity = sceneMgr_->createEntity("BulletEntity"
      + Ogre::StringConverter::toString(bulletCounter_++), "Barrel.mesh");

    Vector3 speed = (temp->getOrientation() * Vector3(1, 0, 0))
          .normalisedCopy() * secondaryBulletSpeed_*0.5;
    speed += getVelocity();

	  temp->setScale(Vector3(1, 1, 1) * 3);
	  temp->roll(Degree(90));

    Bullet* bullet = new Bullet();
    bullet->setNode(temp);
    bullet->attachObject(bulletEntity);
    bullet->setVelocity(speed);
	  bulletManager_->addBullet(bullet);


    leftAmmo_ -= 5;
  }


  void BarrelGun::secondaryFiring(float time)
  {
    if (time > secondaryFiringRate_)
      currentState_ = IDLE;
  }

}
