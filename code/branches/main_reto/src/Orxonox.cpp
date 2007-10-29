#include "Orxonox.h"


void Orxonox::go(void)
{
	if (!setup())
		return;

	mTimer = new Timer();

	unsigned long lastTime = mTimer->getMilliseconds();

	while (true)
	{
		//Pump messages in all registered RenderWindow windows
		WindowEventUtilities::messagePump();

		mOgre->getRoot()->renderOneFrame();

		if (!mRunMgr->tick(mTimer->getMilliseconds(), (mTimer->getMilliseconds() - lastTime) / 1000.0))
			break;
		lastTime = mTimer->getMilliseconds();
	}

	// clean up
	destroy();
}

bool Orxonox::setup(void)
{
	// create new 3D ogre render engine
	mOgre = new OgreControl();
	mOgre->initialise();

	mRunMgr = new RunManager(mOgre);

	return true;
}


void Orxonox::destroy()
{
	if (mTimer)
		delete mTimer;
	if (mRunMgr)
		delete mRunMgr;
	if (mOgre)
		delete mOgre;
}