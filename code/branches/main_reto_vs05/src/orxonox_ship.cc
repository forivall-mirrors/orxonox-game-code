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


#include "orxonox_ship.h"


/**
* Standard constructor, that only initalizes a few variables. Some of them
* could be made static, since any new ship would be derived from the BaseShip.
* Or even better: write config files for each ship so that manipulating
* its properties would be even easier.
* @param mSceneMgr The current main SceneManager
* @param mNode The scene node which the ship will be attached to later.
*/
OrxonoxShip::OrxonoxShip(SceneManager *mSceneMgr, SceneNode *mNode)
	    : mSceneMgr(mSceneMgr), mRootNode(mNode), speed(Vector3(0, 0, 0)),
      baseThrust(1000), thrust(0), sideThrust(0), n(0),
	bulletSpeed(400)
{
}


/**
* Standard destructor.
* Doesn't have any work to do yet.
*/
OrxonoxShip::~OrxonoxShip()
{
}


/**
* Initialises everything.
* Once that ResourceGroups are organised, this method loads them.
* It might be an idea to make this function static in order for the
* SceneManger to call the initialise method of every needed class (macros..)
*/
bool OrxonoxShip::initialise()
{
	// load all the resources needed (no resource groups yet,
  // so the allInit is not executed!)
	// ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// create the "space ship" (currently a fish..)
	// TODO: names must be unique! use static variables..
	mShip = mSceneMgr->createEntity("Ship", "fish.mesh");
	SceneNode *fishNode = mRootNode->createChildSceneNode("fishNode");
	fishNode->yaw(Degree(-90));
	fishNode->attachObject(mShip);
	fishNode->setScale(Vector3(10, 10, 10));

	return true;
}


/**
* Gets the ship to accelerate in the current direction.
* The value should be between 0 and 1, with one beeing full thrust and 0 none.
* @param value Acceleration between 0 and 1
*/
void OrxonoxShip::setThrust(const Real value)
{
	thrust = value * baseThrust;
}


/**
* Gets the ship to accelerate sideways regarding the current direction.
* The value should be between 0 and 1, with one beeing full thrust and 0 none.
* @param value Acceleration between 0 and 1
*/
void OrxonoxShip::setSideThrust(const Real value)
{
	sideThrust = value * baseThrust;
}


/**
* Rotate the ship along with the camera up and down.
* @param angle Pitch value.
*/
void OrxonoxShip::turnUpAndDown(const Radian &angle)
{
  RootNode_->pitch(angle, Ogre::Node::TransformSpace::TS_LOCAL);
}


/**
* Rotate the ship along with the camera left and right.
* @param angle Yaw value.
*/
void OrxonoxShip::turnLeftAndRight(const Radian &angle)
{
  RootNode_->yaw(angle, Ogre::Node::TransformSpace::TS_PARENT);
}


/**
* Fire a bullet (Entity with SceneNode).
* This method creates a new Entity plus a SceneNode. But be sure not make
* the new Node a child of RootNode_!
*/
Bullet* OrxonoxShip::fire()
{
	// TODO: Names must be unique!
	SceneNode *temp = RootNode_->getParentSceneNode()->createChildSceneNode(
        "BulletNode" + StringConverter::toString(objectCounter_));
	temp->setOrientation(RootNode_->getOrientation());
	temp->setPosition(RootNode_->getPosition());
	temp->setScale(Vector3(1, 1, 1) * 10);
	temp->yaw(Degree(-90));
	return new Bullet(temp, mSceneMgr->createEntity("bullet"
        + StringConverter::toString(objectCounter_++), "Barrel.mesh"), speed
        + (RootNode_->getOrientation() * Vector3(0, 0, -1)).normalisedCopy()
        * bulletSpeed_);
}


/**
* Standard tick() function.
* Currently, only the speed is applied according to the thrust values.
* @param time Absolute time.
* @param deltaTime Relative time.
*/
bool OrxonoxShip::tick(unsigned long time, Real deltaTime)
{
  Quaternion quad = mRootNode->getOrientation();
  quad.normalise();
  speed += quad * Vector3(0, 0, -1) * currentThrust_ * deltaTime;
	speed += quad * Vector3(-1, 0,  0) * current_SideThrust_ * deltaTime;

	RootNode_->translate(currentSpeed_ * deltaTime);

	return true;
}
