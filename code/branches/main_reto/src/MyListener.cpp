#include "MyListener.h"

// Declare a subclass of the ExampleFrameListener class
MyListener::MyListener(RenderWindow* win, Camera* cam) : ExampleFrameListener(win, cam)
{
}

bool MyListener::frameStarted(const FrameEvent& evt)
{
	myApp->render(evt.timeSinceLastFrame);
	return ExampleFrameListener::frameStarted(evt);        
}

bool MyListener::frameEnded(const FrameEvent& evt)
{
	return ExampleFrameListener::frameEnded(evt);        
}

void MyListener::setReceiver(OgreControl* app)
{
	myApp = app;
}