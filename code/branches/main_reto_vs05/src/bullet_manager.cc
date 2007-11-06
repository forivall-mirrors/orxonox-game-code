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

#include "bullet_manager.h"
#include "bullet.h"


namespace orxonox {
  using namespace Ogre;

  BulletManager::BulletManager(SceneManager *sceneMgr) : sceneMgr_(sceneMgr),
        bulletsIndex_(0), bulletsSize_(10)
  {
    bullets_ = new Bullet*[10];
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


  bool BulletManager::tick(unsigned long time, Real deltaTime)
  {
    // update the bullet positions
    for (int i = 0; i < bulletsIndex_; i++)
    {
      bullets_[i]->node_->translate(bullets_[i]->speed_*deltaTime);
      bullets_[i]->node_->yaw(Degree(deltaTime*100));
      bullets_[i]->node_->roll(Degree(deltaTime*300));
    }

    return true;
  }

}
