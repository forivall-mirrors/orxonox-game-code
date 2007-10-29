#ifndef __MyListener__
#define __MyListener__

#include "OgreControl.h"
#include "ExampleFrameListener.h"

// Declare a subclass of the ExampleFrameListener class
class MyListener : public ExampleFrameListener
{
public:
	MyListener(RenderWindow* win, Camera* cam);

	void setReceiver(OgreControl*);

	bool frameStarted(const FrameEvent& evt);

	bool frameEnded(const FrameEvent& evt);

private:
	OgreControl *myApp;
};

#endif