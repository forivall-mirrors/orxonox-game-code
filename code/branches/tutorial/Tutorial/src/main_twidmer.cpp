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
            lightNode->translate(Vector3(0, -10 * evt.timeSinceLastFrame, 0));
            
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
            mSceneMgr->setAmbientLight( ColourValue( 0.3, 0.3, 0.3 ) );
            Entity* head = mSceneMgr->createEntity("head", "ogrehead.mesh");

	Entity* head2 = mSceneMgr->createEntity("head2", "ogrehead.mesh");

            SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "OgreHeadNode", Vector3( 0, 0, 0 ) );
            node->attachObject( head );

            SceneNode *node2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "OgreHeadNode2", Vector3( 60, 0, 0 ) );
            node2->attachObject( head2 );


            mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");

            Light *light = mSceneMgr->createLight("Light1");
            light->setType(Light::LT_POINT);
            light->setPosition(Vector3(0, 100, 0));
            light->setDiffuseColour(0.5, 0.5, 0.0);
            light->setSpecularColour(0.5, 0.5, 0.0);

            BillboardSet *bbs = mSceneMgr->createBillboardSet("bb", 1);
            bbs->createBillboard(Vector3::ZERO, ColourValue(1.0, 0.0, 0.0));
            bbs->setMaterialName("Examples/Flare");

            lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("LightNode", Vector3(0, 100, 0));
            lightNode->attachObject(bbs);
            lightNode->attachObject(light);
            light->setPosition(0.0, 0.0, 0.0);
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
