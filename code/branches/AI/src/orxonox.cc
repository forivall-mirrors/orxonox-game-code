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

#include <string>
#include <iostream>

#include "xml/xmlParser.h"
#include "loader/LevelLoader.h"
#include "Flocking.h"

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
Vector3 ElementLocationArray[2];
Vector3 ElementSpeedArray[2];
Vector3 ElementAccelerationArray[2];

Element* arrayOfElements[2];


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
      arrayOfElements[0]->update(*arrayOfElements);
      mgr->getSceneNode("HeadNode1")->translate(0.000000001*evt.timeSinceLastFrame*arrayOfElements[0]->location);
      arrayOfElements[1]->update(*arrayOfElements);
      mgr->getSceneNode("HeadNode2")->translate(0.000000001*evt.timeSinceLastFrame*arrayOfElements[1]->location);
      arrayOfElements[2]->update(*arrayOfElements);
      mgr->getSceneNode("HeadNode3")->translate(0.000000001*evt.timeSinceLastFrame*arrayOfElements[2]->location);
      //mgr->getSceneNode("HeadNode1")->yaw((Radian)10*evt.timeSinceLastFrame);
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
      example();  //my stuff
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

    //declaration of the 3 Ogreheads
   //muss leider global sein.....
    //Element* arrayOfElements[2];

    void example() {
    SceneManager *mgr = mRoot->getSceneManager("Default SceneManager");
    mgr->setAmbientLight(ColourValue(1.0,1.0,1.0));
    Entity* ent1 = mgr->createEntity("Head1", "ogrehead.mesh");
    Entity* ent2 = mgr->createEntity("Head2", "ogrehead.mesh");
    Entity* ent3 = mgr->createEntity("Head3", "ogrehead.mesh");
    SceneNode *node1 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode1", Vector3(0,100,0));
    SceneNode *node2 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode2", Vector3(100,0,0));
    SceneNode *node3 = mgr->getRootSceneNode()->createChildSceneNode("HeadNode3", Vector3(-100,0,0));
    node1->attachObject(ent1);
    node2->attachObject(ent2);
    node3->attachObject(ent3);
    //Camera* cam  = mgr->getCamera("Camera");
    //node1->attachObject(cam);
    ElementLocationArray[0] = node1->getPosition();
    ElementLocationArray[1] = node2->getPosition();
    ElementLocationArray[2] = node3->getPosition();
    ElementSpeedArray[0] = (0,0,0);
    ElementSpeedArray[1] = (0,0,0);
    ElementSpeedArray[2] = (0,0,0);
    ElementAccelerationArray[0] = (0,0,0);
    ElementAccelerationArray[1] = (0,0,0);
    ElementAccelerationArray[2] = (0,0,0);
    arrayOfElements[0] = new Element( ElementLocationArray[0], ElementSpeedArray[0], ElementAccelerationArray[0] );
    arrayOfElements[1] = new Element( ElementLocationArray[1], ElementSpeedArray[1], ElementAccelerationArray[1] );
    arrayOfElements[2] = new Element( ElementLocationArray[2], ElementSpeedArray[2], ElementAccelerationArray[2] );




   /* for (int i=0; i<3; i++) {
      Element* arrayOfElements[i] = new Element( ElementLocationArray[i], ElementSpeedArray[i], ElementAccelerationArray[i] );
    } */
   /* for (int i=0; i<3; i++) {
    arrayOfElements[i]->update(arrayOfElements);
    }  */
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

