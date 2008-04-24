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
//#include <iostream>
//#include <exception>
#include <deque>

//****** OGRE ******
//#include <OgreException.h>
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

// audio
#include "audio/AudioManager.h"

// network
#include "network/Server.h"
#include "network/Client.h"

// objects and tools
#include "tools/Timer.h"
#include "hud/HUD.h"

// FIXME: is this really file scope?
// globals for the server or client
network::Client *client_g;
network::Server *server_g;

namespace orxonox
{
  ConsoleCommandShortcut(Orxonox, exit, AccessLevel::None);
  ConsoleCommandShortcut(Orxonox, slomo, AccessLevel::Offline).setDefaultValue(0, 1.0);
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
    static void calculate(const std::string& calculation)
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
      }
      else
        std::cout << "Cannot calculate expression: Parse error" << std::endl;
    }
  };
  ConsoleCommandShortcut(Calculator, calculate, AccessLevel::None);

  /**
    @brief Reference to the only instance of the class.
  */
  Orxonox *Orxonox::singletonRef_s = 0;

  /**
   * create a new instance of Orxonox
   */
  Orxonox::Orxonox()
  {
    this->ogre_ = &GraphicsEngine::getSingleton();
    this->timer_ = 0;
    this->dataPath_ = "";
    this->auMan_ = 0;
    this->inputHandler_ = 0;
    // turn on frame smoothing by setting a value different from 0
    this->frameSmoothingTime_ = 0.0f;
    this->bAbort_ = false;
    this->timefactor_ = 1.0f;
  }

  /**
   * destruct Orxonox
   */
  Orxonox::~Orxonox()
  {
    // keep in mind: the order of deletion is very important!
    if (this->orxonoxHUD_)
      delete this->orxonoxHUD_;
    Loader::close();
    InputManager::destroy();
    if (this->auMan_)
      delete this->auMan_;
    if (this->timer_)
      delete this->timer_;
    GraphicsEngine::getSingleton().destroy();

    if (client_g)
      delete client_g;
    if (server_g)
      delete server_g;
  }

  /**
    @brief Immediately deletes the orxonox object.
    Never use if you can help it while rendering!
  */
  void Orxonox::abortImmediateForce()
  {
    COUT(1) << "*** Orxonox Error: Orxonox object was unexpectedly destroyed." << std::endl;
    delete this;
  }

  /**
    Asks the mainloop nicely to abort.
  */
  void Orxonox::abortRequest()
  {
    COUT(3) << "*** Orxonox: Abort requested." << std::endl;
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
    if(ar.errorHandling()) abortImmediateForce();
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
    if(!ogre_->load(this->dataPath_)) abortImmediateForce(/* unable to load */);

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
    if(!ogre_->load(this->dataPath_)) abortImmediateForce(/* unable to load */);
  }

  void Orxonox::standaloneInit(std::string path)
  {
    COUT(2) << "initialising standalone mode" << std::endl;

    ogre_->setConfigPath(path);
    ogre_->setup();
    if(!ogre_->load(this->dataPath_)) abortImmediateForce(/* unable to load */);
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
    ConfigFileManager::getSingleton()->setFile(CFT_Settings, "orxonox.ini");
    Factory::createClassHierarchy();


    auMan_ = new audio::AudioManager();

    Ogre::Overlay* hudOverlay = Ogre::OverlayManager::getSingleton().getByName("Orxonox/HUD1.2");
    orxonoxHUD_ = new HUD();
    orxonoxHUD_->setEnergyValue(20);
    orxonoxHUD_->setEnergyDistr(20,20,60);
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
    ConfigFileManager::getSingleton()->setFile(CFT_Settings, "orxonox.ini");
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
    ConfigFileManager::getSingleton()->setFile(CFT_Settings, "orxonox.ini");
    Factory::createClassHierarchy();
    createScene();
    setupInputSystem();

    startRenderLoop();
  }

  void Orxonox::createScene(void)
  {
	  // Init audio
    auMan_ = new audio::AudioManager();

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
    if (!InputManager::initialise(ogre_->getWindowHandle(),
          ogre_->getWindowWidth(), ogre_->getWindowHeight()))
      abortImmediateForce();
    InputManager::setInputState(InputManager::IS_NORMAL);
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
    correctly. Furthermore the time smoothing feature from ogre has been
    implemented too. If turned on (see orxonox constructor), it will calculate
    the dt_n by means of the recent most dt_n-1, dt_n-2, etc.
  */
  void Orxonox::startRenderLoop()
  {
    InputBuffer* ib = dynamic_cast<InputBuffer*>(InputManager::getKeyListener("buffer"));
    console_ = new Testconsole(ib);
    ib->registerListener(console_, &Testconsole::listen, true);
    ib->registerListener(console_, &Testconsole::execute, '\r', false);
    ib->registerListener(console_, &Testconsole::execute, '\n', false);
    ib->registerListener(console_, &Testconsole::hintandcomplete, '\t', true);
    ib->registerListener(console_, &Testconsole::clear, '§', true);
    ib->registerListener(console_, &Testconsole::removeLast, '\b', true);
    ib->registerListener(console_, &Testconsole::exit, (char)0x1B, true);

    // first check whether ogre root object has been created
    if (Ogre::Root::getSingletonPtr() == 0)
    {
      COUT(2) << "*** Orxonox Error: Could not start rendering. No Ogre root object found" << std::endl;
      return;
    }
    Ogre::Root& ogreRoot = Ogre::Root::getSingleton();


    // Contains the times of recently fired events
    // eventTimes[4] is the list for the times required for the fps counter
    std::deque<unsigned long> eventTimes[4];
    // Clear event times
    for (int i = 0; i < 4; ++i)
      eventTimes[i].clear();
    // fill the fps time list with zeros
    for (int i = 0; i < 50; i++)
      eventTimes[3].push_back(0);

    // use the ogre timer class to measure time.
    if (!timer_)
      timer_ = new Ogre::Timer();
    timer_->reset();

	  while (!bAbort_)
	  {
		  // Pump messages in all registered RenderWindows
      // This calls the WindowEventListener objects.
      Ogre::WindowEventUtilities::messagePump();

      // get current time
      unsigned long now = timer_->getMilliseconds();
      eventTimes[3].push_back(now);
      eventTimes[3].erase(eventTimes[3].begin());

      // create an event to pass to the frameStarted method in ogre
      Ogre::FrameEvent evt;
      evt.timeSinceLastEvent = calculateEventTime(now, eventTimes[0]);
      evt.timeSinceLastFrame = calculateEventTime(now, eventTimes[1]);

      // show the current time in the HUD
      orxonoxHUD_->setTime((int)now, 0);
      orxonoxHUD_->setRocket2(ogreRoot.getCurrentFrameNumber());
      if (eventTimes[3].back() - eventTimes[3].front() != 0)
        orxonoxHUD_->setRocket1((int)(50000.0f/(eventTimes[3].back() - eventTimes[3].front())));

      // Iterate through all Tickables and call their tick(dt) function
      for (Iterator<Tickable> it = ObjectList<Tickable>::start(); it; ++it)
        it->tick((float)evt.timeSinceLastFrame * this->timefactor_);

      // don't forget to call _fireFrameStarted in ogre to make sure
      // everything goes smoothly
      ogreRoot._fireFrameStarted(evt);

      // server still renders at the moment
      //if (mode_ != SERVER)
      ogreRoot._updateAllRenderTargets(); // only render in non-server mode

      // get current time
      now = timer_->getMilliseconds();

      // create an event to pass to the frameEnded method in ogre
      evt.timeSinceLastEvent = calculateEventTime(now, eventTimes[0]);
      evt.timeSinceLastFrame = calculateEventTime(now, eventTimes[2]);

      // again, just to be sure ogre works fine
      ogreRoot._fireFrameEnded(evt);
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

  void Orxonox::activateConsole()
  {
    InputManager::setInputState(InputManager::IS_CONSOLE);
  }
}
