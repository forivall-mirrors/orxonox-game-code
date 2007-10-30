#include "OrxonoxShip.h"


OrxonoxShip::OrxonoxShip(SceneManager *mSceneMgr, SceneNode *mNode)
	: mSceneMgr(mSceneMgr), mRootNode(mNode), speed(Vector3(0, 0, 0)), baseThrust(100)
{
}


OrxonoxShip::~OrxonoxShip()
{
}


bool OrxonoxShip::initialise()
{
	// load all the resources needed (no resource groups yet, so the allInit is not executed!)
	//ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// create the "space ship" (currently a fish..)
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
	sideThrust = value;
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


bool OrxonoxShip::tick(unsigned long time, float deltaTime)
{
	speed += (mRootNode->getLocalAxes() * Vector3(0, 0, 1)).normalisedCopy() * thrust * deltaTime;

	mRootNode->translate(speed * deltaTime);

	return true;
}
