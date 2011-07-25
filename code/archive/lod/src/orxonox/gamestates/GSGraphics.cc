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
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "GSGraphics.h"

#include <fstream>
#include <OgreConfigFile.h>
#include <OgreFrameListener.h>
#include <OgreRoot.h>
#include <OgreLogManager.h>
#include <OgreException.h>
#include <OgreRenderWindow.h>
#include <OgreRenderSystem.h>
#include <OgreTextureManager.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>

#include "util/Debug.h"
#include "util/Exception.h"
#include "core/ConsoleCommand.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/Core.h"
#include "core/input/InputManager.h"
#include "core/input/KeyBinder.h"
#include "core/input/ExtendedInputState.h"
#include "core/Loader.h"
#include "core/XMLFile.h"
#include "overlays/console/InGameConsole.h"
#include "gui/GUIManager.h"
#include "tools/WindowEventListener.h"
#include "Settings.h"

// for compatibility
#include "GraphicsEngine.h"

namespace orxonox
{
    GSGraphics::GSGraphics()
        : GameState<GSRoot>("graphics")
        , renderWindow_(0)
        , viewport_(0)
        , bWindowEventListenerUpdateRequired_(false)
        , inputManager_(0)
        , console_(0)
        , guiManager_(0)
        , ogreRoot_(0)
        , ogreLogger_(0)
        , graphicsEngine_(0)
        , masterKeyBinder_(0)
        , frameCount_(0)
        , statisticsRefreshCycle_(0)
        , statisticsStartTime_(0)
        , statisticsStartCount_(0)
        , tickTime_(0)
        , debugOverlay_(0)
    {
        RegisterRootObject(GSGraphics);
        setConfigValues();
    }

    GSGraphics::~GSGraphics()
    {
    }

    void GSGraphics::setConfigValues()
    {
        SetConfigValue(resourceFile_,    "resources.cfg")
            .description("Location of the resources file in the data path.");
        SetConfigValue(ogreConfigFile_,  "ogre.cfg")
            .description("Location of the Ogre config file");
        SetConfigValue(ogrePluginsFile_, "plugins.cfg")
            .description("Location of the Ogre plugins file");
        SetConfigValue(ogreLogFile_,     "ogre.log")
            .description("Logfile for messages from Ogre. Use \"\" to suppress log file creation.");
        SetConfigValue(ogreLogLevelTrivial_ , 5)
            .description("Corresponding orxonox debug level for ogre Trivial");
        SetConfigValue(ogreLogLevelNormal_  , 4)
            .description("Corresponding orxonox debug level for ogre Normal");
        SetConfigValue(ogreLogLevelCritical_, 2)
            .description("Corresponding orxonox debug level for ogre Critical");
        SetConfigValue(statisticsRefreshCycle_, 200000)
            .description("Sets the time in microseconds interval at which average fps, etc. get updated.");
        SetConfigValue(defaultMasterKeybindings_, "def_masterKeybindings.ini")
            .description("Filename of default master keybindings.");
    }

    void GSGraphics::enter()
    {
        Core::setShowsGraphics(true);

        // initialise graphics engine. Doesn't load the render window yet!
        graphicsEngine_ = new GraphicsEngine();

        // Ogre setup procedure
        setupOgre();
        this->declareResources();
        this->loadRenderer();    // creates the render window
        // TODO: Spread this so that this call only initialises things needed for the Console and GUI
        this->initialiseResources();

        // We want to get informed whenever an object of type WindowEventListener is created
        // in order to later update the window size.
        bWindowEventListenerUpdateRequired_ = false;
        RegisterConstructionCallback(GSGraphics, orxonox::WindowEventListener, requestWindowEventListenerUpdate);

        // load debug overlay
        COUT(3) << "Loading Debug Overlay..." << std::endl;
        this->debugOverlay_ = new XMLFile(Settings::getDataPath() + "overlay/debug.oxo");
        Loader::open(debugOverlay_);

        // Calls the InputManager which sets up the input devices.
        // The render window width and height are used to set up the mouse movement.
        inputManager_ = new InputManager();
        size_t windowHnd = 0;
        this->renderWindow_->getCustomAttribute("WINDOW", &windowHnd);
        inputManager_->initialise(windowHnd, renderWindow_->getWidth(), renderWindow_->getHeight(), true);
        // Configure master input state with a KeyBinder
        masterKeyBinder_ = new KeyBinder();
        masterKeyBinder_->loadBindings("masterKeybindings.ini", defaultMasterKeybindings_);
        inputManager_->getMasterInputState()->addKeyHandler(masterKeyBinder_);

        // Load the InGameConsole
        console_ = new InGameConsole();
        console_->initialise(this->renderWindow_->getWidth(), this->renderWindow_->getHeight());

        // load the CEGUI interface
        guiManager_ = new GUIManager();
        guiManager_->initialise(this->renderWindow_);

        // reset frame counter
        this->frameCount_ = 0;
        this->tickTime_ = 0;
        statisticsStartTime_ = 0;
        statisticsStartCount_ = 0;

        // add console commands
        FunctorMember<GSGraphics>* functor1 = createFunctor(&GSGraphics::printScreen);
        functor1->setObject(this);
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor1, "printScreen"));
    }

    void GSGraphics::leave()
    {
        using namespace Ogre;

        // remove our WindowEventListener first to avoid bad calls after the window has been destroyed
        Ogre::WindowEventUtilities::removeWindowEventListener(this->renderWindow_, this);

        delete this->guiManager_;

        delete this->console_;

        //inputManager_->getMasterInputState()->removeKeyHandler(this->masterKeyBinder_);
        delete this->masterKeyBinder_;
        delete this->inputManager_;

        Loader::unload(this->debugOverlay_);
        delete this->debugOverlay_;

        // destroy render window
        RenderSystem* renderer = this->ogreRoot_->getRenderSystem();
        renderer->destroyRenderWindow("Orxonox");

        /*** CODE SNIPPET, UNUSED ***/
        // Does the opposite of initialise()
        //ogreRoot_->shutdown();
        // Remove all resources and resource groups
        //StringVector groups = ResourceGroupManager::getSingleton().getResourceGroups();
        //for (StringVector::iterator it = groups.begin(); it != groups.end(); ++it)
        //{
        //    ResourceGroupManager::getSingleton().destroyResourceGroup(*it);
        //}

        //ParticleSystemManager::getSingleton().removeAllTemplates();

        // Shutdown the render system
        //this->ogreRoot_->setRenderSystem(0);

        delete this->ogreRoot_;

#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
        // delete the ogre log and the logManager (since we have created it).
        this->ogreLogger_->getDefaultLog()->removeListener(this);
        this->ogreLogger_->destroyLog(Ogre::LogManager::getSingleton().getDefaultLog());
        delete this->ogreLogger_;
#endif

        delete graphicsEngine_;

        Core::setShowsGraphics(false);
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
    void GSGraphics::ticked(const Clock& time)
    {
        unsigned long long timeBeforeTick = time.getRealMicroseconds();
        float dt = time.getDeltaTime();

        this->inputManager_->tick(dt);
        // tick console
        this->console_->tick(dt);
        this->tickChild(time);

        if (this->bWindowEventListenerUpdateRequired_)
        {
            // Update all WindowEventListeners for the case a new one was created.
            this->windowResized(this->renderWindow_);
            this->bWindowEventListenerUpdateRequired_ = false;
        }

        unsigned long long timeAfterTick = time.getRealMicroseconds();

        tickTime_ += (unsigned int)(timeAfterTick - timeBeforeTick);
        if (timeAfterTick > statisticsStartTime_ + statisticsRefreshCycle_)
        {
            GraphicsEngine::getInstance().setAverageTickTime(
                (float)tickTime_ * 0.001f / (frameCount_ - statisticsStartCount_));
            float avgFPS = (float)(frameCount_ - statisticsStartCount_)
                / (timeAfterTick - statisticsStartTime_) * 1000000.0;
            GraphicsEngine::getInstance().setAverageFramesPerSecond(avgFPS);

            tickTime_ = 0;
            statisticsStartCount_ = frameCount_;
            statisticsStartTime_  = timeAfterTick;
        }

        // don't forget to call _fireFrameStarted in ogre to make sure
        // everything goes smoothly
        Ogre::FrameEvent evt;
        evt.timeSinceLastFrame = dt;
        evt.timeSinceLastEvent = dt; // note: same time, but shouldn't matter anyway
        ogreRoot_->_fireFrameStarted(evt);

        // Pump messages in all registered RenderWindows
        // This calls the WindowEventListener objects.
        Ogre::WindowEventUtilities::messagePump();
        // make sure the window stays active even when not focused
        // (probably only necessary on windows)
        this->renderWindow_->setActive(true);

        // render
        ogreRoot_->_updateAllRenderTargets();

        // again, just to be sure ogre works fine
        ogreRoot_->_fireFrameEnded(evt); // note: uses the same time as _fireFrameStarted

        ++frameCount_;
    }

    /**
    @brief
        Creates the Ogre Root object and sets up the ogre log.
    */
    void GSGraphics::setupOgre()
    {
        COUT(3) << "Setting up Ogre..." << std::endl;

        // TODO: LogManager doesn't work on oli platform. The why is yet unknown.
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
        // create a new logManager
        ogreLogger_ = new Ogre::LogManager();
        COUT(4) << "Ogre LogManager created" << std::endl;

        // create our own log that we can listen to
        Ogre::Log *myLog;
        if (this->ogreLogFile_ == "")
            myLog = ogreLogger_->createLog("ogre.log", true, false, true);
        else
            myLog = ogreLogger_->createLog(this->ogreLogFile_, true, false, false);
        COUT(4) << "Ogre Log created" << std::endl;

        myLog->setLogDetail(Ogre::LL_BOREME);
        myLog->addListener(this);
#endif

        // Root will detect that we've already created a Log
        COUT(4) << "Creating Ogre Root..." << std::endl;

        if (ogrePluginsFile_ == "")
        {
            COUT(2) << "Warning: Ogre plugins file set to \"\". Defaulting to plugins.cfg" << std::endl;
            ModifyConfigValue(ogrePluginsFile_, tset, "plugins.cfg");
        }
        if (ogreConfigFile_ == "")
        {
            COUT(2) << "Warning: Ogre config file set to \"\". Defaulting to config.cfg" << std::endl;
            ModifyConfigValue(ogreConfigFile_, tset, "config.cfg");
        }
        if (ogreLogFile_ == "")
        {
            COUT(2) << "Warning: Ogre log file set to \"\". Defaulting to ogre.log" << std::endl;
            ModifyConfigValue(ogreLogFile_, tset, "ogre.log");
        }

        // check for config file existence because Ogre displays (caught) exceptions if not
        std::ifstream probe;
        probe.open(ogreConfigFile_.c_str());
        if (!probe)
        {
            // create a zero sized file
            std::ofstream creator;
            creator.open(ogreConfigFile_.c_str());
            creator.close();
        }
        else
            probe.close();

        ogreRoot_ = new Ogre::Root(ogrePluginsFile_, ogreConfigFile_, ogreLogFile_);

#if 0 // Ogre 1.4.3 doesn't yet support setDebugOutputEnabled(.)
#if ORXONOX_PLATFORM != ORXONOX_PLATFORM_WIN32
        // tame the ogre ouput so we don't get all the mess in the console
        Ogre::Log* defaultLog = Ogre::LogManager::getSingleton().getDefaultLog();
        defaultLog->setDebugOutputEnabled(false);
        defaultLog->setLogDetail(Ogre::LL_BOREME);
        defaultLog->addListener(this);
#endif
#endif

        COUT(3) << "Ogre set up done." << std::endl;
    }

    void GSGraphics::declareResources()
    {
        CCOUT(4) << "Declaring Resources" << std::endl;
        //TODO: Specify layout of data file and maybe use xml-loader
        //TODO: Work with ressource groups (should be generated by a special loader)

        if (resourceFile_ == "")
        {
            COUT(2) << "Warning: Ogre resource file set to \"\". Defaulting to resources.cfg" << std::endl;
            ModifyConfigValue(resourceFile_, tset, "resources.cfg");
        }

        // Load resource paths from data file using configfile ressource type
        Ogre::ConfigFile cf;
        try
        {
            cf.load(Settings::getDataPath() + resourceFile_);
        }
        catch (...)
        {
            //COUT(1) << ex.getFullDescription() << std::endl;
            COUT(0) << "Have you forgotten to set the data path in orxnox.ini?" << std::endl;
            throw;
        }

        // Go through all sections & settings in the file
        Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

        std::string secName, typeName, archName;
        while (seci.hasMoreElements())
        {
            try
            {
                secName = seci.peekNextKey();
                Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
                Ogre::ConfigFile::SettingsMultiMap::iterator i;
                for (i = settings->begin(); i != settings->end(); ++i)
                {
                    typeName = i->first; // for instance "FileSystem" or "Zip"
                    archName = i->second; // name (and location) of archive

                    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                        std::string(Settings::getDataPath() + archName), typeName, secName);
                }
            }
            catch (Ogre::Exception& ex)
            {
                COUT(1) << ex.getFullDescription() << std::endl;
            }
        }
    }

    void GSGraphics::loadRenderer()
    {
        CCOUT(4) << "Configuring Renderer" << std::endl;

        if (!ogreRoot_->restoreConfig())
            if (!ogreRoot_->showConfigDialog())
                ThrowException(InitialisationFailed, "Could not show Ogre configuration dialogue.");

        CCOUT(4) << "Creating render window" << std::endl;

        this->renderWindow_ = ogreRoot_->initialise(true, "Orxonox");

        Ogre::WindowEventUtilities::addWindowEventListener(this->renderWindow_, this);

        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(0);

        // create a full screen default viewport
        this->viewport_ = this->renderWindow_->addViewport(0, 0);
    }

    void GSGraphics::initialiseResources()
    {
        CCOUT(4) << "Initialising resources" << std::endl;
        //TODO: Do NOT load all the groups, why are we doing that? And do we really do that? initialise != load...
        //try
        //{
            Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
            /*Ogre::StringVector str = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
            for (unsigned int i = 0; i < str.size(); i++)
            {
            Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(str[i]);
            }*/
        //}
        //catch (...)
        //{
        //    CCOUT(2) << "Error: There was a serious error when initialising the resources." << std::endl;
        //    throw;
        //}
    }

    /**
    @brief
        Method called by the LogListener interface from Ogre.
        We use it to capture Ogre log messages and handle it ourselves.
    @param message
        The message to be logged
    @param lml
        The message level the log is using
    @param maskDebug
        If we are printing to the console or not
    @param logName
        The name of this log (so you can have several listeners
        for different logs, and identify them)
    */
    void GSGraphics::messageLogged(const std::string& message,
        Ogre::LogMessageLevel lml, bool maskDebug, const std::string& logName)
    {
        int orxonoxLevel;
        switch (lml)
        {
        case Ogre::LML_TRIVIAL:
            orxonoxLevel = this->ogreLogLevelTrivial_;
            break;
        case Ogre::LML_NORMAL:
            orxonoxLevel = this->ogreLogLevelNormal_;
            break;
        case Ogre::LML_CRITICAL:
            orxonoxLevel = this->ogreLogLevelCritical_;
            break;
        default:
            orxonoxLevel = 0;
        }
        OutputHandler::getOutStream().setOutputLevel(orxonoxLevel)
            << "Ogre: " << message << std::endl;
    }

    /**
    @brief
        Window has moved.
    @param rw
        The render window it occured in
    */
    void GSGraphics::windowMoved(Ogre::RenderWindow *rw)
    {
        for (ObjectList<orxonox::WindowEventListener>::iterator it = ObjectList<orxonox::WindowEventListener>::begin(); it; ++it)
            it->windowMoved();
    }

    /**
    @brief
        Window has resized.
    @param rw
        The render window it occured in
    @note
        GraphicsEngine has a render window stored itself. This is the same
        as rw. But we have to be careful when using multiple render windows!
    */
    void GSGraphics::windowResized(Ogre::RenderWindow *rw)
    {
        for (ObjectList<orxonox::WindowEventListener>::iterator it = ObjectList<orxonox::WindowEventListener>::begin(); it; ++it)
            it->windowResized(this->renderWindow_->getWidth(), this->renderWindow_->getHeight());

        // OIS needs this under linux even if we only use relative input measurement.
        if (this->inputManager_)
            this->inputManager_->setWindowExtents(renderWindow_->getWidth(), renderWindow_->getHeight());
    }

    /**
    @brief
        Window focus has changed.
    @param rw
        The render window it occured in
    */
    void GSGraphics::windowFocusChange(Ogre::RenderWindow *rw)
    {
        for (ObjectList<orxonox::WindowEventListener>::iterator it = ObjectList<orxonox::WindowEventListener>::begin(); it; ++it)
            it->windowFocusChanged();

        // instruct InputManager to clear the buffers (core library so we cannot use the interface)
        if (this->inputManager_)
            this->inputManager_->clearBuffers();
    }

    /**
    @brief
        Window was closed.
    @param rw
        The render window it occured in
    */
    void GSGraphics::windowClosed(Ogre::RenderWindow *rw)
    {
        this->requestState("root");
    }

    void GSGraphics::printScreen()
    {
        if (this->renderWindow_)
        {
            this->renderWindow_->writeContentsToTimestampedFile("shot_", ".jpg");
        }
    }
}
