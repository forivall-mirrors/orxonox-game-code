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

#include "orxonox_ship.h"


OrxonoxShip::OrxonoxShip(SceneManager *mSceneMgr, SceneNode *mNode)
	    : mSceneMgr(mSceneMgr), mRootNode(mNode), speed(Vector3(0, 0, 0)),
      baseThrust(1000), thrust(0), sideThrust(0), n(0),
	bulletSpeed(400)
{
}


OrxonoxShip::~OrxonoxShip()
{
}


bool OrxonoxShip::initialise()
{
	// load all the resources needed (no resource groups yet,
  // so the allInit is not executed!)
	// ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// create the "space ship" (currently a fish..)
	// TODO: names must be unique!
	mShip = mSceneMgr->createEntity("Ship", "fish.mesh");
	SceneNode *fishNode = mRootNode->createChildSceneNode("fishNode");
	fishNode->yaw(Degree(-90));
	fishNode->attachObject(mShip);
	fishNode->setScale(Vector3(10, 10, 10));

	return true;
}


void OrxonoxShip::setThrust(const Real value)
{
	thrust = value * baseThrust;
}

void OrxonoxShip::setSideThrust(const Real value)
{
	sideThrust = value * baseThrust;
}

void OrxonoxShip::setYaw(const Radian value)
{
	mRootNode->yaw(value);
}

void OrxonoxShip::setPitch(const Radian value)
{
	mRootNode->pitch(value);
}

void OrxonoxShip::setRoll(const Radian value)
{
	mRootNode->roll(value);
}

Real OrxonoxShip::getThrust()
{
	return thrust;
}

Bullet* OrxonoxShip::fire()
{
	// TODO: Names must be unique!
	SceneNode *temp = mRootNode->getParentSceneNode()->createChildSceneNode(
        "BulletNode" + StringConverter::toString(n));
	temp->setOrientation(mRootNode->getOrientation());
	temp->setPosition(mRootNode->getPosition());
	temp->setScale(Vector3(1, 1, 1) * 10);
	temp->yaw(Degree(-90));
	return new Bullet(temp, mSceneMgr->createEntity("bullet"
        + StringConverter::toString(n++), "Barrel.mesh"), speed
        + (mRootNode->getLocalAxes() * Vector3(0, 0, -1)).normalisedCopy()
        * bulletSpeed);
}

bool OrxonoxShip::tick(unsigned long time, Real deltaTime)
{
  Quaternion quad = mRootNode->getOrientation();
  quad.normalise();
  speed += quad * Vector3(0, 0, -1) * thrust * deltaTime;
	speed += quad * Vector3(-1, 0,  0) * sideThrust * deltaTime;

	mRootNode->translate(speed * deltaTime);

	return true;
}
