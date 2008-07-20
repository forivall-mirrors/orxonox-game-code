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
 *      Reto Grieder
 *   Co-authors:
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
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
#include "core/input/InputManager.h"
#include "core/input/SimpleInputState.h"
#include "core/input/KeyBinder.h"
#include "core/TclBind.h"
#include "core/Core.h"

// audio
#include "audio/AudioManager.h"

// network
#include "network/Server.h"
#include "network/Client.h"

// objects and tools
#include "overlays/OverlayGroup.h"
#include "overlays/console/InGameConsole.h"
#include "objects/Tickable.h"
#include "objects/Backlight.h"
#include "tools/ParticleInterface.h"

#include "GraphicsEngine.h"
#include "Settings.h"
#include "Radar.h"

// globals for the server or client
static network::Client *client_g = 0;
static network::Server *server_g = 0;

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
  Orxonox::Orxonox()
    : ogre_(0)
    , startLevel_(0)
    , hud_(0)
    , radar_(0)
    //, auMan_(0)
    , timer_(0)
    , bAbort_(false)
    , timefactor_(1.0f)
    , mode_(STANDALONE)
    , serverIp_("")
    , serverPort_(NETWORK_PORT)
  {
  }

  /**
   * Destruct Orxonox.
   */
  Orxonox::~Orxonox()
  {
    // keep in mind: the order of deletion is very important!
    Loader::unload(startLevel_);
    if (this->startLevel_)
      delete this->startLevel_;

    Loader::unload(hud_);
    if (this->hud_)
      delete this->hud_;

    if (this->radar_)
      delete this->radar_;

    Loader::close();
    //if (this->auMan_)
    //  delete this->auMan_;
    InGameConsole::getInstance().destroy();
    if (this->timer_)
      delete this->timer_;
    dynamic_cast<SimpleInputState*>(InputManager::getState("game"))->removeAndDestroyAllHandlers();
    InputManager::destroy();
    GraphicsEngine::getSingleton().destroy();

    if (network::Client::getSingleton())
      network::Client::destroySingleton();
    if (server_g)
      delete network::Server::getSingleton();
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
    @brief Changes the speed of Orxonox
  */
  void Orxonox::setTimeFactor(float factor)
  {
    float change = factor / Orxonox::getSingleton()->getTimeFactor();
    Orxonox::getSingleton()->timefactor_ = factor;
    for (Iterator<ParticleInterface> it = ObjectList<ParticleInterface>::begin(); it; ++it)
        it->setSpeedFactor(it->getSpeedFactor() * change);

    for (Iterator<Backlight> it = ObjectList<Backlight>::begin(); it; ++it)
        it->setTimeFactor(Orxonox::getSingleton()->getTimeFactor());
  }

  /**
   * initialization of Orxonox object
   * @param argc argument counter
   * @param argv list of argumenst
   * @param path path to config (in home dir or something)
   */
  bool Orxonox::init(int argc, char **argv)
  {
#ifdef _DEBUG
    ConfigFileManager::getSingleton()->setFile(CFT_Settings, "orxonox_d.ini");
#else
    ConfigFileManager::getSingleton()->setFile(CFT_Settings, "orxonox.ini");
#endif
    Factory::createClassHierarchy();

    std::string mode;
    std::string tempDataPath;

    ArgReader ar(argc, argv);
    ar.checkArgument("mode", &mode, false);
    ar.checkArgument("data", &tempDataPath, false);
    ar.checkArgument("ip",   &serverIp_, false);
    ar.checkArgument("port", &serverPort_, false);
    if(ar.errorHandling())
    {
      COUT(1) << "Error while parsing command line arguments" << std::endl;
      COUT(1) << ar.getErrorString();
      COUT(0) << "Usage:" << std::endl << "orxonox [mode client|server|dedicated|standalone] "
        << "[--data PATH] [--ip IP] [--port PORT]" << std::endl;
      return false;
    }

    if (mode == "client")
      mode_ = CLIENT;
    else if (mode == "server")
      mode_ = SERVER;
    else if (mode == "dedicated")
      mode_ = DEDICATED;
    else
    {
      if (mode == "")
        mode = "standalone";
      if (mode != "standalone")
      {
        COUT(2) << "Warning: mode \"" << mode << "\" doesn't exist. "
          << "Defaulting to standalone" << std::endl;
        mode = "standalone";
      }
      mode_ = STANDALONE;
    }
    COUT(3) << "Orxonox: Mode is " << mode << "." << std::endl;

    if (tempDataPath != "")
    {
      if (tempDataPath[tempDataPath.size() - 1] != '/')
        tempDataPath += "/";
      Settings::tsetDataPath(tempDataPath);
    }

    // initialise TCL
    TclBind::getInstance().setDataPath(Settings::getDataPath());

    //if (mode_ == DEDICATED)
      // TODO: decide what to do here
    //else

    // for playable server, client and standalone, the startup
    // procedure until the GUI is identical

    ogre_ = &GraphicsEngine::getSingleton();
    if (!ogre_->setup())       // creates ogre root and other essentials
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

      // TODO: Spread this so that this call only initialises things needed for the Console
      if (!ogre_->initialiseResources())
        return false;

      // Load the InGameConsole
      InGameConsole::getInstance().initialise();

      // Calls the InputManager which sets up the input devices.
      // The render window width and height are used to set up the mouse movement.
      if (!InputManager::initialise(ogre_->getWindowHandle(),
            ogre_->getWindowWidth(), ogre_->getWindowHeight(), true, true, true))
        return false;
      KeyBinder* keyBinder = new KeyBinder();
      InputManager::createSimpleInputState("game", 20)->setHandler(keyBinder);

      // TOOD: load the GUI here
      // set InputManager to GUI mode
      // TODO: run GUI here

      // The following lines depend very much on the GUI output, so they're probably misplaced here..

      keyBinder->loadBindings();
      InputManager::requestEnterState("game");

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

    return startRenderLoop();
  }

  /**
   * Loads everything in the scene except for the actual objects.
   * This includes HUD, audio..
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
    COUT(3) << "Orxonox: Loading HUD" << std::endl;
    hud_ = new Level(Settings::getDataPath() + "overlay/hud.oxo");
    Loader::load(hud_);

    // Start the Radar
    this->radar_ = new Radar();

    return true;
  }

  /**
   * Level loading method for server mode.
   */
  bool Orxonox::serverLoad()
  {
    COUT(0) << "Loading level in server mode" << std::endl;

    //server_g = new network::Server(serverPort_);
    server_g = network::Server::createSingleton(serverPort_);

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
    COUT(0) << "Loading level in client mode" << std::endl;\

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
    COUT(0) << "Loading level in standalone mode" << std::endl;

    if (!loadScene())
      return false;

    return true;
  }

  /**
   * Helper method to load a level.
   */
  bool Orxonox::loadScene()
  {
    startLevel_ = new Level("levels/sample.oxw");
    Loader::open(startLevel_);

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


    // use the ogre timer class to measure time.
    if (!timer_)
      timer_ = new Ogre::Timer();

    unsigned long frameCount = 0;
    
    // TODO: this would very well fit into a configValue
    const unsigned long refreshTime = 200000;
    unsigned long refreshStartTime = 0;
    unsigned long tickTime = 0;
    unsigned long oldFrameCount = 0;

    unsigned long timeBeforeTick = 0;
    unsigned long timeBeforeTickOld = 0;
    unsigned long timeAfterTick = 0;

    COUT(3) << "Orxonox: Starting the main loop." << std::endl;

    timer_->reset();
    while (!bAbort_)
    {
      // get current time
      timeBeforeTickOld = timeBeforeTick;
      timeBeforeTick    = timer_->getMicroseconds();
      float dt = (timeBeforeTick - timeBeforeTickOld) / 1000000.0;


      // tick the core (needs real time for input and tcl thread management)
      Core::tick(dt);

      // Call those objects that need the real time
      for (Iterator<TickableReal> it = ObjectList<TickableReal>::start(); it; ++it)
        it->tick(dt);
      // Call the scene objects
      for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; ++it)
        it->tick(dt * this->timefactor_);

      // call server/client with normal dt
      if (client_g)
        client_g->tick(dt * this->timefactor_);
      if (server_g)
        server_g->tick(dt * this->timefactor_);


      // get current time once again
      timeAfterTick = timer_->getMicroseconds();

      tickTime += timeAfterTick - timeBeforeTick;
      if (timeAfterTick > refreshStartTime + refreshTime)
      {
        GraphicsEngine::getSingleton().setAverageTickTime(
            (float)tickTime * 0.001 / (frameCount - oldFrameCount));
        GraphicsEngine::getSingleton().setAverageFramesPerSecond(
            (float)(frameCount - oldFrameCount) / (timeAfterTick - refreshStartTime) * 1000000.0);
        oldFrameCount = frameCount;
        tickTime = 0;
        refreshStartTime = timeAfterTick;
      }


      // don't forget to call _fireFrameStarted in ogre to make sure
      // everything goes smoothly
      Ogre::FrameEvent evt;
      evt.timeSinceLastFrame = dt;
      evt.timeSinceLastEvent = dt; // note: same time, but shouldn't matter anyway
      ogreRoot._fireFrameStarted(evt);

      if (mode_ != DEDICATED)
      {
        // Pump messages in all registered RenderWindows
        // This calls the WindowEventListener objects.
        Ogre::WindowEventUtilities::messagePump();
        // make sure the window stays active even when not focused
        // (probably only necessary on windows)
        GraphicsEngine::getSingleton().setWindowActivity(true);

        // render
        ogreRoot._updateAllRenderTargets();
      }

      // again, just to be sure ogre works fine
      ogreRoot._fireFrameEnded(evt); // note: uses the same time as _fireFrameStarted

      ++frameCount;
    }

    if (mode_ == CLIENT)
      network::Client::getSingleton()->closeConnection();
    else if (mode_ == SERVER)
      server_g->close();

    return true;
  }
}
