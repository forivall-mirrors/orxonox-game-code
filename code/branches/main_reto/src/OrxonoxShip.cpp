#include "OrxonoxShip.h"


OrxonoxShip::OrxonoxShip(SceneManager *mSceneMgr, SceneNode *mNode) : mSceneMgr(mSceneMgr), mRootNode(mNode)
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
	mRootNode->setScale(Vector3(10, 10, 10));
	mRootNode->attachObject(mShip);

	return true;
}
