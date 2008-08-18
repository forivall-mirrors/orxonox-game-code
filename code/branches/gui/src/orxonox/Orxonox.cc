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
#include <cassert>

//****** OGRE ******
#include <OgreFrameListener.h>
#include <OgreRoot.h>
#include <OgreTimer.h>
#include <OgreWindowEventUtilities.h>

//***** ORXONOX ****
// util
//#include "util/Sleep.h"

// core
#include "core/ConfigFileManager.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/Debug.h"
#include "core/Loader.h"
#include "core/Exception.h"
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
#include "overlays/console/InGameConsole.h"
#include "objects/Tickable.h"
#include "objects/Backlight.h"
#include "tools/ParticleInterface.h"
#include "gui/GUIManager.h"

#include "GraphicsEngine.h"
#include "Settings.h"
#include "Radar.h"

// globals for the server or client
static network::Client *client_g = 0;
static network::Server *server_g = 0;

namespace orxonox
{
  SetConsoleCommand(Orxonox, exit, true).setKeybindMode(KeybindMode::OnPress);
  SetConsoleCommand(Orxonox, slomo, true).setAccessLevel(AccessLevel::Offline).setDefaultValue(0, 1.0).setAxisParamIndex(0).setIsAxisRelative(false);
  SetConsoleCommand(Orxonox, setTimeFactor, true).setAccessLevel(AccessLevel::Offline).setDefaultValue(0, 1.0);
  SetConsoleCommand(Orxonox, loadGame, true).setAccessLevel(AccessLevel::User).setDefaultValue(0, "standalone");

  /**
    @brief Reference to the only instance of the class.
  */
  Orxonox *Orxonox::singletonRef_s = 0;

  /**
   * Create a new instance of Orxonox. Avoid doing any actual work here.
   */
  Orxonox::Orxonox()
    : startLevel_(0)
    , hud_(0)
    //, auMan_(0)
    , timer_(0)
    , bAbort_(false)
    , timefactor_(1.0f)
    , mode_(GameMode::GM_Unspecified)
    , debugRefreshTime_(0.0f)
    , graphicsEngine_(0)
    , inputManager_(0)
    , radar_(0)
    , console_(0)
    , guiManager_(0)
  {
    RegisterRootObject(Orxonox);

    //assert(singletonRef_s == 0);
    OrxAssert(singletonRef_s == 0, "asdfasdfasdfasdfblahblah");
    singletonRef_s = this;
  }

  /**
   * Destruct Orxonox.
   */
  Orxonox::~Orxonox()
  {
    // keep in mind: the order of deletion is very important!
    if (this->timer_)
      delete this->timer_;

    Loader::unload(startLevel_);
    if (this->startLevel_)
      delete this->startLevel_;

    Loader::unload(hud_);
    if (this->hud_)
      delete this->hud_;

    if (this->radar_)
      delete this->radar_;

    if (this->guiManager_)
      delete guiManager_;

    //if (this->auMan_)
    //  delete this->auMan_;

    if (this->console_)
      delete this->console_;

    if (inputManager_)
      delete inputManager_;

    if (this->graphicsEngine_)
      delete graphicsEngine_;

    if (network::Client::getSingleton())
      network::Client::destroySingleton();
    if (server_g)
      delete network::Server::getSingleton();

    // this call will delete every BaseObject!
    // But currently this will call methods of objects that exist no more
    // The only 'memory leak' is the ParticleSpawer. They would be deleted here
    // and call a sceneNode method that has already been destroy by the corresponding space ship.
    //Loader::close();

    singletonRef_s = 0;
  }

  void Orxonox::setConfigValues()
  {
    SetConfigValue(debugRefreshTime_, 0.2).description("Sets the time interval at which average fps, etc. get updated.");
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
  Orxonox& Orxonox::getInstance()
  {
    assert(singletonRef_s);
    return *singletonRef_s;
  }

  /**
    @brief Changes the speed of Orxonox
  */
  void Orxonox::setTimeFactor(float factor)
  {
    float change = factor / Orxonox::getInstance().getTimeFactor();
    Orxonox::getInstance().timefactor_ = factor;
    for (Iterator<ParticleInterface> it = ObjectList<ParticleInterface>::begin(); it; ++it)
        it->setSpeedFactor(it->getSpeedFactor() * change);

    for (Iterator<Backlight> it = ObjectList<Backlight>::begin(); it; ++it)
        it->setTimeFactor(Orxonox::getInstance().getTimeFactor());
  }

    /**
    @brief
        Starts the whole Game.
    @param path
        path to config (in home dir or something)
    */
    void Orxonox::start()
    {
#if ORXONOX_DEBUG_MODE == 1
        ConfigFileManager::getSingleton()->setFile(CFT_Settings, "orxonox_d.ini");
#else
        ConfigFileManager::getSingleton()->setFile(CFT_Settings, "orxonox.ini");
#endif

        // creates the class hierarchy for all classes with factories
        Factory::createClassHierarchy();

        setConfigValues();

        const Settings::CommandLineArgument* mode = Settings::getCommandLineArgument("mode");
        assert(mode);
        if (!mode->bHasDefaultValue_)
        {
            Settings::setGameMode(mode->value_);
            this->mode_ = Settings::getGameMode();
        }
        COUT(3) << "Orxonox: Game mode is " << mode_.name << "." << std::endl;

        const Settings::CommandLineArgument* dataPath = Settings::getCommandLineArgument("dataPath");
        assert(dataPath);
        if (!dataPath->bHasDefaultValue_)
        {
            if (*dataPath->value_.getString().end() != '/' && *dataPath->value_.getString().end() != '\\')
                Settings::tsetDataPath(dataPath->value_.getString() + "/");
            else
                Settings::tsetDataPath(dataPath->value_.getString());
        }

        try
        {
            // initialise TCL
            TclBind::getInstance().setDataPath(Settings::getDataPath());

            graphicsEngine_ = new GraphicsEngine();
            graphicsEngine_->setup();       // creates ogre root and other essentials

            if (mode_.showsGraphics)
            {
                graphicsEngine_->loadRenderer();    // creates the render window

                // TODO: Spread this so that this call only initialises things needed for the Console and GUI
                graphicsEngine_->initialiseResources();

                // Calls the InputManager which sets up the input devices.
                // The render window width and height are used to set up the mouse movement.
                inputManager_ = new InputManager();
                inputManager_->initialise(graphicsEngine_->getWindowHandle(),
                    graphicsEngine_->getWindowWidth(), graphicsEngine_->getWindowHeight(), true);
                KeyBinder* keyBinder = new KeyBinder();
                keyBinder->loadBindings();
                inputManager_->createInputState<SimpleInputState>("game", 20)->setHandler(keyBinder);

                // Load the InGameConsole
                console_ = new InGameConsole();
                console_->initialise();

                // load the CEGUI interface
                guiManager_ = new GUIManager();
                guiManager_->initialise();
            }
            else
            {
                // TODO: Initialise a not yet written console that operates in the shell we
                // started the game from.
                // We probably want to use std::cin to catch input (OIS uses DirectX or X server)
            }

            bool showGUI = true;
            if (mode_.mode != GameMode::Unspecified)
            {
                showGUI = false;
                // a game mode was specified with the command line
                // we therefore load the game and level directly

                if (!loadLevel(this->mode_))
                {
                    COUT(1) << "Loading with predefined mode failed. Showing main menu." << std::endl;
                    showGUI = true;
                    mode_ = GameMode::GM_Unspecified;
                }
            }

            if (showGUI)
            {
                // show main menu
                GUIManager::getInstance().showGUI("MainMenu", 0);
                GraphicsEngine::getInstance().getViewport()->setCamera(GUIManager::getInstance().getCamera());
            }
        }
        catch (std::exception& ex)
        {
            COUT(1) << ex.what() << std::endl;
            COUT(1) << "Loading sequence aborted." << std::endl;
            return;
        }

        modeRequest_ = mode_;
        // here happens the game
        startRenderLoop();

        if (mode_.mode == GameMode::Client)
            network::Client::getSingleton()->closeConnection();

        if (mode_.hasServer)
            server_g->close();
    }

    /*static*/ void Orxonox::loadGame(const std::string& name)
    {
        const GameMode& mode = Settings::getGameMode(name);
        if (mode.mode == GameMode::None)
            return;

        getInstance().modeRequest_ = mode;
    }

    bool Orxonox::loadLevel(const GameMode& mode)
    {
        bool success = true;

        if (mode.showsGraphics)
        {
            // create Ogre SceneManager for the level
            graphicsEngine_->createNewScene();

            if (!loadPlayground())
                return false;
        }

        switch (mode.mode)
        {
        case GameMode::Server:
            success &= serverLoad();
            break;
        case GameMode::Client:
            success &= clientLoad();
            break;
        case GameMode::Dedicated:
            success &= serverLoad();
            break;
        case GameMode::Standalone:
            success &= standaloneLoad();
            break;
        default: // never happens
            assert(false);
        }

        if (success)
        {
            InputManager::getInstance().requestEnterState("game");
            this->mode_ = mode;
        }

        return success;
    }

  /**
   * Loads everything in the scene except for the actual objects.
   * This includes HUD, audio..
   */
  bool Orxonox::loadPlayground()
  {
    // Start the Radar
    this->radar_ = new Radar();

    // Load the HUD
    COUT(3) << "Orxonox: Loading HUD" << std::endl;
    hud_ = new Level(Settings::getDataPath() + "overlay/hud.oxo");
    return Loader::load(hud_);
  }

  /**
   * Helper method to load a level.
   */
  bool Orxonox::loadScene()
  {
    COUT(0) << "Loading level..." << std::endl;
    startLevel_ = new Level(Settings::getDataPath() + "levels/sample.oxw");
    return Loader::open(startLevel_);
  }

  /**
   * Level loading method for server mode.
   */
  bool Orxonox::serverLoad()
  {
    COUT(0) << "Loading level in server mode" << std::endl;

    assert(Settings::getCommandLineArgument("port"));
    int serverPort = Settings::getCommandLineArgument("port")->value_;
    //server_g = new network::Server(serverPort_);
    server_g = network::Server::createSingleton(serverPort);

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

    assert(Settings::getCommandLineArgument("port"));
    assert(Settings::getCommandLineArgument("ip"));
    int serverPort = Settings::getCommandLineArgument("port")->value_;
    std::string serverIP = Settings::getCommandLineArgument("ip")->value_;

    if (serverIP.compare("") == 0)
      client_g = network::Client::createSingleton();
    else
      client_g = network::Client::createSingleton(serverIP, serverPort);

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
    Main loop of the orxonox game.
    We use the Ogre::Timer to measure time since it uses the most precise
    method an a platform (however the windows timer lacks time when under
    heavy kernel load!).
    There is a simple mechanism to measure the average time spent in our
    ticks as it may indicate performance issues.
    A note about the Ogre::FrameListener: Even though we don't use them,
    they still get called. However, the delta times are not correct (except
    for timeSinceLastFrame, which is the most important). A little research
    as shown that there is probably only one FrameListener that doesn't even
    need the time. So we shouldn't run into problems.
  */
  void Orxonox::startRenderLoop()
  {
    Ogre::Root& ogreRoot = Ogre::Root::getSingleton();

    // use the ogre timer class to measure time.
    if (!timer_)
      timer_ = new Ogre::Timer();

    unsigned long frameCount = 0;
    
    const unsigned long refreshTime = (unsigned long)(debugRefreshTime_ * 1000000.0f);
    unsigned long refreshStartTime = 0;
    unsigned long tickTime = 0;
    unsigned long oldFrameCount = 0;

    unsigned long timeBeforeTick = 0;
    unsigned long timeBeforeTickOld = 0;
    unsigned long timeAfterTick = 0;

    // TODO: Update time in seconds every 7 seconds to avoid any overflow (7 secs is very tight)

    COUT(3) << "Orxonox: Starting the main loop." << std::endl;

    try
    {
    timer_->reset();
    while (!bAbort_)
    {
        // get current time
        timeBeforeTickOld = timeBeforeTick;
        timeBeforeTick    = timer_->getMicroseconds();
        float dt = (timeBeforeTick - timeBeforeTickOld) / 1000000.0;

        // check whether we have to load a game
        if (mode_.mode != modeRequest_.mode && mode_.mode == GameMode::Unspecified)
        {
            this->loadLevel(modeRequest_);
            this->modeRequest_ = GameMode::GM_None;
        }


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
          GraphicsEngine::getInstance().setAverageTickTime(
              (float)tickTime * 0.001 / (frameCount - oldFrameCount));
          float avgFPS = (float)(frameCount - oldFrameCount) / (timeAfterTick - refreshStartTime) * 1000000.0;
          GraphicsEngine::getInstance().setAverageFramesPerSecond(avgFPS);

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

        if (mode_.showsGraphics)
        {
          // Pump messages in all registered RenderWindows
          // This calls the WindowEventListener objects.
          Ogre::WindowEventUtilities::messagePump();
          // make sure the window stays active even when not focused
          // (probably only necessary on windows)
          GraphicsEngine::getInstance().setWindowActivity(true);

          // tick CEGUI
          GUIManager::getInstance().tick(dt);

          // render
          ogreRoot._updateAllRenderTargets();
        }

        // again, just to be sure ogre works fine
        ogreRoot._fireFrameEnded(evt); // note: uses the same time as _fireFrameStarted

        ++frameCount;
    }
    }
    catch (std::exception& ex)
    {
      // something went wrong.
      COUT(1) << ex.what() << std::endl;
      COUT(1) << "Main loop was stopped by an unhandled exception. Shutting down." << std::endl;
    }
  }
}
