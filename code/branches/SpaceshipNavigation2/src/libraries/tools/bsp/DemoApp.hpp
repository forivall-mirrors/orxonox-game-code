//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_DEMO_HPP
#define OGRE_DEMO_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "OgreFramework.hpp"

// for loading/saving the stereo config file
#include <io.h>
#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <sys/stat.h>

//|||||||||||||||||||||||||||||||||||||||||||||||

class DemoApp : public OIS::KeyListener
{
public:
	DemoApp();
	~DemoApp();

	void startDemo();
	
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

private:
	int setupDemoScene();
	int runDemo_Default(); // no gpu buffer flushing code
	int runDemo_MethodA(); // my HOQ gpu buffer flushing code
	int runDemo_MethodB(); // ogre HOQ gpu buffer flushing code
	int runDemo_MethodC(); // my blit gpu buffer flushing code

	UINT m_uLastTime ;
	void ResetSectionTimer() ;
	UINT GetSectionTimer() ;


	void StereoConfig_Write( void ) ;
	bool StereoConfig_Read(void) ;

	void BZNConfig_Write(void) ;
	bool BZNConfig_Read(void) ;

	

	Ogre::GpuCommandBufferFlush mBufferFlush;

	bool						m_bShutdown;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif 

//|||||||||||||||||||||||||||||||||||||||||||||||
