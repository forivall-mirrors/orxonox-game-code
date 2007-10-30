#ifndef __OrxonoxShip__
#define __OrxonoxShip__

#include "Ogre.h"

using namespace Ogre;

class OrxonoxShip
{
public:
	OrxonoxShip(SceneManager*, SceneNode*);
	virtual ~OrxonoxShip();

	/*void setThrust(float);
	void setSideThrust(float);*/

	virtual bool initialise();

protected:
	SceneManager *mSceneMgr;
	SceneNode *mRootNode;
	Entity *mShip;
};


#endif
