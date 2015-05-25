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
 *      Felix Schulthess
 *
 */

#include "GraphicsManager.h"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/shared_array.hpp>

#include <OgreFrameListener.h>
#include <OgreRoot.h>
#include <OgreLogManager.h>
#include <OgreRenderWindow.h>
#include <OgreRenderSystem.h>
#include <OgreResourceGroupManager.h>
#include <OgreTextureManager.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>

#include "SpecialConfig.h"
#include "util/Clock.h"
#include "util/Convert.h"
#include "util/Exception.h"
#include "util/StringUtils.h"
#include "util/SubString.h"
#include "config/ConfigValueIncludes.h"
#include "CoreIncludes.h"
#include "Core.h"
#include "Game.h"
#include "GameMode.h"
#include "GUIManager.h"
#include "Loader.h"
#include "PathConfig.h"
#include "ViewportEventListener.h"
#include "WindowEventListener.h"
#include "XMLFile.h"
#include "command/ConsoleCommandIncludes.h"
#include "input/InputManager.h"

namespace orxonox
{
    static const std::string __CC_GraphicsManager_group = "GraphicsManager";
    static const std::string __CC_setScreenResolution_name = "setScreenResolution";
    static const std::string __CC_setFSAA_name = "setFSAA";
    static const std::string __CC_setVSync_name = "setVSync";
    DeclareConsoleCommand(__CC_GraphicsManager_group, __CC_setScreenResolution_name, &prototype::string__uint_uint_bool);
    DeclareConsoleCommand(__CC_GraphicsManager_group, __CC_setFSAA_name, &prototype::string__string);
    DeclareConsoleCommand(__CC_GraphicsManager_group, __CC_setVSync_name, &prototype::string__bool);

    static const std::string __CC_printScreen_name = "printScreen";
    DeclareConsoleCommand(__CC_printScreen_name, &prototype::void__void);

    class OgreWindowEventListener : public Ogre::WindowEventListener
    {
    public:
        void windowResized     (Ogre::RenderWindow* rw)
            { orxonox::WindowEventListener::resizeWindow(rw->getWidth(), rw->getHeight()); }
        void windowFocusChange (Ogre::RenderWindow* rw)
            { orxonox::WindowEventListener::changeWindowFocus(rw->isActive()); }
        void windowClosed      (Ogre::RenderWindow* rw)
            { orxonox::Game::getInstance().stop(); }
        void windowMoved       (Ogre::RenderWindow* rw)
            { orxonox::WindowEventListener::moveWindow(); }
    };

    GraphicsManager* GraphicsManager::singletonPtr_s = 0;

    RegisterAbstractClass(GraphicsManager).inheritsFrom<Configurable>();

    GraphicsManager::GraphicsManager(bool bLoadRenderer)
        : ogreWindowEventListener_(new OgreWindowEventListener())
        , renderWindow_(0)
        , viewport_(0)
        , lastFrameStartTime_(0.0f)
        , lastFrameEndTime_(0.0f)
        , destructionHelper_(this)
    {
        RegisterObject(GraphicsManager);

        orxout(internal_status) << "initializing GraphicsManager..." << endl;
        this->setConfigValues();

        // Ogre setup procedure (creating Ogre::Root)
        this->loadOgreRoot();

        // At first, add the root paths of the data directories as resource locations
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(PathConfig::getDataPathString(), "FileSystem");
        // Load resources
        resources_.reset(new XMLFile("DefaultResources.oxr"));
        resources_->setLuaSupport(false);
        Loader::getInstance().open(resources_.get(), ClassTreeMask(), false);

        // Only for runs in the build directory (not installed)
        if (PathConfig::buildDirectoryRun())
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(PathConfig::getExternalDataPathString(), "FileSystem");

        extResources_.reset(new XMLFile("resources.oxr"));
        extResources_->setLuaSupport(false);
        Loader::getInstance().open(extResources_.get(), ClassTreeMask(), false);

        if (bLoadRenderer)
        {
            // Reads the ogre config and creates the render window
            this->upgradeToGraphics();
        }

        orxout(internal_status) << "finished initializing GraphicsManager" << endl;
    }

    void GraphicsManager::destroy()
    {
        orxout(internal_status) << "destroying GraphicsManager..." << endl;

        Loader::getInstance().unload(debugOverlay_.get());

        Ogre::WindowEventUtilities::removeWindowEventListener(renderWindow_, ogreWindowEventListener_);
        ModifyConsoleCommand(__CC_printScreen_name).resetFunction();
        ModifyConsoleCommand(__CC_GraphicsManager_group, __CC_setScreenResolution_name).resetFunction();
        ModifyConsoleCommand(__CC_GraphicsManager_group, __CC_setFSAA_name).resetFunction();
        ModifyConsoleCommand(__CC_GraphicsManager_group, __CC_setVSync_name).resetFunction();

        // Undeclare the resources
        Loader::getInstance().unload(resources_.get());
        Loader::getInstance().unload(extResources_.get());

        safeObjectDelete(&ogreRoot_);
        safeObjectDelete(&ogreLogger_);
        safeObjectDelete(&ogreWindowEventListener_);

        orxout(internal_status) << "finished destroying GraphicsManager" << endl;
    }

    void GraphicsManager::setConfigValues()
    {
        SetConfigValue(ogreConfigFile_,  "ogre.cfg")
            .description("Location of the Ogre config file");
        SetConfigValue(ogrePlugins_, specialConfig::ogrePlugins)
            .description("Comma separated list of all plugins to load.");
        SetConfigValue(ogreLogFile_,     "ogre.log")
            .description("Logfile for messages from Ogre. Use \"\" to suppress log file creation.");
    }

    /**
    @brief
        Loads the renderer and creates the render window if not yet done so.
    @remarks
        This operation is irreversible without recreating the GraphicsManager!
        So if it throws you HAVE to recreate the GraphicsManager!!!
        It therefore offers almost no exception safety.
    */
    void GraphicsManager::upgradeToGraphics()
    {
        if (renderWindow_ != NULL)
            return;

        orxout(internal_info) << "GraphicsManager upgrade to graphics" << endl;

        // load all the required plugins for Ogre
        orxout(user_info) << "Loading Ogre plugins..." << endl;
        this->loadOgrePlugins();

        orxout(user_info) << "Creating render window..." << endl;
        this->loadRenderer();

        // Initialise all resources (do this AFTER the renderer has been loaded!)
        // Note: You can only do this once! Ogre will check whether a resource group has
        // already been initialised. If you need to load resources later, you will have to
        // choose another resource group.
        orxout(user_info) << "Initializing all resource groups..." << endl;
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        orxout(internal_info) << "GraphicsManager finished upgrade to graphics" << endl;
    }

    /**
    @brief
        Creates the Ogre Root object and sets up the ogre log.
    */
    void GraphicsManager::loadOgreRoot()
    {
        orxout(internal_info) << "Setting up Ogre..." << endl;

        if (ogreConfigFile_.empty())
        {
            orxout(internal_warning) << "Ogre config file set to \"\". Defaulting to config.cfg" << endl;
            ModifyConfigValue(ogreConfigFile_, tset, "config.cfg");
        }
        if (ogreLogFile_.empty())
        {
            orxout(internal_warning) << "Ogre log file set to \"\". Defaulting to ogre.log" << endl;
            ModifyConfigValue(ogreLogFile_, tset, "ogre.log");
        }

        boost::filesystem::path ogreConfigFilepath(PathConfig::getConfigPath() / this->ogreConfigFile_);
        boost::filesystem::path ogreLogFilepath(PathConfig::getLogPath() / this->ogreLogFile_);

        // create a new logManager
        // Ogre::Root will detect that we've already created a Log
        ogreLogger_ = new Ogre::LogManager();
        orxout(internal_info) << "Ogre LogManager created" << endl;

        // create our own log that we can listen to
        Ogre::Log *myLog;
        myLog = ogreLogger_->createLog(ogreLogFilepath.string(), true, false, false);
        orxout(internal_info) << "Ogre Log created" << endl;

        myLog->setLogDetail(Ogre::LL_BOREME);
        myLog->addListener(this);

        orxout(internal_info) << "Creating Ogre Root..." << endl;

        // check for config file existence because Ogre displays (caught) exceptions if not
        if (!boost::filesystem::exists(ogreConfigFilepath))
        {
            // create a zero sized file
            std::ofstream creator;
            creator.open(ogreConfigFilepath.string().c_str());
            creator.close();
        }

        // Leave plugins file empty. We're going to do that part manually later
        ogreRoot_ = new Ogre::Root("", ogreConfigFilepath.string(), ogreLogFilepath.string());

        orxout(internal_info) << "Ogre set up done." << endl;
    }

    void GraphicsManager::loadOgrePlugins()
    {
        orxout(internal_info) << "loading ogre plugins" << endl;

        // Plugin path can have many different locations...
        std::string pluginPath = specialConfig::ogrePluginsDirectory;
#ifdef DEPENDENCY_PACKAGE_ENABLE
        if (!PathConfig::buildDirectoryRun())
        {
#  if defined(ORXONOX_PLATFORM_WINDOWS)
            pluginPath = PathConfig::getExecutablePathString();
#  elif defined(ORXONOX_PLATFORM_APPLE)
            // TODO: Where are the plugins being installed to?
            pluginPath = PathConfig::getExecutablePathString();
#  endif
        }
#endif

        // Do some SubString magic to get the comma separated list of plugins
        SubString plugins(ogrePlugins_, ",", " ", false, '\\', false, '"', false, '{', '}', false, '\0');
        for (unsigned int i = 0; i < plugins.size(); ++i)
            ogreRoot_->loadPlugin(pluginPath + '/' + plugins[i]);
    }

    void GraphicsManager::loadRenderer()
    {
        orxout(internal_info) << "GraphicsManager: Configuring Renderer" << endl;

        bool updatedConfig = Core::getInstance().getConfig()->getOgreConfigTimestamp() > Core::getInstance().getConfig()->getLastLevelTimestamp();
        if (updatedConfig)
            orxout(user_info)<< "Ogre config file has changed, but no level was started since then. Displaying config dialogue again to verify the changes." << endl;

        if (!ogreRoot_->restoreConfig() || updatedConfig)
        {
            if (!ogreRoot_->showConfigDialog())
                ThrowException(InitialisationFailed, "OGRE graphics configuration dialogue canceled.");
            else
                Core::getInstance().getConfig()->updateOgreConfigTimestamp();
        }

        orxout(internal_info) << "Creating render window" << endl;

        this->renderWindow_ = ogreRoot_->initialise(true, "Orxonox");
        // Propagate the size of the new winodw
        this->ogreWindowEventListener_->windowResized(renderWindow_);

        Ogre::WindowEventUtilities::addWindowEventListener(this->renderWindow_, ogreWindowEventListener_);

        // create a full screen default viewport
        // Note: This may throw when adding a viewport with an existing z-order!
        //       But in our case we only have one viewport for now anyway, therefore
        //       no ScopeGuards or anything to handle exceptions.
        this->viewport_ = this->renderWindow_->addViewport(0, 0);

        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(Ogre::MIP_UNLIMITED);

        //Add program icon
#if defined(ORXONOX_PLATFORM_WINDOWS)
        HWND hwnd;
        this->renderWindow_->getCustomAttribute("WINDOW", (void*)&hwnd);
        LONG iconID = (LONG)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(101));
        SetClassLong(hwnd, GCL_HICON, iconID);
#endif


        // add console commands
        ModifyConsoleCommand(__CC_printScreen_name).setFunction(&GraphicsManager::printScreen, this);
        ModifyConsoleCommand(__CC_GraphicsManager_group, __CC_setScreenResolution_name).setFunction(&GraphicsManager::setScreenResolution, this);
        ModifyConsoleCommand(__CC_GraphicsManager_group, __CC_setFSAA_name).setFunction(&GraphicsManager::setFSAA, this);
        ModifyConsoleCommand(__CC_GraphicsManager_group, __CC_setVSync_name).setFunction(&GraphicsManager::setVSync, this);
    }

    void GraphicsManager::loadDebugOverlay()
    {
        // Load debug overlay to show info about fps and tick time
        orxout(internal_info) << "Loading Debug Overlay..." << endl;
        debugOverlay_.reset(new XMLFile("debug.oxo"));
        Loader::getInstance().open(debugOverlay_.get(), ClassTreeMask(), false);
    }

    /**
    @note
        A note about the Ogre::FrameListener: Even though we don't use them,
        they still get called.
    */
    void GraphicsManager::postUpdate(const Clock& time)
    {
        // Time before rendering
        uint64_t timeBeforeTick = time.getRealMicroseconds();

        // Ogre's time keeping object
        Ogre::FrameEvent evt;

        // Translate to Ogre float times before the update
        float temp = lastFrameStartTime_;
        lastFrameStartTime_ = (float)timeBeforeTick * 0.000001f;
        evt.timeSinceLastFrame = lastFrameStartTime_ - temp;
        evt.timeSinceLastEvent = lastFrameStartTime_ - lastFrameEndTime_;

        // Ogre requires the time too
        ogreRoot_->_fireFrameStarted(evt);

        // Pump messages in all registered RenderWindows
        // This calls the WindowEventListener objects.
        Ogre::WindowEventUtilities::messagePump();
        // Make sure the window stays active even when not focused
        // (probably only necessary on windows)
        this->renderWindow_->setActive(true);

        // Render frame
        ogreRoot_->_updateAllRenderTargets();

        uint64_t timeAfterTick = time.getRealMicroseconds();
        // Subtract the time used for rendering from the tick time counter
        Game::getInstance().subtractTickTime((int32_t)(timeAfterTick - timeBeforeTick));

        // Translate to Ogre float times after the update
        temp = lastFrameEndTime_;
        lastFrameEndTime_ = (float)timeBeforeTick * 0.000001f;
        evt.timeSinceLastFrame = lastFrameEndTime_ - temp;
        evt.timeSinceLastEvent = lastFrameEndTime_ - lastFrameStartTime_;

        // Ogre also needs the time after the frame finished
        ogreRoot_->_fireFrameEnded(evt);
    }

    void GraphicsManager::setCamera(Ogre::Camera* camera)
    {
        Ogre::Camera* oldCamera = this->viewport_->getCamera();

        this->viewport_->setCamera(camera);
        GUIManager::getInstance().setCamera(camera);

        for (ObjectList<ViewportEventListener>::iterator it = ObjectList<ViewportEventListener>::begin(); it != ObjectList<ViewportEventListener>::end(); ++it)
            it->cameraChanged(this->viewport_, oldCamera);
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
    @param skipThisMessage
        If set to true by the messageLogged() implementation message will not be logged
    */
#if OGRE_VERSION >= 0x010800
    void GraphicsManager::messageLogged(const std::string& message,
        Ogre::LogMessageLevel lml, bool maskDebug, const std::string& logName, bool& skipThisMessage)
        // TODO: do we have to ignore the message if skipThisMessage is set?
#else
    void GraphicsManager::messageLogged(const std::string& message,
        Ogre::LogMessageLevel lml, bool maskDebug, const std::string& logName)
#endif
    {
        OutputLevel orxonoxLevel;
        std::string introduction;
        // Do not show caught OGRE exceptions in front
        if (message.find("EXCEPTION") != std::string::npos)
        {
            orxonoxLevel = level::internal_error;
            introduction = "Ogre, caught exception: ";
        }
        else
        {
            switch (lml)
            {
            case Ogre::LML_TRIVIAL:
                orxonoxLevel = level::verbose_more;
                break;
            case Ogre::LML_NORMAL:
                orxonoxLevel = level::verbose;
                break;
            case Ogre::LML_CRITICAL:
                orxonoxLevel = level::internal_warning;
                break;
            default:
                orxonoxLevel = level::debug_output;
            }
            introduction = "Ogre: ";
        }

        orxout(orxonoxLevel, context::ogre) << introduction << message << endl;
    }

    size_t GraphicsManager::getRenderWindowHandle()
    {
        size_t windowHnd = 0;
        renderWindow_->getCustomAttribute("WINDOW", &windowHnd);
        return windowHnd;
    }

    bool GraphicsManager::isFullScreen() const
    {
        return this->renderWindow_->isFullScreen();
    }

    unsigned int GraphicsManager::getWindowWidth() const
    {
        return this->renderWindow_->getWidth();
    }

    unsigned int GraphicsManager::getWindowHeight() const
    {
        return this->renderWindow_->getHeight();
    }

    bool GraphicsManager::hasVSyncEnabled() const
    {
        Ogre::ConfigOptionMap& options = ogreRoot_->getRenderSystem()->getConfigOptions();
        Ogre::ConfigOptionMap::iterator it = options.find("VSync");
        if (it != options.end())
            return (it->second.currentValue == "Yes");
        else
            return false;
    }

    std::string GraphicsManager::getFSAAMode() const
    {
        Ogre::ConfigOptionMap& options = ogreRoot_->getRenderSystem()->getConfigOptions();
        Ogre::ConfigOptionMap::iterator it = options.find("FSAA");
        if (it != options.end())
            return it->second.currentValue;
        else
            return "";
    }

    std::string GraphicsManager::setScreenResolution(unsigned int width, unsigned int height, bool fullscreen)
    {
        // workaround to detect if the colour depth should be written to the config file
        bool bWriteColourDepth = false;
        Ogre::ConfigOptionMap& options = ogreRoot_->getRenderSystem()->getConfigOptions();
        Ogre::ConfigOptionMap::iterator it = options.find("Video Mode");
        if (it != options.end())
            bWriteColourDepth = (it->second.currentValue.find('@') != std::string::npos);

        if (bWriteColourDepth)
        {
            this->ogreRoot_->getRenderSystem()->setConfigOption("Video Mode", multi_cast<std::string>(width)
                                                                    + " x " + multi_cast<std::string>(height)
                                                                    + " @ " + multi_cast<std::string>(this->getRenderWindow()->getColourDepth()) + "-bit colour");
        }
        else
        {
            this->ogreRoot_->getRenderSystem()->setConfigOption("Video Mode", multi_cast<std::string>(width)
                                                                    + " x " + multi_cast<std::string>(height));
        }

        this->ogreRoot_->getRenderSystem()->setConfigOption("Full Screen", fullscreen ? "Yes" : "No");

        std::string validate = this->ogreRoot_->getRenderSystem()->validateConfigOptions();

        if (validate == "")
        {
            GraphicsManager::getInstance().getRenderWindow()->setFullscreen(fullscreen, width, height);
            this->ogreRoot_->saveConfig();
            Core::getInstance().getConfig()->updateOgreConfigTimestamp();
            // Also reload the input devices
            InputManager::getInstance().reload();
        }

        return validate;
    }

    std::string GraphicsManager::setFSAA(const std::string& mode)
    {
        this->ogreRoot_->getRenderSystem()->setConfigOption("FSAA", mode);

        std::string validate = this->ogreRoot_->getRenderSystem()->validateConfigOptions();

        if (validate == "")
        {
            //this->ogreRoot_->getRenderSystem()->reinitialise(); // can't use this that easily, because it recreates the render window, invalidating renderWindow_
            this->ogreRoot_->saveConfig();
            Core::getInstance().getConfig()->updateOgreConfigTimestamp();
        }

        return validate;
    }

    std::string GraphicsManager::setVSync(bool vsync)
    {
        this->ogreRoot_->getRenderSystem()->setConfigOption("VSync", vsync ? "Yes" : "No");

        std::string validate = this->ogreRoot_->getRenderSystem()->validateConfigOptions();

        if (validate == "")
        {
            //this->ogreRoot_->getRenderSystem()->reinitialise(); // can't use this that easily, because it recreates the render window, invalidating renderWindow_
            this->ogreRoot_->saveConfig();
            Core::getInstance().getConfig()->updateOgreConfigTimestamp();
        }

        return validate;
    }

    void GraphicsManager::printScreen()
    {
        assert(this->renderWindow_);
        this->renderWindow_->writeContentsToTimestampedFile(PathConfig::getLogPathString() + "screenShot_", ".png");
    }
}
