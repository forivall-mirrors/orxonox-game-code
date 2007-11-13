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

#include "OgreMath.h"
#include "OgreVector3.h"
#include "OgreStringConverter.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreSceneManager.h"

#include "bullet.h"
#include "bullet_manager.h"
#include "inertial_node.h"
#include "ammunition_dump.h"

#include "base_weapon.h"


namespace orxonox {
namespace weapon {
  using namespace Ogre;

  BaseWeapon::BaseWeapon(SceneManager *sceneMgr, InertialNode *node,
        BulletManager *bulletManager, AmmunitionDump *ammoDump)
        : sceneMgr_(sceneMgr), node_(node),
        bulletCounter_(0), primaryFireRequest_(false), currentState_(IDLE),
        secondaryFireRequest_(false),
        bulletManager_(bulletManager), secondaryFired_(false),
        timeSinceNextActionAdded_(0), actionAdded_(false), nextAction_(NOTHING),
        name_("Base Weapon"), primaryFirePower_(100), secondaryFirePower_(500),
        primaryFiringRate_(10), secondaryFiringRate_(2), primaryBulletSpeed_(1000),
        secondaryBulletSpeed_(500), magazineSize_(25), ammoDump_(ammoDump)
  {
    leftAmmo_ = ammoDump_->getAmmunition(magazineSize_);
  }


  BaseWeapon::~BaseWeapon()
  {
  }


  bool BaseWeapon::addAction(const Action act)
  {
    if (nextAction_ == NOTHING)
    {
      nextAction_ = act;
      actionAdded_ = true;
      return true;
    }
    else
      return false;
  }


  void BaseWeapon::primaryFireRequest()
  {
    primaryFireRequest_ = true;
  }


  void BaseWeapon::primaryFire()
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


  void BaseWeapon::primaryFiring(unsigned int time)
  {
    if (time > 100)
    {
      currentState_ = IDLE;
    }
  }


  void BaseWeapon::secondaryFireRequest()
  {
    secondaryFireRequest_ = true;
  }


  void BaseWeapon::secondaryFire()
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


  void BaseWeapon::secondaryFiring(unsigned int time)
  {
    if (time > 250)
      currentState_ = IDLE;
  }


  bool BaseWeapon::tick(unsigned long time, Real deltaTime)
  {
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
          leftAmmo_ += ammoDump_->getAmmunition(magazineSize_ - leftAmmo_);
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


  int BaseWeapon::getAmmoState()
  {
    return leftAmmo_;
  }
}
}
