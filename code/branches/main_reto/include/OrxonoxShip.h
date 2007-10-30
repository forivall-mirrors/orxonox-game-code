#ifndef __OrxonoxShip__
#define __OrxonoxShip__

#include "Ogre.h"

using namespace Ogre;

class OrxonoxShip
{
public:
	OrxonoxShip(SceneManager*, SceneNode*);
	virtual ~OrxonoxShip();

	virtual bool initialise();

	void setThrust(const Real);
	void setSideThrust(const Real);
	void setYaw(const Radian);
	void setPitch(const Radian);
	void setRoll(const Radian);

	bool tick(unsigned long, float);

protected:
	SceneManager *mSceneMgr;
	SceneNode *mRootNode;
	Entity *mShip;

	Vector3 speed;
	float thrust, sideThrust, baseThrust;
};


#endif
