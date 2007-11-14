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

#include "spaceship_steering.h"
SpaceshipSteering* steering;


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

class OrxExitListener : public FrameListener
{
  public:
    OrxExitListener(OIS::Keyboard *keyboard)
  : mKeyboard(keyboard)
    {
    }

    bool frameStarted(const FrameEvent& evt)
    {
      float speed = 1;
      float rotate = 1;
      mKeyboard->capture();
      if (mKeyboard->isKeyDown(OIS::KC_SPACE))
        steering->moveForward(speed);
      else
        steering->moveForward(0);
      if(mKeyboard->isKeyDown(OIS::KC_C))
        steering->brake(speed);
      else
        steering->brake(0.1);
      if (mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W))
        steering->rotateUp(rotate);
      else
        steering->rotateUp(0);
      if (mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S))
        steering->rotateDown(rotate);
      else
        steering->rotateDown(0);
      if (mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D))
        steering->rotateRight(rotate);
      else
        steering->rotateRight(0);
      if (mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A))
        steering->rotateLeft(rotate);
      else
        steering->rotateLeft(0);
      if (mKeyboard->isKeyDown(OIS::KC_E))
        steering->loopRight(rotate);
      else
        steering->loopRight(0);
      if (mKeyboard->isKeyDown(OIS::KC_Q))
        steering->loopLeft(rotate);
      else
        steering->loopLeft(0);
      steering->tick(evt.timeSinceLastFrame);
//	scenemanager->spacehip->tick(evt.timesincelastframe);
      return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
    }

  private:
    OIS::Keyboard *mKeyboard;
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
      cam->setPosition(Vector3(0,0,-250));
      cam->lookAt(Vector3(0,0,0));
      Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(cam);


      mgr->setAmbientLight(ColourValue(1,1,1));
      Entity* head = mgr->createEntity("head", "ogrehead.mesh");
      SceneNode *node = mgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode", Vector3(0,0,0));
      node->attachObject(head);
      node->attachObject(cam);
      mgr->setSkyBox(true, "Examples/SceneSkyBox2");

      Entity* head1 = mgr->createEntity("head1", "ogrehead.mesh");
      SceneNode *node1 = mgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode1", Vector3(200,0,0));
      node1->attachObject(head1);
      Entity* head2 = mgr->createEntity("head2", "ogrehead.mesh");
      SceneNode *node2 = mgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode2", Vector3(200,400,-100));
      node2->attachObject(head2);
      Entity* head3 = mgr->createEntity("head3", "ogrehead.mesh");
      SceneNode *node3 = mgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode3", Vector3(0,400,200));
      node3->attachObject(head3);
      Entity* head4 = mgr->createEntity("head4", "ogrehead.mesh");
      SceneNode *node4 = mgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode4", Vector3(-400,-200,600));
      node4->attachObject(head4);
      Entity* head5 = mgr->createEntity("head5", "ogrehead.mesh");
      SceneNode *node5 = mgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode5", Vector3(0,0,-400));
      node5->attachObject(head5);

      steering = new SpaceshipSteering(500, 200, 200, 200);
      steering->addNode(node);

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
      mListener = new OrxExitListener(mKeyboard);
      mRoot->addFrameListener(mListener);      
    }

    void startRenderLoop()
    {
      mRoot->startRendering();
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

