#ifndef __CameraManager__
#define __CameraManager__

#include "Ogre.h"

using namespace Ogre;

class CameraManager
{
public:
	CameraManager(SceneManager*);
	~CameraManager();

	bool setCameraPosition(int);

protected:
	SceneManager *mSceneMgr;
	Camera *mCamera;
};


#endif
