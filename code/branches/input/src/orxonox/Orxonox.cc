/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
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
 @file
 @brief Orxonox Main Class
 */

// Precompiled Headers
#include "OrxonoxStableHeaders.h"
#include "Orxonox.h"

//****** STD *******
#include <deque>

//****** OGRE ******
#include <OgreFrameListener.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreTimer.h>
#include <OgreWindowEventUtilities.h>

//***** ORXONOX ****
// util
//#include "util/Sleep.h"
#include "util/ArgReader.h"

// core
#include "core/ConfigFileManager.h"
#include "core/ConsoleCommand.h"
#include "core/Debug.h"
#include "core/Loader.h"
#include "core/Tickable.h"
#include "core/InputManager.h"
#include "core/TclBind.h"

// audio
#include "audio/AudioManager.h"

// network
#include "network/Server.h"
#include "network/Client.h"

// objects and tools
#include "hud/HUD.h"
#include <Ogre.h>

#include "GraphicsEngine.h"

// FIXME: is this really file scope?
// globals for the server or client
network::Client *client_g;
network::Server *server_g;

namespace orxonox
{
  SetConsoleCommandShortcut(Orxonox, exit).setKeybindMode(KeybindMode::OnPress);
  SetConsoleCommandShortcut(Orxonox, slomo).setAccessLevel(AccessLevel::Offline).setDefaultValue(0, 1.0).setAxisParamIndex(0).setIsAxisRelative(false);
  SetConsoleCommandShortcut(Orxonox, setTimeFactor).setAccessLevel(AccessLevel::Offline).setDefaultValue(0, 1.0);

  /**
    @brief Reference to the only instance of the class.
  */
  Orxonox *Orxonox::singletonRef_s = 0;

  /**
   * Create a new instance of Orxonox. Avoid doing any actual work here.
   */
  Orxonox::Orxonox() :
    ogre_(0),
    //auMan_(0),
    timer_(0),
    // turn on frame smoothing by setting a value different from 0
    frameSmoothingTime_(0.0f),
    orxonoxHUD_(0),
    bAbort_(false),
    timefactor_(1.0f),
    mode_(STANDALONE),
    serverIp_(""),
    serverPort_(NETWORK_PORT)
  {
  }

  /**
   * Destruct Orxonox.
   */
  Orxonox::~Orxonox()
  {
    // keep in mind: the order of deletion is very important!
//    if (this->orxonoxHUD_)
//      delete this->orxonoxHUD_;
    Loader::close();
    InputManager::destroy();
    //if (this->auMan_)
    //  delete this->auMan_;
    if (this->timer_)
      delete this->timer_;
    GraphicsEngine::getSingleton().destroy();

    if (network::Client::getSingleton())
      network::Client::destroySingleton();
    if (server_g)
      delete server_g;
  }


  /**
    Asks the mainloop nicely to abort.
  */
  void Orxonox::abortRequest()
  {
    COUT(3) << "Orxonox: Abort requested." << std::endl;
    bAbort_ = true;
  }

  /**
   * @return singleton reference
   */
  Orxonox* Orxonox::getSingleton()
  {
    if (!singletonRef_s)
      singletonRef_s = new Orxonox();
    return singletonRef_s;
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
  bool Orxonox::init(int argc, char **argv, std::string path)
  {
    //TODO: find config file (assuming executable directory)
    //TODO: read config file
    //TODO: give config file to Ogre
    std::string mode;
    std::string dataPath;

    ArgReader ar(argc, argv);
    ar.checkArgument("mode", mode, false);
    ar.checkArgument("data", dataPath, false);
    ar.checkArgument("ip", serverIp_, false);
    ar.checkArgument("port", serverPort_, false);
    if(ar.errorHandling())
      return false;

    if (mode == "client")
      mode_ = CLIENT;
    else if (mode == "server")
      mode_ = SERVER;
    else if (mode == "dedicated")
      mode_ = DEDICATED;
    else
    {
      mode = "standalone";
      mode_ = STANDALONE;
    }
    COUT(3) << "Orxonox: Mode is " << mode << "." << std::endl;

    //if (mode_ == DEDICATED)
      // TODO: decide what to do here
    //else

    // for playable server, client and standalone, the startup
    // procedure until the GUI is identical

    ConfigFileManager::getSingleton()->setFile(CFT_Settings, "orxonox.ini");
    Factory::createClassHierarchy();

    ogre_ = &GraphicsEngine::getSingleton();
    if (!ogre_->setup(path))       // creates ogre root and other essentials
      return false;

    return true;
  }

  /**
   * start modules
   */
  bool Orxonox::start()
  {
    if (mode_ == DEDICATED)
    {
      // do something else
    }
    else
    { // not dedicated server
      if (!ogre_->loadRenderer())    // creates the render window
        return false;

      // Calls the InputManager which sets up the input devices.
      // The render window width and height are used to set up the mouse movement.
      if (!InputManager::initialise(ogre_->getWindowHandle(),
            ogre_->getWindowWidth(), ogre_->getWindowHeight(), true, true, true))
        return false;

      // TODO: Spread this so that this call only initialises things needed for the GUI
      if (!ogre_->initialiseResources())
        return false;

      // TOOD: load the GUI here
      // set InputManager to GUI mode
      InputManager::setInputState(InputManager::IS_GUI);
      // TODO: run GUI here

      // The following lines depend very much on the GUI output, so they're probably misplaced here..

      InputManager::setInputState(InputManager::IS_NONE);

      // create Ogre SceneManager
      ogre_->createNewScene();

      if (!loadPlayground())
        return false;
    }

    switch (mode_)
    {
    case SERVER:
      if (!serverLoad())
        return false;
      break;
    case CLIENT:
      if (!clientLoad())
        return false;
      break;
    case DEDICATED:
      if (!serverLoad())
        return false;
      break;
    default:
      if (!standaloneLoad())
        return false;
    }

    InputManager::setInputState(InputManager::IS_NORMAL);

    return startRenderLoop();
  }

  /**
   * Loads everything in the scene except for the actual objects.
   * This includes HUD, Console..
   */
  bool Orxonox::loadPlayground()
  {
    // Init audio
    //auMan_ = new audio::AudioManager();
    //auMan_->ambientAdd("a1");
    //auMan_->ambientAdd("a2");
    //auMan_->ambientAdd("a3");
    //auMan->ambientAdd("ambient1");
    //auMan_->ambientStart();

    // Load the HUD
    COUT(3) << "Orxonox: Loading HUD..." << std::endl;
    orxonoxHUD_ = &HUD::getSingleton();
    return true;
  }

  /**
   * Level loading method for server mode.
   */
  bool Orxonox::serverLoad()
  {
    COUT(2) << "Loading level in server mode" << std::endl;

    server_g = new network::Server(serverPort_);

    if (!loadScene())
      return false;

    server_g->open();

    return true;
  }

  /**
   * Level loading method for client mode.
   */
  bool Orxonox::clientLoad()
  {
    COUT(2) << "Loading level in client mode" << std::endl;\

    if (serverIp_.compare("") == 0)
      client_g = network::Client::createSingleton();
    else

      client_g = network::Client::createSingleton(serverIp_, serverPort_);

    if(!client_g->establishConnection())
      return false;
    client_g->tick(0);

    return true;
  }

  /**
   * Level loading method for standalone mode.
   */
  bool Orxonox::standaloneLoad()
  {
    COUT(2) << "Loading level in standalone mode" << std::endl;

    if (!loadScene())
      return false;

    return true;
  }

  /**
   * Helper method to load a level.
   */
  bool Orxonox::loadScene()
  {
    Level* startlevel = new Level("levels/sample.oxw");
    Loader::open(startlevel);
    
    // HACK: shader stuff for presentation
    /*Ogre::SceneManager* mSceneMgr = GraphicsEngine::getSingleton().getSceneManager();
    mSceneMgr->setAmbientLight(ColourValue(0.4,0.4,0.4));
    Ogre::Light* dirlight = mSceneMgr->createLight("Light1");

    dirlight->setType(Ogre::Light::LT_DIRECTIONAL);
    dirlight->setDirection(Vector3( 0, 1, 5 ));
    dirlight->setDiffuseColour(ColourValue(0.6, 0.6, 0.4));
    dirlight->setSpecularColour(ColourValue(1.0, 1.0, 1.0));*/
    
    return true;
  }


  /**
    Main loop of the orxonox game.
    About the loop: The design is almost exactly like the one in ogre, so that
    if any part of ogre registers a framelisteners, it will still behave
    correctly. Furthermore the time smoothing feature from ogre has been
    implemented too. If turned on (see orxonox constructor), it will calculate
    the dt_n by means of the recent most dt_n-1, dt_n-2, etc.
  */
  bool Orxonox::startRenderLoop()
  {
    // first check whether ogre root object has been created
    if (Ogre::Root::getSingletonPtr() == 0)
    {
      COUT(2) << "Orxonox Error: Could not start rendering. No Ogre root object found" << std::endl;
      return false;
    }
    Ogre::Root& ogreRoot = Ogre::Root::getSingleton();


    // Contains the times of recently fired events
    // eventTimes[4] is the list for the times required for the fps counter
    std::deque<unsigned long> eventTimes[3];
    // Clear event times
    for (int i = 0; i < 3; ++i)
      eventTimes[i].clear();

    // use the ogre timer class to measure time.
    if (!timer_)
      timer_ = new Ogre::Timer();
    timer_->reset();

    float renderTime = 0.0f;
    float frameTime = 0.0f;
    clock_t time = 0;

    //Ogre::SceneManager* mSceneMgr = GraphicsEngine::getSingleton().getSceneManager();
    //Ogre::Viewport* mViewport = mSceneMgr->getCurrentViewport();
    
    //Ogre::CompositorManager::getSingleton().addCompositor(mViewport, "Bloom");
    //Ogre::CompositorManager::getSingleton().addCompositor(mViewport, "MotionBlur");

    COUT(3) << "Orxonox: Starting the main loop." << std::endl;
    while (!bAbort_)
    {
      // get current time
      unsigned long now = timer_->getMilliseconds();

      // create an event to pass to the frameStarted method in ogre
      Ogre::FrameEvent evt;
      evt.timeSinceLastEvent = calculateEventTime(now, eventTimes[0]);
      evt.timeSinceLastFrame = calculateEventTime(now, eventTimes[1]);
      frameTime += evt.timeSinceLastFrame;

      // show the current time in the HUD
      // HUD::getSingleton().setTime(now);
      if (mode_ != DEDICATED && frameTime > 0.4f)
      {
        HUD::getSingleton().setRenderTimeRatio(renderTime / frameTime);
        frameTime = 0.0f;
        renderTime = 0.0f;
      }

      // Call those objects that need the real time
      for (Iterator<TickableReal> it = ObjectList<TickableReal>::start(); it; ++it)
        it->tick((float)evt.timeSinceLastFrame);
      // Call the scene objects
      for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; ++it)
        it->tick((float)evt.timeSinceLastFrame * this->timefactor_);

      // don't forget to call _fireFrameStarted in ogre to make sure
      // everything goes smoothly
      ogreRoot._fireFrameStarted(evt);

      // get current time
      now = timer_->getMilliseconds();
      calculateEventTime(now, eventTimes[2]);

      if (mode_ != DEDICATED)
      {
        // Pump messages in all registered RenderWindows
        // This calls the WindowEventListener objects.
        Ogre::WindowEventUtilities::messagePump();

        // render
        ogreRoot._updateAllRenderTargets();
      }

      // get current time
      now = timer_->getMilliseconds();

      // create an event to pass to the frameEnded method in ogre
      evt.timeSinceLastEvent = calculateEventTime(now, eventTimes[0]);
      renderTime += calculateEventTime(now, eventTimes[2]);

      // again, just to be sure ogre works fine
      ogreRoot._fireFrameEnded(evt);
      //msleep(200);
    }

    if (mode_ == CLIENT)
      network::Client::getSingleton()->closeConnection();
    else if (mode_ == SERVER)
      server_g->close();

    return true;
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
    // during the last frameSmoothingTime_ seconds.

    times.push_back(now);

    if(times.size() == 1)
      return 0;

    // Times up to frameSmoothingTime_ seconds old should be kept
    unsigned long discardThreshold = (unsigned long)(frameSmoothingTime_ * 1000.0f);

    // Find the oldest time to keep
    std::deque<unsigned long>::iterator it  = times.begin();
    // We need at least two times
    std::deque<unsigned long>::iterator end = times.end() - 2;

    while(it != end)
    {
      if (now - *it > discardThreshold)
        ++it;
      else
        break;
    }

    // Remove old times
    times.erase(times.begin(), it);

    return (float)(times.back() - times.front()) / ((times.size() - 1) * 1000);
  }
}
