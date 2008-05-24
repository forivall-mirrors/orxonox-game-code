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
#include "util/ExprParser.h"

// core
#include "core/ConfigFileManager.h"
#include "core/ConsoleCommand.h"
#include "core/Debug.h"
#include "core/Factory.h"
#include "core/Loader.h"
#include "core/Tickable.h"
#include "core/InputBuffer.h"
#include "core/InputManager.h"
#include "core/TclBind.h"

// audio
#include "audio/AudioManager.h"

// network
#include "network/Server.h"
#include "network/Client.h"

// objects and tools
#include "tools/Timer.h"
#include "hud/HUD.h"
#include "console/InGameConsole.h"

// FIXME: is this really file scope?
// globals for the server or client
network::Client *client_g;
network::Server *server_g;

namespace orxonox
{
  ConsoleCommandShortcut(Orxonox, exit, AccessLevel::None).setKeybindMode(KeybindMode::OnPress);
  ConsoleCommandShortcut(Orxonox, slomo, AccessLevel::Offline).setDefaultValue(0, 1.0)
    .setAxisParamIndex(0).setIsAxisRelative(false);
  ConsoleCommandShortcut(Orxonox, setTimeFactor, AccessLevel::Offline).setDefaultValue(0, 1.0);
  ConsoleCommandShortcut(Orxonox, activateConsole, AccessLevel::None);
  class Testconsole : public InputBufferListener
  {
    public:
      Testconsole(InputBuffer* ib) : ib_(ib) {}
      void listen() const
      {
        std::cout << "> " << this->ib_->get() << std::endl;
      }
      void execute() const
      {
        std::cout << ">> " << this->ib_->get() << std::endl;
        if (!CommandExecutor::execute(this->ib_->get()))
          std::cout << "Error" << std::endl;
        this->ib_->clear();
      }
      void hintandcomplete() const
      {
        std::cout << CommandExecutor::hint(this->ib_->get()) << std::endl;
        this->ib_->set(CommandExecutor::complete(this->ib_->get()));
      }
      void clear() const
      {
        this->ib_->clear();
      }
      void removeLast() const
      {
        this->ib_->removeLast();
      }
      void exit() const
      {
        InputManager::setInputState(InputManager::IS_NORMAL);
      }

    private:
      InputBuffer* ib_;
  };

  class Calculator
  {
  public:
    static float calculate(const std::string& calculation)
    {
      ExprParser expr(calculation);
      if (expr.getSuccess())
      {
        if (expr.getResult() == 42.0)
          std::cout << "Greetings from the restaurant at the end of the universe." << std::endl;
        // FIXME: insert modifier to display in full precision
        std::cout << "Result is: " << expr.getResult() << std::endl;
        if (expr.getRemains() != "")
          std::cout << "Warning: Expression could not be parsed to the end! Remains: '"
              << expr.getRemains() << "'" << std::endl;
        return expr.getResult();
      }
      else
      {
        std::cout << "Cannot calculate expression: Parse error" << std::endl;
        return 0;
      }
    }
  };
  ConsoleCommandShortcut(Calculator, calculate, AccessLevel::None);

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
    orxonoxConsole_(0),
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

    TclBind::getInstance().setDataPath(dataPath);
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
    //if (mode == DEDICATED)
    // do something else
    //else

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

    if (!loadPlayground())
      return false;

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
    ogre_->createNewScene();

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

    COUT(3) << "Orxonox: Loading Console..." << std::endl;
    InputBuffer* ib = dynamic_cast<InputBuffer*>(InputManager::getKeyHandler("buffer"));
    /*
    Testconsole* console = new Testconsole(ib);
    ib->registerListener(console, &Testconsole::listen, true);
    ib->registerListener(console, &Testconsole::execute, '\r', false);
    ib->registerListener(console, &Testconsole::hintandcomplete, '\t', true);
    ib->registerListener(console, &Testconsole::clear, '�', true);
    ib->registerListener(console, &Testconsole::removeLast, '\b', true);
    ib->registerListener(console, &Testconsole::exit, (char)0x1B, true);
    */
    orxonoxConsole_ = new InGameConsole(ib);
    ib->registerListener(orxonoxConsole_, &InGameConsole::listen, true);
    ib->registerListener(orxonoxConsole_, &InGameConsole::execute, '\r', false);
    ib->registerListener(orxonoxConsole_, &InGameConsole::hintandcomplete, '\t', true);
    //ib->registerListener(orxonoxConsole_, &InGameConsole::clear, '§', true);
    ib->registerListener(orxonoxConsole_, &InGameConsole::removeLast, '\b', true);
    ib->registerListener(orxonoxConsole_, &InGameConsole::exit, (char)0x1B, true);

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

    client_g->establishConnection();
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

    COUT(3) << "Orxonox: Starting the main loop." << std::endl;
	  while (!bAbort_)
	  {
		  // Pump messages in all registered RenderWindows
      // This calls the WindowEventListener objects.
      Ogre::WindowEventUtilities::messagePump();

      // get current time
      unsigned long now = timer_->getMilliseconds();

      // create an event to pass to the frameStarted method in ogre
      Ogre::FrameEvent evt;
      evt.timeSinceLastEvent = calculateEventTime(now, eventTimes[0]);
      evt.timeSinceLastFrame = calculateEventTime(now, eventTimes[1]);

      // show the current time in the HUD
      // HUD::getSingleton().setTime(now);

      // Call those objects that need the real time
      for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; ++it)
        it->tick((float)evt.timeSinceLastFrame * this->timefactor_);
      // Call the scene objects
      for (Iterator<TickableReal> it = ObjectList<TickableReal>::start(); it; ++it)
        it->tick((float)evt.timeSinceLastFrame);
      // TODO: currently a hack. Somehow the console doesn't work with OrxonoxClass
      orxonoxConsole_->tick((float)evt.timeSinceLastFrame);

      // don't forget to call _fireFrameStarted in ogre to make sure
      // everything goes smoothly
      ogreRoot._fireFrameStarted(evt);

      ogreRoot._updateAllRenderTargets(); // only render in non-server mode

      // get current time
      now = timer_->getMilliseconds();

      // create an event to pass to the frameEnded method in ogre
      evt.timeSinceLastEvent = calculateEventTime(now, eventTimes[0]);
      evt.timeSinceLastFrame = calculateEventTime(now, eventTimes[2]);

      // again, just to be sure ogre works fine
      ogreRoot._fireFrameEnded(evt);
	  }

    if (mode_==CLIENT)
      network::Client::getSingleton()->closeConnection();
    else if (mode_==SERVER)
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

  /**
   * Static function that shows the console in game mode.
   */
  void Orxonox::activateConsole()
  {
    // currently, the console shows itself when feeded with input.
    InputManager::setInputState(InputManager::IS_CONSOLE);
  }
}
