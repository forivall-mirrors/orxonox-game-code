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
//#include <OgreException.h>
#include <OgreFrameListener.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreTimer.h>
#include <OgreWindowEventUtilities.h>

//****** STD *******
//#include <iostream>
//#include <exception>
#include <deque>

//***** ORXONOX ****
//misc
//#include "util/Sleep.h"
#include "util/ArgReader.h"

// audio
#include "audio/AudioManager.h"

// network
#include "network/Server.h"
#include "network/Client.h"
network::Client *client_g;
network::Server *server_g;

// objects
#include "core/Debug.h"
#include "core/Factory.h"
#include "core/Loader.h"
#include "core/Tickable.h"
#include "hud/HUD.h"
#include "tools/Timer.h"
#include "objects/weapon/BulletManager.h"

#include "core/InputManager.h"

#include "Orxonox.h"

namespace orxonox
{
  /**
    @brief Reference to the only instance of the class.
  */
  Orxonox *Orxonox::singletonRef_s = 0;

  /**
   * create a new instance of Orxonox
   */
  Orxonox::Orxonox()
  {
    this->ogre_ = new GraphicsEngine();
    this->timer_ = 0;
    this->dataPath_ = "";
    this->auMan_ = 0;
    this->inputHandler_ = 0;
    //this->root_ = 0;
    // turn on frame smoothing by setting a value different from 0
    this->frameSmoothingTime_ = 0.0f;
    this->bAbort_ = false;
  }

  /**
   * destruct Orxonox
   */
  Orxonox::~Orxonox()
  {
    // keep in mind: the order of deletion is very important!
    if (this->bulletMgr_)
      delete this->bulletMgr_;
    if (this->orxonoxHUD_)
      delete this->orxonoxHUD_;
    Loader::close();
    InputManager::destroySingleton();
    if (this->auMan_)
      delete this->auMan_;
    if (this->timer_)
      delete this->timer_;
    if (this->ogre_)
      delete this->ogre_;

    if (client_g)
      delete client_g;
    if (server_g)
      delete server_g;
  }

  /**
   * error kills orxonox
   */
  void Orxonox::abortImmediate(/* some error code */)
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

  /**
   * @return singleton object
   */
  Orxonox* Orxonox::getSingleton()
  {
    if (!singletonRef_s)
      singletonRef_s = new Orxonox();
    return singletonRef_s;
    //static Orxonox theOnlyInstance;
    //return &theOnlyInstance;
  }

  /**
    @brief Destroys the Orxonox singleton.
  */
  void Orxonox::destroySingleton()
  {
    if (singletonRef_s)
      delete singletonRef_s;
    singletonRef_s = 0;
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

    ArgReader ar(argc, argv);
    ar.checkArgument("mode", mode, false);
    ar.checkArgument("data", this->dataPath_, false);
    ar.checkArgument("ip", serverIp_, false);
    if(ar.errorHandling()) abortImmediate();
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

  void Orxonox::serverInit(std::string path)
  {
    COUT(2) << "initialising server" << std::endl;
    
    ogre_->setConfigPath(path);
    ogre_->setup();
    //root_ = ogre_->getRoot();
    if(!ogre_->load(this->dataPath_)) abortImmediate(/* unable to load */);
    
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
    if(!ogre_->load(this->dataPath_)) abortImmediate(/* unable to load */);
  }
  
  void Orxonox::standaloneInit(std::string path)
  {
    COUT(2) << "initialising standalone mode" << std::endl;
    
    ogre_->setConfigPath(path);
    ogre_->setup();
    //root_ = ogre_->getRoot();
    if(!ogre_->load(this->dataPath_)) abortImmediate(/* unable to load */);
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
    ogre_->initialise();
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
    
    startRenderLoop();
  }
  
  void Orxonox::serverStart(){
    //TODO: start modules
    ogre_->initialise();
    //TODO: run engine
    Factory::createClassHierarchy();
    createScene();
    setupInputSystem();
    
    server_g->open();
    
    startRenderLoop();
  }
  
  void Orxonox::standaloneStart(){
    //TODO: start modules
    ogre_->initialise();
    //TODO: run engine
    Factory::createClassHierarchy();
    createScene();
    setupInputSystem();
    
    startRenderLoop();
  }

  void Orxonox::createScene(void)
  {
	  // Init audio
    auMan_ = new audio::AudioManager();

    bulletMgr_ = new BulletManager();

    // load this file from config
    Level* startlevel = new Level("levels/sample.oxw");
    Loader::open(startlevel);

    Ogre::Overlay* hudOverlay = Ogre::OverlayManager::getSingleton().getByName("Orxonox/HUD1.2");
    orxonoxHUD_ = new HUD();
    orxonoxHUD_->setEnergyValue(20);
    orxonoxHUD_->setEnergyDistr(20,20,60);
    hudOverlay->show();

	/*
    auMan_->ambientAdd("a1");
    auMan_->ambientAdd("a2");
    auMan_->ambientAdd("a3");
    //auMan->ambientAdd("ambient1");
    auMan_->ambientStart();
  */
  }

  /**
    @brief Calls the InputHandler which sets up the input devices.
    The render window width and height are used to set up the mouse movement.
  */
  void Orxonox::setupInputSystem()
  {
    inputHandler_ = InputManager::getSingleton();
    if (!inputHandler_->initialise(ogre_->getWindowHandle(),
          ogre_->getWindowWidth(), ogre_->getWindowHeight()))
      abortImmediate();
  }

  /**
    Main loop of the orxonox game.
    This is a new solution, using the ogre engine instead of being used by it.
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
  void Orxonox::startRenderLoop()
  {
    // use the ogre timer class to measure time.
    if (!timer_)
      timer_ = new Ogre::Timer();
    timer_->reset();

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
      unsigned long now = timer_->getMilliseconds();

      // create an event to pass to the frameStarted method in ogre
      Ogre::FrameEvent evt;
      evt.timeSinceLastEvent = calculateEventTime(now, eventTimes[0]);
      evt.timeSinceLastFrame = calculateEventTime(now, eventTimes[1]);

      // show the current time in the HUD
      orxonoxHUD_->setTime((int)now, 0);

      // Iterate through all Tickables and call their tick(dt) function
      for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; )
        (it++)->tick((float)evt.timeSinceLastFrame);

      // don't forget to call _fireFrameStarted in ogre to make sure
      // everything goes smoothly
      ogre_->frameStarted(evt);

      if (mode_ != SERVER)
        ogre_->renderOneFrame(); // only render in non-server mode

      // get current time
      now = timer_->getMilliseconds();

      // create an event to pass to the frameEnded method in ogre
      evt.timeSinceLastEvent = calculateEventTime(now, eventTimes[0]);
      evt.timeSinceLastFrame = calculateEventTime(now, eventTimes[2]);

      // again, just to be sure ogre works fine
      ogre_->frameEnded(evt);
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

  /**
    @brief Test method for the InputHandler.
    But: Is actually responsible for catching an exit event..
  */
  void Orxonox::eventOccured(InputEvent &evt)
  {
    if (evt.id == 1)
      this->abortRequest();
  }
}
