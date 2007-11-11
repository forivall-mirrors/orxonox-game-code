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
#include "OgreEntity.h"
#include "OgreSceneNode.h"
#include "OgreVector3.h"
#include "OgreStringConverter.h"

#include "weapon.h"
#include "bullet.h"
#include "bullet_manager.h"
#include "inertial_node.h"
#include "weapon_manager.h"

#define ACTION_LIST_SIZE 4


namespace orxonox {
namespace weapon {
  using namespace Ogre;

  Weapon** WeaponManager::weaponList_s = NULL;

  WeaponManager::WeaponManager(SceneManager *sceneMgr, InertialNode *node,
        BulletManager *bulletManager, int slotSize)
        : sceneMgr_(sceneMgr), node_(node), slotSize_(slotSize), slotIndex_(0),
        bulletCounter_(0), primaryFireRequest_(false), currentState_(IDLE),
        secondaryFireRequest_(false), selectedWeapon_(0),
        bulletManager_(bulletManager), secondaryFired_(false),
        timeSinceNextActionAdded_(0), actionAdded_(false), nextAction_(NOTHING)
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
    if (!weaponList_s)
      return false;

    if (name == weaponList_s[0]->name_)
    {
      // this is ugly, but for the time being, it has to fit.
      selectedWeapon_ = slotIndex_;
      slots_[slotIndex_++] = weaponList_s[0];
      return true;
    }
    else
      return false;
  }


  bool WeaponManager::addAction(const Action act)
  {
    if (nextAction_ != NOTHING)
    {
      nextAction_ = act;
      actionAdded_ = true;
      return true;
    }
    else
      return false;
  }


  void WeaponManager::primaryFireRequest()
  {
    primaryFireRequest_ = true;
  }


  void WeaponManager::primaryFire()
  {
    SceneNode *temp = sceneMgr_->getRootSceneNode()->createChildSceneNode(
          node_->getSceneNode()->getWorldPosition(),
          node_->getSceneNode()->getWorldOrientation());

    Entity* bulletEntity = sceneMgr_->createEntity("BulletEntity"
          + StringConverter::toString(bulletCounter_++), "Barrel.mesh");

    Vector3 speed = (temp->getOrientation() * Vector3(0, 0, -1))
          .normalisedCopy() * slots_[selectedWeapon_]->bulletSpeed_;
    speed += node_->getWorldSpeed();

	  temp->setScale(Vector3(1, 1, 1) * 4);
	  temp->yaw(Degree(-90));

	  bulletManager_->addBullet(new Bullet(temp, bulletEntity, speed));
  }


  void WeaponManager::primaryFiring(unsigned int time)
  {
    if (time > 100)
    {
      currentState_ = IDLE;
    }
  }


  void WeaponManager::secondaryFireRequest()
  {
    secondaryFireRequest_ = true;
  }


  void WeaponManager::secondaryFire()
  {
    SceneNode *temp = sceneMgr_->getRootSceneNode()->createChildSceneNode(
          node_->getSceneNode()->getWorldPosition(),
          node_->getSceneNode()->getWorldOrientation());

    Entity* bulletEntity = sceneMgr_->createEntity("BulletEntity"
          + StringConverter::toString(bulletCounter_++), "Barrel.mesh");

    Vector3 speed = (temp->getOrientation() * Vector3(0, 0, -1))
          .normalisedCopy() * slots_[selectedWeapon_]->bulletSpeed_*0.5;
    speed += node_->getWorldSpeed();

	  temp->setScale(Vector3(1, 1, 1) * 10);
	  temp->yaw(Degree(-90));

	  bulletManager_->addBullet(new Bullet(temp, bulletEntity, speed));
  }


  void WeaponManager::secondaryFiring(unsigned int time)
  {
    if (time > 250)
      currentState_ = IDLE;
  }


  bool WeaponManager::tick(unsigned long time, Real deltaTime)
  {
    // return if no weapon has been added
    if (!slots_[slotIndex_])
      return true;

    // process action adder
    if (actionAdded_)
    {
      timeSinceNextActionAdded_ = time;
      actionAdded_ = false;
    }

    switch (currentState_)
    {
    case IDLE:
      // first, process next action
      if (nextAction_ != NOTHING)
      {
        actionStartTime_ = time;
        switch (nextAction_)
        {
        case RELOAD:
          break;

        case CHANGE_AMMO:
          break;

        case SPECIAL:
          break;

        default:
          break;
        }

        // pay attention when multithreaded!
        nextAction_ = NOTHING;
      }
      else
      {
        // secondly, execute firing
        if (primaryFireRequest_ && !(secondaryFired_ && secondaryFireRequest_))
        {
          actionStartTime_ = time;
          currentState_ = PRIMARY_FIRE;
          secondaryFired_ = false;
          primaryFire();
        }
        else if (secondaryFireRequest_)
        {
          actionStartTime_ = time;
          currentState_ = SECONDARY_FIRE;
          secondaryFired_ = true;
          secondaryFire();
        }
      }

      break;

    case PRIMARY_FIRE:
      primaryFiring((unsigned int)(time - actionStartTime_));
      break;

    case SECONDARY_FIRE:
      secondaryFiring((unsigned int)(time - actionStartTime_));
      break;

    case RELOADING:
      break;

    case CHANGING_AMMO:
      break;
    }

    primaryFireRequest_ = false;
    secondaryFireRequest_ = false;

    if (time - timeSinceNextActionAdded_ > nextActionValidityPeriod_)
      nextAction_ = NOTHING;

    return true;
  }


  // static
  bool WeaponManager::loadWeapons()
  {
    weaponList_s = new Weapon*[5];
    for (int i = 0; i < 5; i++)
      weaponList_s[i] = NULL;
    weaponList_s[0] = new Weapon("Barrel Gun", 10, 2, 1000);
    return true;
  }


  // static
  void WeaponManager::destroyWeapons()
  {
    if (weaponList_s)
    {
      for (int i = 0; i < 5; i++)
        if (weaponList_s[i])
          delete weaponList_s[i];
      delete weaponList_s;
    }
  }

}
}
