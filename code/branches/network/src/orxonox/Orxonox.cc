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
 @file  Orxonox.cc
 @brief Orxonox Main Class
 */

// Precompiled Headers
#include "OrxonoxStableHeaders.h"

//****** OGRE ******
#include <OgreException.h>
#include <OgreRoot.h>
#include <OgreFrameListener.h>
#include <OgreRenderWindow.h>
#include <OgreTextureManager.h>
#include <OgreResourceGroupManager.h>
#include <OgreConfigFile.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreTimer.h>
#include <OgreWindowEventUtilities.h>

//****** OIS *******
#include <OIS/OIS.h>

//****** STD *******
#include <iostream>
#include <exception>

//***** ORXONOX ****
//misc
#include "util/Sleep.h"

// audio
#include "audio/AudioManager.h"

// network
#include "network/Server.h"
#include "network/Client.h"
#include "network/NetworkPrereqs.h"
network::Client *client_g;
network::Server *server_g;


// objects
#include "tools/Timer.h"
#include "tools/OrxListener.h"
#include "core/ArgReader.h"
#include "core/Debug.h"
#include "core/Factory.h"
#include "core/Loader.h"
#include "core/Tickable.h"
#include "hud/HUD.h"
#include "objects/weapon/BulletManager.h"
#include "GraphicsEngine.h"

#include "Orxonox.h"

namespace orxonox
{
  /// init static singleton reference of Orxonox
  Orxonox* Orxonox::singletonRef_ = NULL;

  /**
   * create a new instance of Orxonox
   */
  Orxonox::Orxonox()
  {
    this->ogre_ = new GraphicsEngine();
    this->dataPath_ = "";
    this->auMan_ = 0;
    this->singletonRef_ = 0;
    this->keyboard_ = 0;
    this->mouse_ = 0;
    this->inputManager_ = 0;
    this->frameListener_ = 0;
    this->root_ = 0;
    // turn frame smoothing on by setting a value different from 0
    this->frameSmoothingTime_ = 0.0f;
    this->bAbort_ = false;
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
   * @param argv list of argumenst
   * @param path path to config (in home dir or something)
   */
  void Orxonox::init(int argc, char **argv, std::string path)
  {
    //TODO: find config file (assuming executable directory)
    //TODO: read config file
    //TODO: give config file to Ogre
    std::string mode;
    
    
    ArgReader ar = ArgReader(argc, argv);
    ar.checkArgument("mode", mode, false);
    ar.checkArgument("data", this->dataPath_, false);
    ar.checkArgument("ip", serverIp_, false);
    if(ar.errorHandling()) die();
    if(mode == std::string("client"))
    {
      mode_ = CLIENT;
      clientInit(path);
    }
    else if(mode== std::string("server")){
      mode_ = SERVER;
      serverInit(path);
    }
    else{
      mode_ = STANDALONE;
      standaloneInit(path);
    }
  }

  
  /**
   * start modules
   */
  void Orxonox::start()
  {
    switch(mode_){
    case CLIENT:
      clientStart();
      break;
    case SERVER:
      serverStart();
      break;
    default:
      standaloneStart();
    }
  }
  
  void Orxonox::clientStart(){
    ogre_->startRender();
    Factory::createClassHierarchy();
    
    
    auMan_ = new audio::AudioManager();

    bulletMgr_ = new BulletManager();
    
    Ogre::Overlay* hudOverlay = Ogre::OverlayManager::getSingleton().getByName("Orxonox/HUD1.2");
    HUD* orxonoxHud;
    orxonoxHud = new HUD();
    orxonoxHud->setEnergyValue(20);
    orxonoxHud->setEnergyDistr(20,20,60);
    hudOverlay->show();
    
    client_g->establishConnection();
    client_g->tick(0);
    
    
    //setupInputSystem();
    
    createFrameListener();
    
    startRenderLoop();
  }
  
  void Orxonox::serverStart(){
    //TODO: start modules
    ogre_->startRender();
    //TODO: run engine
    Factory::createClassHierarchy();
    createScene();
    setupScene();
    setupInputSystem();
    
    createFrameListener();
    server_g->open();
    
    startRenderLoop();
  }
  
  void Orxonox::standaloneStart(){
    //TODO: start modules
    ogre_->startRender();
    //TODO: run engine
    Factory::createClassHierarchy();
    createScene();
    setupScene();
    setupInputSystem();
    
    createFrameListener();
    
    startRenderLoop();
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
    delete this;
  }

  /**
    Asks the mainloop nicely to abort.
  */
  void Orxonox::abortRequest()
  {
    bAbort_ = true;
  }


  void Orxonox::serverInit(std::string path)
  {
    COUT(2) << "initialising server" << std::endl;
    
    ogre_->setConfigPath(path);
    ogre_->setup();
    root_ = ogre_->getRoot();
    if(!ogre_->load()) die(/* unable to load */);
    
    server_g = new network::Server();
  }

  void Orxonox::clientInit(std::string path)
  {
    COUT(2) << "initialising client" << std::endl;\
    
    ogre_->setConfigPath(path);
    ogre_->setup();
    if(serverIp_.compare("")==0)
      client_g = new network::Client();
    else
      client_g = new network::Client(serverIp_, NETWORK_PORT);
    if(!ogre_->load()) die(/* unable to load */);
  }
  
  void Orxonox::standaloneInit(std::string path)
  {
    COUT(2) << "initialising standalone mode" << std::endl;
    
    ogre_->setConfigPath(path);
    ogre_->setup();
    root_ = ogre_->getRoot();
    if(!ogre_->load()) die(/* unable to load */);
  }

  void Orxonox::defineResources()
  {
    std::string secName, typeName, archName;
    Ogre::ConfigFile cf;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    cf.load(macBundlePath() + "/Contents/Resources/resources.cfg");
#else
    cf.load(dataPath_ + "resources.cfg");
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
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation( std::string(macBundlePath() + "/" + archName), typeName, secName);
#else
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation( archName, typeName, secName);
#endif
      }
    }
  }

  void Orxonox::setupRenderSystem()
  {
    if (!root_->restoreConfig() && !root_->showConfigDialog())
      throw Ogre::Exception(52, "User canceled the config dialog!", "OrxApplication::setupRenderSystem()");
  }

  void Orxonox::createRenderWindow()
  {
    root_->initialise(true, "OrxonoxV2");
  }

  void Orxonox::initializeResourceGroups()
  {
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  }

  void Orxonox::createScene(void)
  {
	// Init audio
    auMan_ = new audio::AudioManager();

    bulletMgr_ = new BulletManager();

    // load this file from config
//    loader_ = new loader::LevelLoader("sample.oxw");
//    loader_->loadLevel();
    Level* startlevel = new Level("levels/sample.oxw");
    Loader::open(startlevel);

    Ogre::Overlay* hudOverlay = Ogre::OverlayManager::getSingleton().getByName("Orxonox/HUD1.2");
    //HUD* orxonoxHud;
    orxonoxHUD_ = new HUD();
    orxonoxHUD_->setEnergyValue(20);
    orxonoxHUD_->setEnergyDistr(20,20,60);
    hudOverlay->show();

	/*
    auMan_->ambientAdd("a1");
    auMan_->ambientAdd("a2");
    auMan_->ambientAdd("a3");
                                //auMan->ambientAdd("ambient1");
    auMan_->ambientStart();*/
  }


  void Orxonox::setupScene()
  {
//    SceneManager *mgr = ogre_->getSceneManager();


//    SceneNode* node = (SceneNode*)mgr->getRootSceneNode()->getChild("OgreHeadNode");
//     SceneNode *node = mgr->getRootSceneNode()->createChildSceneNode("OgreHeadNode", Vector3(0,0,0));


/*
    particle::ParticleInterface *e = new particle::ParticleInterface(mgr,"engine","Orxonox/strahl");
    e->particleSystem_->setParameter("local_space","true");
    e->setPositionOfEmitter(0, Vector3(0,-10,0));
    e->setDirection(Vector3(0,0,-1));
    e->addToSceneNode(node);
*/
  }


  void Orxonox::setupInputSystem()
  {
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    OIS::ParamList pl;

    // fixes auto repeat problem
    #if defined OIS_LINUX_PLATFORM
      pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
    #endif

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

  // FIXME we actually want to do this differently...
  void Orxonox::createFrameListener()
  {
    frameListener_ = new OrxListener(keyboard_, auMan_, mode_);
  }

  void Orxonox::startRenderLoop()
  {
    // FIXME
    // this is a hack!!!
    // the call to reset the mouse clipping size should probably be somewhere
    // else, however this works for the moment.
    unsigned int width, height, depth;
    int left, top;
    ogre_->getRoot()->getAutoCreatedWindow()->getMetrics(width, height, depth, left, top);

    if(mode_!=CLIENT){
      const OIS::MouseState &ms = mouse_->getMouseState();
      ms.width = width;
      ms.height = height;
    }
    mainLoop();
  }

  /**
    Main loop of the orxonox game.
    This is a new solution, using the ogre engine instead of beeing used by it.
    An alternative solution would be to simply use the timer of the Root object,
    but that implies using Ogre in any case. There would be no way to test
    our code without the help of the root object.
    There's even a chance that we can dispose of the root object entirely
    in server mode.
    About the loop: The design is almost exactly like the one in ogre, so that
    if any part of ogre registers a framelisteners, it will still behave
    correctly. Furthermore I have taken over the time smoothing feature from
    ogre. If turned on (see orxonox constructor), it will calculate the dt_n by
    means of the recent most dt_n-1, dt_n-2, etc.
  */
  void Orxonox::mainLoop()
  {
    // use the ogre timer class to measure time.
    Ogre::Timer *timer = new Ogre::Timer();
    timer->reset();

    // Contains the times of recently fired events
    std::deque<unsigned long> eventTimes[3];
    // Clear event times
    for (int i = 0; i < 3; ++i)
      eventTimes[i].clear();

	  while (!bAbort_)
	  {
		  // Pump messages in all registered RenderWindows
      Ogre::WindowEventUtilities::messagePump();

      // get current time
      unsigned long now = timer->getMilliseconds();

      // create an event to pass to the frameStarted method in ogre
      Ogre::FrameEvent evt;
      evt.timeSinceLastEvent = calculateEventTime(now, eventTimes[0]);
      evt.timeSinceLastFrame = calculateEventTime(now, eventTimes[1]);

      // show the current time in the HUD
      orxonoxHUD_->setTime((int)now, 0);

      // don't forget to call _fireFrameStarted in ogre to make sure
      // everything goes smoothly
      if (!ogre_->getRoot()->_fireFrameStarted(evt))
        break;

      // Iterate through all Tickables and call their tick(dt) function
      for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; )
        (it++)->tick((float)evt.timeSinceLastFrame);

      if (mode_ != SERVER)
      {
        // only render in non-server mode
        ogre_->getRoot()->_updateAllRenderTargets();
      }

      // get current time
      now = timer->getMilliseconds();

      // create an event to pass to the frameEnded method in ogre
      evt.timeSinceLastEvent = calculateEventTime(now, eventTimes[0]);
      evt.timeSinceLastFrame = calculateEventTime(now, eventTimes[2]);

      // again, just to be sure ogre works fine
      if (!ogre_->getRoot()->_fireFrameEnded(evt))
        break;
	  }
  }

  /**
    Method for calculating the average time between recently fired events.
    Code directly taken from OgreRoot.cc
    @param now The current time in ms.
    @param type The type of event to be considered.
  */
  float Orxonox::calculateEventTime(unsigned long now, std::deque<unsigned long> &times)
  {
    // Calculate the average time passed between events of the given type
    // during the last mFrameSmoothingTime seconds.

    times.push_back(now);

    if(times.size() == 1)
      return 0;

    // Times up to mFrameSmoothingTime seconds old should be kept
    unsigned long discardThreshold =
      static_cast<unsigned long>(frameSmoothingTime_ * 1000.0f);

    // Find the oldest time to keep
    std::deque<unsigned long>::iterator it = times.begin(),
      end = times.end()-2; // We need at least two times
    while(it != end)
    {
      if (now - *it > discardThreshold)
        ++it;
      else
        break;
    }

    // Remove old times
    times.erase(times.begin(), it);

    return (float)(times.back() - times.front()) / ((times.size()-1) * 1000);
  }
}
