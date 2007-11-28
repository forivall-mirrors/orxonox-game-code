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
#include "OgreQuaternion.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreSceneManager.h"
#include "OgreParticleSystem.h"

#include "inertial_node.h"
#include "run_manager.h"
#include "weapon/bullet.h"
#include "weapon/bullet_manager.h"
#include "weapon/weapon_station.h"
#include "weapon/base_weapon.h"
#include "weapon/barrel_gun.h"
#include "weapon/ammunition_dump.h"

#include "orxonox_ship.h"


namespace orxonox {
  using namespace Ogre;
  using namespace weapon;

  /**
  * Base class for any kind of flyable ship in Orxonox.
  * 
  * The ship offers steering methods (like left, right, etc.) and translates
  * them into movement. A ship can also hold more than one weapons (where each
  * of these can be replaced during the game). This means that a ship can have
  * many WeaponManagers but only one MunitionManager (independant object that
  * is referenced in each WeaponManager).
  * Furthermore a ship in Orxonox is responsible for its visualization, which is
  * why it receives a pointer to the SceneManager.
  */


  /**
  * Standard constructor, that only initalizes a few variables. Some of them
  * could be made static, since any new ship would be derived from the BaseShip.
  * Or even better: write config files for each ship so that manipulating
  * its properties would be even easier.
  * @param mSceneMgr The current main SceneManager
  * @param mNode The scene node which the ship will be attached to later.
  */
  OrxonoxShip::OrxonoxShip(SceneNode *node)
    : sceneMgr_(RunManager::getSingletonPtr()->getSceneManagerPtr()),
      bulletManager_(RunManager::getSingletonPtr()->getBulletManagerPtr()),
      baseThrust_(1000), currentThrust_(Vector3::ZERO), objectCounter_(0)
  {
    rootNode_ = new InertialNode(node, Vector3::ZERO);
  }


  /**
  * Standard destructor.
  * Doesn't have any work to do yet.
  */
  OrxonoxShip::~OrxonoxShip()
  {
    if (mainWeapon_)
      delete mainWeapon_;
    if (railGunStation_)
      delete railGunStation_;
    if (rootNode_)
      delete rootNode_;
  }


  /**
  * Initialises everything.
  * Once that ResourceGroups are organised, this method loads them.
  * It might be an idea to make this function static in order for the
  * SceneManger to call the initialise method of every needed class (macros..)
  * @return Returns false when failed.
  */
  bool OrxonoxShip::initialise()
  {
	  // load all the resources needed (no resource groups yet,
    // so the allInit is not executed!)
	  // ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	  // create the "space ship" (currently a fish..)
	  // TODO: names must be unique! use static variables..
	  shipEntity_ = sceneMgr_->createEntity("Ship", "fish.mesh");
	  InertialNode *fishNode = rootNode_->createChildNode();
    fishNode->getSceneNode()->yaw(Degree(-90));
	  fishNode->getSceneNode()->attachObject(shipEntity_);
	  fishNode->getSceneNode()->setScale(Vector3(10, 10, 10));

    // initialise weapon(s)
    ammoDump_ = new AmmunitionDump();
    ammoDump_->setDumpSize("Barrel", 1000);
    ammoDump_->store("Barrel", 420);

    InertialNode *mainWeaponNode = rootNode_->createChildNode();
    mainWeapon_ = new BarrelGun(mainWeaponNode, ammoDump_);

    railGunStation_ = new WeaponStation(4);
    railGunStation_->addWeapon(mainWeapon_);
    railGunStation_->selectWeapon(0);

    // create some nice effects

    ParticleSystem *particles = RunManager::getSingletonPtr()
      ->getSceneManagerPtr()->createParticleSystem("asdf", "Examples/Smoke");

    fishNode->getSceneNode()->attachObject(particles);




	  return true;
  }


  /**
  * Gets the ship to accelerate in the current direction.
  * The value should be between 0 and 1, with one beeing full thrust and 0 none
  * @param value Acceleration between 0 and 1
  */
  void OrxonoxShip::setMainThrust(const Real value)
  {
    currentThrust_.z = value * baseThrust_;
  }


  /**
  * Gets the ship to accelerate sideways regarding the current direction.
  * The value should be between 0 and 1, with one beeing full thrust and 0 none
  * @param value Acceleration between 0 and 1
  */
  void OrxonoxShip::setSideThrust(const Real value)
  {
    currentThrust_.x = value * baseThrust_;
  }


  /**
  * Gets the ship to accelerate up and down.
  * The value should be between 0 and 1, with one beeing full thrust and 0 none
  * @param value Acceleration between 0 and 1
  */
  void OrxonoxShip::setYThrust(const Real value)
  {
    currentThrust_.y = value * baseThrust_;
  }


  /**
  * Rotate the ship along with the camera up and down.
  * @param angle Pitch value.
  */
  void OrxonoxShip::turnUpAndDown(const Radian &angle)
  {
    rootNode_->getSceneNode()->pitch(-angle, Node::TS_LOCAL);
  }


  /**
  * Rotate the ship along with the camera left and right.
  * @param angle Yaw value.
  */
  void OrxonoxShip::turnLeftAndRight(const Radian &angle)
  {
    rootNode_->getSceneNode()->yaw(-angle, Node::TS_PARENT);
  }


  /**
  * Returns the current speed of the ship according to its parent node.
  * @return The current speed.
  */
  Vector3 OrxonoxShip::getSpeed()
  {
    return rootNode_->getSpeed();
  }

  /**
  * Returns the ship's root SceneNode.
  * @return The Root Node.
  */
  InertialNode* OrxonoxShip::getRootNode()
  {
    return rootNode_;
  }


  /**
  * Fire a bullet (Entity with SceneNode).
  * This method creates a new Entity plus a SceneNode. But be sure not make
  * the new Node a child of RootNode_!
  * @return Bullet containing speed and entity.
  */
  BaseWeapon* OrxonoxShip::getMainWeapon()
  {
    return mainWeapon_;
  }


  int OrxonoxShip::getAmmoStock()
  {
    return ammoDump_->getStockSize("Barrel");
  }


  /**
  * Standard tick() function.
  * Currently, only the speed is applied according to the thrust values.
  * @param time Absolute time.
  * @param deltaTime Relative time.
  * @return Return true to continue render
  */
  bool OrxonoxShip::tick(unsigned long time, Real deltaTime)
  {
    mainWeapon_->tick(time, deltaTime);

    Quaternion quad = rootNode_->getSceneNode()->getOrientation();
    quad.normalise();
    rootNode_->addSpeed(quad * (Vector3(-1, -1, -1) * currentThrust_) * deltaTime);

    rootNode_->getSceneNode()->translate(rootNode_->getSpeed() * deltaTime);

	  return true;
  }

}
