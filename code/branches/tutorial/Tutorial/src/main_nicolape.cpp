/**
* Modified by Nicolas Perrenoud
* nicolape@ee.ethz.ch
*/


#include "ExampleApplication.h"
#include <iostream>
using namespace std;

SceneNode *lightNode;
SceneNode *lightNode2;
float t=0;

class TutorialFrameListener : public ExampleFrameListener
{
public:
    TutorialFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr) : ExampleFrameListener(win, cam, false, false)
    {
    }

    bool frameStarted(const FrameEvent &evt)
    {
				int r1=120;
				int r2=120;
				lightNode->setPosition(Vector3(r1 * cos(t),r1 * sin(t),r1 * sin(t)));				
				lightNode2->setPosition(Vector3(-r2 * cos(t),r2 * sin(t),-r2 * sin(t)));				
				t+=evt.timeSinceLastFrame*2;
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
			mSceneMgr->setAmbientLight(ColourValue(0.5,0.5,0.5));
			Entity* head1 = mSceneMgr->createEntity("head1","ogrehead.mesh");
			Entity* head2 = mSceneMgr->createEntity("head2","ogrehead.mesh");
			Entity* head3 = mSceneMgr->createEntity("head3","ogrehead.mesh");
			SceneNode* node1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode1",Vector3(0,0,0));
			SceneNode* node2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode2",Vector3(100,0,0));
			SceneNode* node3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode3",Vector3(-100,0,0));
			node1->attachObject(head1);
			node2->attachObject(head2);
			node3->attachObject(head3);
			mSceneMgr->setSkyBox(true,"Examples/SpaceSkyBox");

			Light* light1 = mSceneMgr->createLight("Light1");
			light1->setType(Light::LT_POINT);
			light1->setPosition(Vector3(0,0,0));
			light1->setDiffuseColour(1.0,0.0,0.0);
			light1->setSpecularColour(1.0,0.0,0.0);

			Light* light2= mSceneMgr->createLight("Light2");
			light2->setType(Light::LT_POINT);
			light2->setPosition(Vector3(0,0,0));
			light2->setDiffuseColour(0.0,0.0,1.0);
			light2->setSpecularColour(0.0,0.0,1.0);

			BillboardSet* bbs = mSceneMgr->createBillboardSet("bb",1);
			bbs->createBillboard(Vector3::ZERO, ColourValue(1.0,0.0,0.0));
			bbs->setMaterialName("Examples/Flare");
			lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("LightNode1",Vector3(0,100,0));
			lightNode->attachObject(bbs);
			lightNode->attachObject(light1);

			BillboardSet* bbs2 = mSceneMgr->createBillboardSet("bb2",2);
			bbs2->createBillboard(Vector3::ZERO, ColourValue(0.0,0.0,1.0));
			bbs2->setMaterialName("Examples/Flare");
			lightNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("LightNode2",Vector3(0,-100,0));
			lightNode2->attachObject(bbs2);
			lightNode2->attachObject(light2);
      
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
