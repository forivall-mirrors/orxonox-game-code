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
#include "util/MathConvert.h"
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
#include "core/Iterator.h"
#include "core/ConsoleCommand.h"
#include "core/Loader.h"
#include "core/input/InputManager.h"
#include "core/TclBind.h"
#include "core/Core.h"
#include "util/Debug.h"

// audio
#include "audio/AudioManager.h"

// network
#include "network/Server.h"
#include "network/Client.h"

// objects and tools
#include "hud/HUD.h"
#include "objects/Tickable.h"
#include "tools/ParticleInterface.h"

#include "GraphicsEngine.h"
#include "Settings.h"


#define TestConv(nr, Type1, var1, Type2, res) \
    Type1 var##nr##1##var1; \
    Type2 var##nr##2; \
    assert(ConvertValue(&var##nr##2, var##nr##1)); \
    COUT(0) << "Converting " << #var1 << " (" << #Type1 << ") to " << var##nr##2 << " (" #Type2 << ")." << std::endl; \
    assert(res == var##nr##2)

template <>
struct ConverterExplicit<orxonox::Radian, const char*>
{
    static bool convert(orxonox::Radian* output, const char* input)
    {
        //std::string asdf("asfd");
        //std::cout << asdf;
        float temp;
        convertValue(&temp, input);
        *output = temp;
    }
};

class FooBar { };

inline bool explicitConversion(FooBar* output, std::string input)
{
    return true;
}

//inline bool explicitConversion(std::string* output, const orxonox::Vector3& input)
//{
//    std::ostringstream ostream;
//    if (ostream << input.x << "," << input.y << "," << input.z)
//    {
//        (*output) = ostream.str();
//        return true;
//    }
//    return false;
//}



// FIXME: is this really file scope?
// globals for the server or client
network::Client *client_g = 0;
network::Server *server_g = 0;

namespace orxonox
{
  SetConsoleCommandShortcut(Orxonox, exit).keybindMode(KeybindMode::OnPress);
  SetConsoleCommandShortcut(Orxonox, slomo).accessLevel(AccessLevel::Offline).defaultValue(0, 1.0).axisParamIndex(0).isAxisRelative(false);
  SetConsoleCommandShortcut(Orxonox, setTimeFactor).accessLevel(AccessLevel::Offline).defaultValue(0, 1.0);

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
    this->orxonoxHUD_->destroy();
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

    for (ObjectList<ParticleInterface>::iterator it = ObjectList<ParticleInterface>::begin(); it; ++it)
        it->setSpeedFactor(it->getSpeedFactor() * change);
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
    ConfigFileManager::getInstance()->setFile(CFT_Settings, "orxonox_d.ini");
#else
    ConfigFileManager::getInstance()->setFile(CFT_Settings, "orxonox.ini");
#endif
    Factory::createClassHierarchy();

    std::string nmbr;
    char res('a');
    //const char* nmbr;
    //const char* str;
    convertValue(&nmbr, res);
    //const unsigned int blah = 4;
    //convertValue(nmbr, blah);
    //convertValue(&str, 4.0f);

    //using ::operator<<;
    using std::string;
    int a = 3;
    FooBar asdf;
    std::string asdf2;
    std::ostringstream oss;
    char blah(' ');
    oss << blah;
    std::cout << asdf2;
    ConvertValue(&asdf2, asdf);
    //if (conversionTests::OStreamOperator<FooBar>::exists)
    //    int asdfasdfasdf = 7;
    //COUT(3) << asdf;

    TestConv(1, int, (3), float, 3.0);
    TestConv(2, int, (3), string, "3");
    TestConv(3, string, ("3.0"), float, 3.0f);
    TestConv(4, char, ('a'), string, "a");
    TestConv(5, string, ("df"), char, 'd');
    TestConv(6, Vector2, (3,4), string, "3,4");
    TestConv(7, const char*, ("4.3"), float, 4.3f);
    TestConv(8, const char*, ("4,3"), Vector2, Vector2(4,3));
    //TestConv(9, const char*, ("4.4"), Radian, Radian(4.4));
    TestConv(100, int, (3), const char*, "3");
    TestConv(101, Vector3, (1, 2, 3), float, 3.0);

    std::ostringstream out;

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
    orxonoxHUD_->initialise();
    return true;
  }

  /**
   * Level loading method for server mode.
   */
  bool Orxonox::serverLoad()
  {
    COUT(2) << "Loading level in server mode" << std::endl;

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
//    clock_t time = 0;

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

      // tick the core
      Core::tick((float)evt.timeSinceLastFrame);
      // Call those objects that need the real time
      for (ObjectList<TickableReal>::iterator it = ObjectList<TickableReal>::begin(); it; ++it)
        it->tick((float)evt.timeSinceLastFrame);
      // Call the scene objects
      for (ObjectList<Tickable>::iterator it = ObjectList<Tickable>::begin(); it; ++it)
        it->tick((float)evt.timeSinceLastFrame * this->timefactor_);
      //AudioManager::tick();
      if (client_g)
        client_g->tick((float)evt.timeSinceLastFrame);
      if (server_g)
        server_g->tick((float)evt.timeSinceLastFrame);

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
