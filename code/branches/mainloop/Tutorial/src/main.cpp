#include "ExampleApplication.h"

SceneNode *lightNode;

class TutorialFrameListener : public ExampleFrameListener
{
public:
    TutorialFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr)
        : ExampleFrameListener(win, cam, false, false)
    {
    }

    bool frameStarted(const FrameEvent &evt)
    {
        // add tutorial code here:
        // ...

        return ExampleFrameListener::frameStarted(evt);
    }
private:
};


class TutorialApplication : public ExampleApplication
{
protected:
public:
    TutorialApplication()
    {
    }

    ~TutorialApplication()
    {
    }
protected:
    void createCamera(void)
    {
        // create camera
        mCamera = mSceneMgr->createCamera("PlayerCam");
        mCamera->setNearClipDistance(5);
        mCamera->setPosition(Vector3(0,10,500));
        mCamera->lookAt(Vector3(0,0,0));
    }

    void createScene(void)
    {
        // add tutorial code here:
        // ...
        mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );
        Entity *ent1 = mSceneMgr->createEntity( "Robot", "robot.mesh" );
        SceneNode *node1 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode" );
        node1->attachObject( ent1 );

        Entity *ent2 = mSceneMgr->createEntity( "Robot2", "robot.mesh" );
        SceneNode *node2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode2", Vector3( 50, 0, 0 ) );
        node2->attachObject( ent2 );


    }

    void createFrameListener(void)
    {
        // create frame listener
        mFrameListener = new TutorialFrameListener(mWindow, mCamera, mSceneMgr);
        mRoot->addFrameListener(mFrameListener);
    }
};

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
    TutorialApplication app;

    try {
        app.go();
    } catch( Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n",
                e.getFullDescription().c_str());
#endif
    }

    return 0;
}
