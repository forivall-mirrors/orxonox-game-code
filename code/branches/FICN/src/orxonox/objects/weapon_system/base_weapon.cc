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

#include "../../Orxonox.h"

#include "bullet.h"
#include "bullet_manager.h"
#include "ammunition_dump.h"

#include "base_weapon.h"


namespace orxonox {
  using namespace Ogre;
  
  CreateFactory(BaseWeapon);

  float BaseWeapon::nextActionValidityPeriod_ = 0.5;

  BaseWeapon::BaseWeapon()
    : sceneMgr_(Orxonox::getSingleton()->getSceneManager()),
      bulletCounter_(0), primaryFireRequest_(false), currentState_(IDLE),
      secondaryFireRequest_(false),
      bulletManager_(Orxonox::getSingleton()->getBulletMgr()),
      secondaryFired_(false),
      timeSinceNextActionAdded_(0), actionAdded_(false), nextAction_(NOTHING),
      ammoDump_(NULL), totalTime_(0.0), leftAmmo_(0)
  {
    RegisterObject(BaseWeapon);
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


  void BaseWeapon::secondaryFireRequest()
  {
    secondaryFireRequest_ = true;
  }


  void BaseWeapon::tick(float dt)
  {
    totalTime_ += dt;
    // process action adder
    if (actionAdded_)
    {
      timeSinceNextActionAdded_ = totalTime_;
      actionAdded_ = false;
    }

    if (currentState_ != IDLE)
    {
      switch (currentState_)
      {
      case PRIMARY_FIRE:
        primaryFiring(totalTime_ - actionStartTime_);
        break;

      case SECONDARY_FIRE:
        secondaryFiring(totalTime_ - actionStartTime_);
        break;

      case RELOADING:
        break;

      case CHANGING_AMMO:
        break;
      }
    }

    if (currentState_ == IDLE)
    {
      // first, process next action
      if (nextAction_ != NOTHING)
      {
        actionStartTime_ = totalTime_;
        switch (nextAction_)
        {
        case RELOAD:
          leftAmmo_ += ammoDump_->getAmmunition("Barrel", magazineSize_ - leftAmmo_);
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
          actionStartTime_ = totalTime_;
          currentState_ = PRIMARY_FIRE;
          secondaryFired_ = false;
          primaryFire();
        }
        else if (secondaryFireRequest_)
        {
          actionStartTime_ = totalTime_;
          currentState_ = SECONDARY_FIRE;
          secondaryFired_ = true;
          secondaryFire();
        }
      }
    }

    primaryFireRequest_ = false;
    secondaryFireRequest_ = false;

    if (totalTime_ - timeSinceNextActionAdded_ > nextActionValidityPeriod_)
      nextAction_ = NOTHING;
  }


  int BaseWeapon::getAmmoState()
  {
    return leftAmmo_;
  }

  void BaseWeapon::setAmmoDump(AmmunitionDump* dump)
  {
    this->ammoDump_ = dump;
  }
}
