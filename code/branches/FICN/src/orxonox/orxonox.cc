/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

/**
 @file  orxonox.cc
 @brief Orxonox Main Class
 */

#include "orxonox.h"
#include "graphicsEngine.h"

//#include <Ogre.h>
// 40% speed up: (instead of Ogre.h)
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreRoot.h>
#include <OgreFrameListener.h>
#include <OgreConfigFile.h>
#include <OgreTextureManager.h>
#include <OgreEntity.h>
#include <OgreRenderWindow.h>

#include <OIS/OIS.h>
//#include <CEGUI/CEGUI.h>
//#include <OgreCEGUIRenderer.h>

#include <string>
#include <iostream>

#include "objects/Tickable.h"
#include "objects/Timer.h"
#include "core/Factory.h"

#include "../loader/LevelLoader.h"
#include "../audio/AudioManager.h"

#include "spaceship_steering.h"


//network stuff
#include "../network/Server.h"
#include "../network/Client.h"
#include "../network/NetworkFrameListener.h"

// some tests to see if enet works without includsion
//#include <enet/enet.h>
//#include <enet/protocol.h>


namespace orxonox
{
  using namespace Ogre;

   // put this in seperate Class or solve the problem in another fashion
  class OrxListener : public FrameListener, public OIS::MouseListener
  {
    public:
      OrxListener(OIS::Keyboard *keyboard, OIS::Mouse *mouse, audio::AudioManager*  auMan)
      : mKeyboard(keyboard), mMouse(mouse)
      {
      	
      	SpaceshipSteering* steering = orxonox::Orxonox::getSingleton()->getSteeringPointer();
      	
        speed = 250;
        loop = 100;
        rotate = 10;
        mouseX = 0;
        mouseY = 0;
        maxMouseX = 0;
        minMouseX = 0;
        moved = false;
        steering->brakeRotate(rotate*10);
        steering->brakeLoop(loop);
        mMouse->setEventCallback(this);
        auMan_ = auMan;
      }
      bool frameStarted(const FrameEvent& evt)
      {

        auMan_->update();

      	SpaceshipSteering* steering = orxonox::Orxonox::getSingleton()->getSteeringPointer();

        mKeyboard->capture();
        mMouse->capture();
        if (mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W))
          steering->moveForward(speed);
        else
          steering->moveForward(0);
        if(mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S))
          steering->brakeForward(speed);
        else
          steering->brakeForward(speed/10);
        if (mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D))
          steering->loopRight(loop);
        else
          steering->loopRight(0);
        if (mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A))
          steering->loopLeft(loop);
        else
          steering->loopLeft(0);

        if(moved) {
          if (mouseY<=0)
            steering->rotateUp(-mouseY*rotate);
          if (mouseY>0)
            steering->rotateDown(mouseY*rotate);
          if (mouseX>0)
            steering->rotateRight(mouseX*rotate);
          if (mouseX<=0)
            steering->rotateLeft(-mouseX*rotate);
          mouseY = 0;
          mouseX = 0;
          moved = false;
        }
        else {
          steering->rotateUp(0);
          steering->rotateDown(0);
          steering->rotateRight(0);
          steering->rotateLeft(0);
        }

  		steering->tick(evt.timeSinceLastFrame);
  		
  		
  		
//      scenemanager->spacehip->tick(evt.timesincelastframe);
        if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
          cout << "maximal MouseX: " << maxMouseX << "\tminMouseX: " << minMouseX << endl;
        return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
      }

      bool mouseMoved(const OIS::MouseEvent &e)
      {
        mouseX += e.state.X.rel;
        mouseY += e.state.Y.rel;
        if(mouseX>maxMouseX) maxMouseX = mouseX;
        if(mouseX<minMouseX) minMouseX = mouseX;
        cout << "mouseX: " << mouseX << "\tmouseY: " << mouseY << endl;
        moved = true;
        return true;
      }

      bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) { return true; }
      bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) { return true; }

    private:
      float speed;
      float rotate;
      float loop;
      float mouseY;
      float mouseX;
      float maxMouseX;
      float minMouseX;
      bool moved;
      OIS::Keyboard *mKeyboard;
      OIS::Mouse *mMouse;
      audio::AudioManager*  auMan_;
  };
  // init static singleton reference of Orxonox
  Orxonox* Orxonox::singletonRef_ = NULL;

  /**
   * create a new instance of Orxonox
   */
  Orxonox::Orxonox()
  {
    ogre_ = new GraphicsEngine();
  }

  /**
   * destruct Orxonox
   */
  Orxonox::~Orxonox()
  {
    // nothing to delete as for now
  }

  /**
   * initialization of Orxonox object
   * @param argc argument counter
   * @param argv list of arguments
   * @param path path to config (in home dir or something)
   */
  void Orxonox::init(int argc, char **argv, std::string path)
  {
    //TODO: initialization with command line parameters
    //TODO: find config file (assuming executable directory)
    //TODO: read config file
    //TODO: give config file to Ogre
    if(argc >=2 && strcmp(argv[1], "server")==0)
    {
      serverInit(path);
    }
    else if(argc >=2 && strcmp(argv[1], "client")==0)
    {
      clientInit(path);
    }
    standalone(path);
  }

  /**
   * start modules
   */
  void Orxonox::start()
  {
    //TODO: start modules

    //TODO: run engine
  }

  /**
   * @return singleton object
   */
  Orxonox* Orxonox::getSingleton()
  {
    if (!singletonRef_)
      singletonRef_ = new Orxonox();
    return singletonRef_;
  }

  /**
   * error kills orxonox
   */
  void Orxonox::die(/* some error code */)
  {
    //TODO: destroy and destruct everything and print nice error msg
  }

  void Orxonox::standalone(std::string path)
  {
    ogre_->setConfigPath(path);
    ogre_->setup();
    root_ = ogre_->getRoot();
    //if(!ogre_->load()) die(/* unable to load */);

    defineResources();
    setupRenderSystem();
    createRenderWindow();
    initializeResourceGroups();
    createScene();
    setupScene();
    setupInputSystem();
    createFrameListener();
    Factory::createClassHierarchy();
    startRenderLoop();
  }

  void Orxonox::serverInit(std::string path)
  {
    ogre_->setConfigPath(path);
    ogre_->setup();
    server_g = new network::Server(); // add some settings if wanted
    if(!ogre_->load()) die(/* unable to load */);
    ogre_->getRoot()->addFrameListener(new network::ServerFrameListener());
    ogre_->startRender();

    createScene();
    setupScene();
  }

  void Orxonox::clientInit(std::string path)
  {
    ogre_->setConfigPath(path);
    ogre_->setup();
    client_g = new network::Client(); // address here
    if(!ogre_->load()) die(/* unable to load */);
    ogre_->getRoot()->addFrameListener(new network::ClientFrameListener());
    ogre_->startRender();

    createScene();
    setupScene();
    setupInputSystem();
    createFrameListener();
    startRenderLoop();
  }

  void Orxonox::defineResources()
  {
    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    cf.load(macBundlePath() + "/Contents/Resources/resources.cfg");
#else
    cf.load("resources.cfg");
#endif

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
      secName = seci.peekNextKey();
      Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
      Ogre::ConfigFile::SettingsMultiMap::iterator i;
      for (i = settings->begin(); i != settings->end(); ++i)
      {
        typeName = i->first;
        archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation( String(macBundlePath() + "/" + archName), typeName, secName);
#else
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation( archName, typeName, secName);
#endif
      }
    }
  }

  void Orxonox::setupRenderSystem()
  {
    if (!root_->restoreConfig() && !root_->showConfigDialog())
      throw Exception(52, "User canceled the config dialog!", "OrxApplication::setupRenderSystem()");
  }

  void Orxonox::createRenderWindow()
  {
    root_->initialise(true, "OrxonoxV2");
  }

  void Orxonox::initializeResourceGroups()
  {
    TextureManager::getSingleton().setDefaultNumMipmaps(5);
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  }

  void Orxonox::createScene(void)
  {
	// Init audio
    auMan_ = new audio::AudioManager();

    // load this file from config
    loader_ = new loader::LevelLoader("sample.oxw");
    loader_->loadLevel();





	/*
    auMan_->ambientAdd("a1");
    auMan_->ambientAdd("a2");
    auMan_->ambientAdd("a3");
                                //auMan->ambientAdd("ambient1");
    auMan_->ambientStart();
	*/
  }


  void Orxonox::setupScene()
  {
    SceneManager *mgr = ogre_->getSceneManager();

	/*
    Camera *cam = mgr->createCamera("Camera");
    cam->setPosition(Vector3(0,0,-250));
    cam->lookAt(Vector3(0,0,0));


    Viewport *vp = ogre_->getRoot()->getAutoCreatedWindow()->addViewport(cam);

/*
    Entity* head = mgr->createEntity("ASSF", "assf2.mesh");
    SceneNode *node = mgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode", Vector3(0,0,0));
    node->attachObject(head);
    node->attachObject(cam);



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
    */

  }


  void Orxonox::setupInputSystem()
  {
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    OIS::ParamList pl;
    Ogre::RenderWindow *win = ogre_->getRoot()->getAutoCreatedWindow();

    win->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    inputManager_ = OIS::InputManager::createInputSystem(pl);

    try
    {
      keyboard_ = static_cast<OIS::Keyboard*>(inputManager_->createInputObject(OIS::OISKeyboard, false));
      mouse_ = static_cast<OIS::Mouse*>(inputManager_->createInputObject(OIS::OISMouse, true));
    }
    catch (const OIS::Exception &e)
    {
      throw new Ogre::Exception(42, e.eText, "OrxApplication::setupInputSystem");
    }
  }

  // we actually want to do this differently...
  void Orxonox::createFrameListener()
  {
    TickFrameListener* TickFL = new TickFrameListener();
    ogre_->getRoot()->addFrameListener(TickFL);

    TimerFrameListener* TimerFL = new TimerFrameListener();
    ogre_->getRoot()->addFrameListener(TimerFL);

    frameListener_ = new OrxListener(keyboard_, mouse_, auMan_);
    ogre_->getRoot()->addFrameListener(frameListener_);
  }

  void Orxonox::startRenderLoop()
  {
    // this is a hack!!!
    // the call to reset the mouse clipping size should probably be somewhere
    // else, however this works for the moment.
		unsigned int width, height, depth;
		int left, top;
		ogre_->getRoot()->getAutoCreatedWindow()->getMetrics(width, height, depth, left, top);

		const OIS::MouseState &ms = mouse_->getMouseState();
		ms.width = width;
		ms.height = height;

    ogre_->getRoot()->startRendering();
  }
}
