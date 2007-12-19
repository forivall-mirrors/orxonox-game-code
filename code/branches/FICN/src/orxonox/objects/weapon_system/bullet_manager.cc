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

#include "OgreSceneNode.h"

#include "bullet_manager.h"
#include "bullet.h"


namespace orxonox {
  using namespace Ogre;

  CreateFactory(BulletManager);

  BulletManager::BulletManager() : bulletsIndex_(0), bulletsSize_(8)
  {
    RegisterObject(BulletManager);
    bullets_ = new Bullet*[bulletsSize_];
  }


  BulletManager::~BulletManager()
  {
    // clean up the bullet list
    if (bullets_)
    {
      for (int i = 0; i < bulletsIndex_; i++)
        delete bullets_[i];
      delete bullets_;
    }
  }


  void BulletManager::addBullet(Bullet *bullet)
  {
    // resize array if neccessary (double the size then)
    if (bulletsIndex_ >= bulletsSize_)
    {
      // redimension the array
      Bullet **tempArray = new Bullet*[2*bulletsSize_];
      for (int i = 0; i < bulletsSize_; i++)
        tempArray[i] = bullets_[i];
      bulletsSize_ *= 2;
      delete bullets_;
      bullets_ = tempArray;
    }

    // add the bullet to the list
    bullets_[bulletsIndex_++] = bullet;
  }


  void BulletManager::tick(float dt)
  {
    // update the bullet positions
    for (int i = 0; i < bulletsIndex_; i++)
    {
      bullets_[i]->getNode()->translate(bullets_[i]->getVelocity()*dt);
      //bullets_[i]->getNode()->yaw(Degree(dt*100));
      //bullets_[i]->getNode()->roll(Degree(dt*300));
    }
  }

  int BulletManager::getAmmunitionID(const Ogre::String &ammoName)
  {
    Ogre::String ammoTypes[] = { "Energy Cell", "Barrel", "Lead Shot" };
    int ammoTypesLength = 3;

    for (int i = 0; i < ammoTypesLength; i++)
    {
      if (ammoTypes[i] == ammoName)
        return i;
    }
    return -1;
  }

  int BulletManager::getNumberOfAmmos()
  {
    return 3;
  }

}
