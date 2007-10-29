#ifndef __OgreControl__
#define __OgreControl__

#include "Ogre.h"
#include "OgreConfigFile.h"


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
#endif

using namespace Ogre;

class OgreControl
{
public:
	OgreControl();

	virtual ~OgreControl();

	/** Sets up the application - returns false if the user chooses to abandon configuration. */
	virtual bool initialise(void);

	virtual Root* getRoot(void);

	virtual RenderWindow* getRenderWindow(void);

	virtual Ogre::String getResourcePath(void);

protected:
	Root         *mRoot;
	RenderWindow *mWindow;
	Ogre::String  mResourcePath;

	/** Configures the application - returns false if the user chooses to abandon configuration. */
	virtual bool configure(void);

	/// Method which will define the source of resources (other than current folder)
	virtual void setupResources(void);

};

#endif