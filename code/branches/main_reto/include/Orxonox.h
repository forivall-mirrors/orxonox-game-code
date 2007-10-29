#ifndef __Orxonox__
#define __Orxonox__

#include "Ogre.h"
#include "OgreTimer.h"

#include "OgreControl.h"
#include "RunManager.h"

class Orxonox
{
public:
	virtual void go(void);

protected:
	OgreControl *mOgre;
	RunManager  *mRunMgr;
	Timer       *mTimer;

	virtual bool setup(void);

	virtual void destroy(void);

};

#endif