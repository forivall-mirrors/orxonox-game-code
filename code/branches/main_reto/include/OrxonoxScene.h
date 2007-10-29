#ifndef __DemoScene__
#define __DemoScene__

#include "Ogre.h"

using namespace Ogre;

class OrxonoxScene
{
public:
	OrxonoxScene(SceneManager*);

	virtual ~OrxonoxScene();

	virtual bool initialise();

	virtual void tick(unsigned long, float);

private:
	SceneManager* mSceneMgr;

	//specific variables for test purposes
	Light        *mLight;
	SceneNode    *lightNode;
	BillboardSet *bbs;
	float distance, radius;

	/// method where you can perform resource group loading
	virtual void loadResources(void);

	// Define what is in the scene
	virtual void createScene(void);
};
#endif