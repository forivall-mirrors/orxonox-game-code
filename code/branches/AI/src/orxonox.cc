/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *   Author:
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

/**
 @file  orxonox.cc
 @brief Orxonox Main File
 */

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>
#include <OgreMath.h>

#include <string>
#include <iostream>

#include "SignalHandler.h"
#include "xml/xmlParser.h"
#include "loader/LevelLoader.h"
#include "Flocking.h"
#include "Wander.h"

// some tests to see if enet works without includsion
//#include <enet/enet.h>
//#include <enet/protocol.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>

// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
std::string macBundlePath()
{
  char path[1024];
  CFBundleRef mainBundle = CFBundleGetMainBundle();
  assert(mainBundle);

  CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
  assert(mainBundleURL);

  CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
  assert(cfStringRef);

  CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

  CFRelease(mainBundleURL);
  CFRelease(cfStringRef);

  return std::string(path);
}
#endif

using namespace Ogre;

//my-stuff
//globale definition eines Arrays welches alle nodes enthält
Vector3 ElementLocationArray[9];
Vector3 ElementSpeedArray[9];
Vector3 ElementAccelerationArray[9];

Element arrayOfElements[9];
Wander walker;
int counter = 0;
int times = 0;



class OrxExitListener : public FrameListener
{
  public:
    OrxExitListener(OIS::Keyboard *keyboard, Root* root)
  : mKeyboard(keyboard)
    {
	root_ = root;
    }

    bool frameStarted(const FrameEvent& evt)
    {
      moving(evt);
      mKeyboard->capture();
      return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
    }
  
    //code the movments of the nodes here
    void moving(const FrameEvent& evt) {
      SceneManager *mgr = root_->getSceneManager("Default SceneManager");

/*    // RUN WANDER

      walker.update();
      walker.speed = walker.speed + 10*walker.acceleration*evt.timeSinceLastFrame;
      walker.location = walker.location + 10*walker.speed*evt.timeSinceLastFrame;
      walker.acceleration  = (0,0,0);
      mgr->getSceneNode("HeadNode10")->setPosition(walker.location);

*/   // END RUN WANDER




     //  RUN FLOCKING

      arrayOfElements[8].location = 100*Vector3(Math::Cos(Math::DegreesToRadians(counter)/10),Math::Sin(Math::DegreesToRadians(counter)/10),Math::Cos(Math::DegreesToRadians(counter+(counter-180)/2)/10));

      arrayOfElements[0].update(arrayOfElements);
      arrayOfElements[1].update(arrayOfElements);
      arrayOfElements[2].update(arrayOfElements);
      arrayOfElements[3].update(arrayOfElements);
      arrayOfElements[4].update(arrayOfElements);
      arrayOfElements[5].update(arrayOfElements);
      arrayOfElements[6].update(arrayOfElements);
      arrayOfElements[7].update(arrayOfElements);
      arrayOfElements[8].update(arrayOfElements);

      for(int i=0; i<9; i++) {
        arrayOfElements[i].speed = 0.995*arrayOfElements[i].speed + arrayOfElements[i].acceleration*evt.timeSinceLastFrame;
        arrayOfElements[i].location = arrayOfElements[i].location + arrayOfElements[i].speed*evt.timeSinceLastFrame;
        arrayOfElements[i].acceleration  = (0,0,0);
      }

      mgr->getSceneNode("HeadNode1")->setPosition(arrayOfElements[0].location);
      mgr->getSceneNode("HeadNode2")->setPosition(arrayOfElements[1].location);
      mgr->getSceneNode("HeadNode3")->setPosition(arrayOfElements[2].location);
      mgr->getSceneNode("HeadNode4")->setPosition(arrayOfElements[3].location);
      mgr->getSceneNode("HeadNode5")->setPosition(arrayOfElements[4].location);
      mgr->getSceneNode("HeadNode6")->setPosition(arrayOfElements[5].location);
      mgr->getSceneNode("HeadNode7")->setPosition(arrayOfElements[6].location);
      mgr->getSceneNode("HeadNode8")->setPosition(arrayOfElements[7].location);
      mgr->getSceneNode("HeadNode9")->setPosition(arrayOfElements[8].location);

      counter = counter + 1;
      counter = counter%7200;

      // END RUN FLOCKING 

    }

  private:
    OIS::Keyboard *mKeyboard;
    Root* root_;
};

class OrxApplication
{
  public:
    void go()
    {
      createRoot();
      defineResources();
      setupRenderSystem();
      createRenderWindow();
      initializeResourceGroups();
      createScene();
      setupScene();
      setupInputSystem();
      setupCEGUI();
      createFrameListener();
      startRenderLoop();
    }

    ~OrxApplication()
    {
      mInputManager->destroyInputObject(mKeyboard);
      OIS::InputManager::destroyInputSystem(mInputManager);

//       delete mRenderer;
//       delete mSystem;

      delete mListener;
      delete mRoot;
    }

  private:
    Root *mRoot;
    OIS::Keyboard *mKeyboard;
    OIS::Mouse *mMouse;
    OIS::InputManager *mInputManager;
    CEGUI::OgreCEGUIRenderer *mRenderer;
    CEGUI::System *mSystem;
    OrxExitListener *mListener;

    void createRoot()
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
      mRoot = new Root(macBundlePath() + "/Contents/Resources/plugins.cfg");
#else
      mRoot = new Root();
#endif
    }

    void defineResources()
    {
      String secName, typeName, archName;
      ConfigFile cf;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
      cf.load(macBundlePath() + "/Contents/Resources/resources.cfg");
#else
      cf.load("resources.cfg");
#endif

      ConfigFile::SectionIterator seci = cf.getSectionIterator();
      while (seci.hasMoreElements())
      {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
          typeName = i->first;
          archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
          ResourceGroupManager::getSingleton().addResourceLocation( String(macBundlePath() + "/" + archName), typeName, secName);
#else
          ResourceGroupManager::getSingleton().addResourceLocation( archName, typeName, secName);
#endif
        }
      }
    }

    void setupRenderSystem()
    {
      if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
        throw Exception(52, "User canceled the config dialog!", "OrxApplication::setupRenderSystem()");
    }

    void createRenderWindow()
    {
      mRoot->initialise(true, "Ogre Render Window");
    }

    void initializeResourceGroups()
    {
      TextureManager::getSingleton().setDefaultNumMipmaps(5);
      ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }

    void createScene(void)
    {

      string levelFile = "sp_level_moonstation.oxw";
      loader::LevelLoader* loader = new loader::LevelLoader(levelFile);
    }
    
    void setupScene()
    {
      SceneManager *mgr = mRoot->createSceneManager(ST_GENERIC, "Default SceneManager");
      Camera *cam = mgr->createCamera("Camera");
      cam->setPosition(Vector3(0,0,1000));
      cam->lookAt(Vector3(0,0,0));
      Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(cam);

      //Invoke example to test AI
      example();
    }

    void setupInputSystem()
    {
      size_t windowHnd = 0;
      std::ostringstream windowHndStr;
      OIS::ParamList pl;
      RenderWindow *win = mRoot->getAutoCreatedWindow();

      win->getCustomAttribute("WINDOW", &windowHnd);
      windowHndStr << windowHnd;
      pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
      mInputManager = OIS::InputManager::createInputSystem(pl);

      try
      {
        mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));
        mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, false));
      }
      catch (const OIS::Exception &e)
      {
        throw new Exception(42, e.eText, "OrxApplication::setupInputSystem");
      }
    }

    void setupCEGUI()
    {
      SceneManager *mgr = mRoot->getSceneManager("Default SceneManager");
      RenderWindow *win = mRoot->getAutoCreatedWindow();

      // CEGUI setup
//       mRenderer = new CEGUI::OgreCEGUIRenderer(win, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mgr);
//       mSystem = new CEGUI::System(mRenderer);

      // Other CEGUI setup here.
    }

    void createFrameListener()
    {
      mListener = new OrxExitListener(mKeyboard, mRoot);
      mRoot->addFrameListener(mListener);
    }

    void startRenderLoop()
    {
      mRoot->startRendering();
    }

    void example() {
    SceneManager *mgr = mRoot->getSceneManager("Default SceneManager");
    mgr->setAmbientLight(ColourValue(1.0,1.0,1.0));

/*  //TEST DATA WANDER

    Entity* ent10 = mgr->createEntity("Head10", "ogrehead.mesh");
    SceneNode *node10 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode10", Vector3(0,0,0));
    node10->attachObject(ent10);
    Vector3 temp;
    temp = (0,0,0);
    walker.setValues(node10->getPosition(),temp,temp,true);

*/  //END TEST DATA WANDER


//   TEST DATA FLOCKING

    Entity* ent1 = mgr->createEntity("Head1", "ogrehead.mesh");
    Entity* ent2 = mgr->createEntity("Head2", "ogrehead.mesh");
    Entity* ent3 = mgr->createEntity("Head3", "ogrehead.mesh");
    Entity* ent4 = mgr->createEntity("Head4", "ogrehead.mesh");
    Entity* ent5 = mgr->createEntity("Head5", "ogrehead.mesh");
    Entity* ent6 = mgr->createEntity("Head6", "ogrehead.mesh");
    Entity* ent7 = mgr->createEntity("Head7", "ogrehead.mesh");
    Entity* ent8 = mgr->createEntity("Head8", "ogrehead.mesh");
    Entity* ent9 = mgr->createEntity("Head9", "ogrehead.mesh");

    SceneNode *node1 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode1", Vector3(100,300,100));
    SceneNode *node2 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode2", Vector3(300,0,200));
    SceneNode *node3 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode3", Vector3(-300,0,-100));
    SceneNode *node4 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode4", Vector3(-100,-300,150));
    SceneNode *node5 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode5", Vector3(150,150,-100));
    SceneNode *node6 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode6", Vector3(150,-150,-100));
    SceneNode *node7 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode7", Vector3(-150,-150,0));
    SceneNode *node8 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode8", Vector3(-150,150,0));
    SceneNode *node9 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode9", Vector3(0,0,0));

    node1->attachObject(ent1);
    node2->attachObject(ent2);
    node3->attachObject(ent3);
    node4->attachObject(ent4);
    node5->attachObject(ent5);
    node6->attachObject(ent6);
    node7->attachObject(ent7);
    node8->attachObject(ent8);
    node9->attachObject(ent9);

    ElementLocationArray[0] = node1->getPosition();
    ElementLocationArray[1] = node2->getPosition();
    ElementLocationArray[2] = node3->getPosition();
    ElementLocationArray[3] = node4->getPosition();
    ElementLocationArray[4] = node5->getPosition();
    ElementLocationArray[5] = node6->getPosition();
    ElementLocationArray[6] = node7->getPosition();
    ElementLocationArray[7] = node8->getPosition();
    ElementLocationArray[8] = node9->getPosition();

    ElementSpeedArray[0] = (0,0,0);
    ElementSpeedArray[1] = (0,0,0);
    ElementSpeedArray[2] = (0,0,0);
    ElementSpeedArray[3] = (0,0,0);
    ElementSpeedArray[4] = (0,0,0);
    ElementSpeedArray[5] = (0,0,0);
    ElementSpeedArray[6] = (0,0,0);
    ElementSpeedArray[7] = (0,0,0);
    ElementSpeedArray[8] = (0,0,0);

    ElementAccelerationArray[0] = (0,0,0);
    ElementAccelerationArray[1] = (0,0,0);
    ElementAccelerationArray[2] = (0,0,0);
    ElementAccelerationArray[3] = (0,0,0);
    ElementAccelerationArray[4] = (0,0,0);
    ElementAccelerationArray[5] = (0,0,0);
    ElementAccelerationArray[6] = (0,0,0);
    ElementAccelerationArray[7] = (0,0,0);
    ElementAccelerationArray[8] = (0,0,0);

    arrayOfElements[0].setValues( ElementLocationArray[0], ElementSpeedArray[0], ElementAccelerationArray[0], true);
    arrayOfElements[1].setValues( ElementLocationArray[1], ElementSpeedArray[1], ElementAccelerationArray[1], true);
    arrayOfElements[2].setValues( ElementLocationArray[2], ElementSpeedArray[2], ElementAccelerationArray[2], true);
    arrayOfElements[3].setValues( ElementLocationArray[3], ElementSpeedArray[3], ElementAccelerationArray[3], true);
    arrayOfElements[4].setValues( ElementLocationArray[4], ElementSpeedArray[4], ElementAccelerationArray[4], true);
    arrayOfElements[5].setValues( ElementLocationArray[5], ElementSpeedArray[5], ElementAccelerationArray[5], true);
    arrayOfElements[6].setValues( ElementLocationArray[6], ElementSpeedArray[6], ElementAccelerationArray[6], true);
    arrayOfElements[7].setValues( ElementLocationArray[7], ElementSpeedArray[7], ElementAccelerationArray[7], true);
    arrayOfElements[8].setValues( ElementLocationArray[8], ElementSpeedArray[8], ElementAccelerationArray[8], false);

   // END TEST DATA FLOCKING


    }
};



#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

             INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
             int main(int argc, char **argv)
#endif
{
  try
  {
    SignalHandler::getInstance()->doCatch(argv[0], "orxonox.log");
    OrxApplication orxonox;
    orxonox.go();
  }
  catch(Exception& e)
  {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
    fprintf(stderr, "An exception has occurred: %s\n",
            e.getFullDescription().c_str());
#endif
  }

  return 0;
}

