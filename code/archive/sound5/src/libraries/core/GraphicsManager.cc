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

#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/shared_array.hpp>

#include <OgreArchiveFactory.h>
#include <OgreArchiveManager.h>
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
#include "util/Exception.h"
#include "util/StringUtils.h"
#include "util/SubString.h"
#include "ConsoleCommand.h"
#include "ConfigValueIncludes.h"
#include "CoreIncludes.h"
#include "Game.h"
#include "GameMode.h"
#include "Loader.h"
#include "MemoryArchive.h"
#include "PathConfig.h"
#include "WindowEventListener.h"
#include "XMLFile.h"

namespace orxonox
{
    class OgreWindowEventListener : public Ogre::WindowEventListener
    {
    public:
        void windowResized     (Ogre::RenderWindow* rw)
            { orxonox::WindowEventListener::resizeWindow(rw->getWidth(), rw->getHeight()); }
        void windowFocusChange (Ogre::RenderWindow* rw)
            { orxonox::WindowEventListener::changeWindowFocus(); }
        void windowClosed      (Ogre::RenderWindow* rw)
            { orxonox::Game::getInstance().stop(); }
        void windowMoved       (Ogre::RenderWindow* rw)
            { orxonox::WindowEventListener::moveWindow(); }
    };

    GraphicsManager* GraphicsManager::singletonPtr_s = 0;

    /**
    @brief
        Non-initialising constructor.
    */
    GraphicsManager::GraphicsManager(bool bLoadRenderer)
        : ogreWindowEventListener_(new OgreWindowEventListener())
#if OGRE_VERSION < 0x010600
        , memoryArchiveFactory_(new MemoryArchiveFactory())
#endif
        , renderWindow_(0)
        , viewport_(0)
    {
        RegisterObject(GraphicsManager);

        this->setConfigValues();

        // Ogre setup procedure (creating Ogre::Root)
        this->loadOgreRoot();

        // At first, add the root paths of the data directories as resource locations
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(PathConfig::getDataPathString(), "FileSystem");
        // Load resources
        resources_.reset(new XMLFile("DefaultResources.oxr"));
        resources_->setLuaSupport(false);
        Loader::open(resources_.get());

        // Only for development runs
        if (PathConfig::isDevelopmentRun())
        {
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(PathConfig::getExternalDataPathString(), "FileSystem");
            extResources_.reset(new XMLFile("resources.oxr"));
            extResources_->setLuaSupport(false);
            Loader::open(extResources_.get());
        }

        if (bLoadRenderer)
        {
            // Reads the ogre config and creates the render window
            this->upgradeToGraphics();
        }
    }

    /**
    @brief
        Destruction is done by the member scoped_ptrs.
    */
    GraphicsManager::~GraphicsManager()
    {
        Loader::unload(debugOverlay_.get());

        Ogre::WindowEventUtilities::removeWindowEventListener(renderWindow_, ogreWindowEventListener_.get());
        // TODO: Destroy the console command

        // Undeclare the resources
        Loader::unload(resources_.get());
        if (PathConfig::isDevelopmentRun())
            Loader::unload(extResources_.get());
    }

    void GraphicsManager::setConfigValues()
    {
        SetConfigValue(ogreConfigFile_,  "ogre.cfg")
            .description("Location of the Ogre config file");
        SetConfigValue(ogrePluginsDirectory_, specialConfig::ogrePluginsDirectory)
            .description("Folder where the Ogre plugins are located.");
        SetConfigValue(ogrePlugins_, specialConfig::ogrePlugins)
            .description("Comma separated list of all plugins to load.");
        SetConfigValue(ogreLogFile_,     "ogre.log")
            .description("Logfile for messages from Ogre. Use \"\" to suppress log file creation.");
        SetConfigValue(ogreLogLevelTrivial_ , 5)
            .description("Corresponding orxonox debug level for ogre Trivial");
        SetConfigValue(ogreLogLevelNormal_  , 4)
            .description("Corresponding orxonox debug level for ogre Normal");
        SetConfigValue(ogreLogLevelCritical_, 2)
            .description("Corresponding orxonox debug level for ogre Critical");
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

        // load all the required plugins for Ogre
        this->loadOgrePlugins();

        this->loadRenderer();

#if OGRE_VERSION < 0x010600
        // WORKAROUND: There is an incompatibility for particle scripts when trying
        // to support both Ogre 1.4 and 1.6. The hacky solution is to create
        // scripts for the 1.6 version and then remove the inserted "particle_system"
        // keyword. But we need to supply these new scripts as well, which is why
        // there is an extra Ogre::Archive dealing with it in the memory.
        using namespace Ogre;
        ArchiveManager::getSingleton().addArchiveFactory(memoryArchiveFactory_.get());
        const StringVector& groups = ResourceGroupManager::getSingleton().getResourceGroups();
        // Travers all groups
        for (StringVector::const_iterator itGroup = groups.begin(); itGroup != groups.end(); ++itGroup)
        {
            FileInfoListPtr files = ResourceGroupManager::getSingleton().findResourceFileInfo(*itGroup, "*.particle");
            for (FileInfoList::const_iterator itFile = files->begin(); itFile != files->end(); ++itFile)
            {
                // open file
                Ogre::DataStreamPtr input = ResourceGroupManager::getSingleton().openResource(itFile->filename, *itGroup, false);
                std::stringstream output;
                // Parse file and replace "particle_system" with nothing
                while (!input->eof())
                {
                    std::string line = input->getLine();
                    size_t pos = line.find("particle_system");
                    if (pos != std::string::npos)
                    {
                        // 15 is the length of "particle_system"
                        line.replace(pos, 15, "");
                    }
                    output << line << std::endl;
                }
                // Add file to the memory archive
                shared_array<char> data(new char[output.str().size()]);
                // Debug optimisations
                const std::string& outputStr = output.str();
                char* rawData = data.get();
                for (unsigned i = 0; i < outputStr.size(); ++i)
                    rawData[i] = outputStr[i];
                MemoryArchive::addFile("particle_scripts_ogre_1.4_" + *itGroup, itFile->filename, data, output.str().size());
            }
            if (!files->empty())
            {
                // Declare the files, but using a new group
                ResourceGroupManager::getSingleton().addResourceLocation("particle_scripts_ogre_1.4_" + *itGroup,
                    "Memory", "particle_scripts_ogre_1.4_" + *itGroup);
            }
        }
#endif

        // Initialise all resources (do this AFTER the renderer has been loaded!)
        // Note: You can only do this once! Ogre will check whether a resource group has
        // already been initialised. If you need to load resources later, you will have to
        // choose another resource group.
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }

    /**
    @brief
        Creates the Ogre Root object and sets up the ogre log.
    */
    void GraphicsManager::loadOgreRoot()
    {
        COUT(3) << "Setting up Ogre..." << std::endl;

        if (ogreConfigFile_.empty())
        {
            COUT(2) << "Warning: Ogre config file set to \"\". Defaulting to config.cfg" << std::endl;
            ModifyConfigValue(ogreConfigFile_, tset, "config.cfg");
        }
        if (ogreLogFile_.empty())
        {
            COUT(2) << "Warning: Ogre log file set to \"\". Defaulting to ogre.log" << std::endl;
            ModifyConfigValue(ogreLogFile_, tset, "ogre.log");
        }

        boost::filesystem::path ogreConfigFilepath(PathConfig::getConfigPath() / this->ogreConfigFile_);
        boost::filesystem::path ogreLogFilepath(PathConfig::getLogPath() / this->ogreLogFile_);

        // create a new logManager
        // Ogre::Root will detect that we've already created a Log
        ogreLogger_.reset(new Ogre::LogManager());
        COUT(4) << "Ogre LogManager created" << std::endl;

        // create our own log that we can listen to
        Ogre::Log *myLog;
        myLog = ogreLogger_->createLog(ogreLogFilepath.string(), true, false, false);
        COUT(4) << "Ogre Log created" << std::endl;

        myLog->setLogDetail(Ogre::LL_BOREME);
        myLog->addListener(this);

        COUT(4) << "Creating Ogre Root..." << std::endl;

        // check for config file existence because Ogre displays (caught) exceptions if not
        if (!boost::filesystem::exists(ogreConfigFilepath))
        {
            // create a zero sized file
            std::ofstream creator;
            creator.open(ogreConfigFilepath.string().c_str());
            creator.close();
        }

        // Leave plugins file empty. We're going to do that part manually later
        ogreRoot_.reset(new Ogre::Root("", ogreConfigFilepath.string(), ogreLogFilepath.string()));

        COUT(3) << "Ogre set up done." << std::endl;
    }

    void GraphicsManager::loadOgrePlugins()
    {
        // just to make sure the next statement doesn't segfault
        if (ogrePluginsDirectory_.empty())
            ogrePluginsDirectory_ = '.';

        boost::filesystem::path folder(ogrePluginsDirectory_);
        // Do some SubString magic to get the comma separated list of plugins
        SubString plugins(ogrePlugins_, ",", " ", false, '\\', false, '"', false, '(', ')', false, '\0');
        // Use backslash paths on Windows! file_string() already does that though.
        for (unsigned int i = 0; i < plugins.size(); ++i)
            ogreRoot_->loadPlugin((folder / plugins[i]).file_string());
    }

    void GraphicsManager::loadRenderer()
    {
        CCOUT(4) << "Configuring Renderer" << std::endl;

        if (!ogreRoot_->restoreConfig())
            if (!ogreRoot_->showConfigDialog())
                ThrowException(InitialisationFailed, "OGRE graphics configuration dialogue failed.");

        CCOUT(4) << "Creating render window" << std::endl;

        this->renderWindow_ = ogreRoot_->initialise(true, "Orxonox");
        // Propagate the size of the new winodw
        this->ogreWindowEventListener_->windowResized(renderWindow_);

        Ogre::WindowEventUtilities::addWindowEventListener(this->renderWindow_, ogreWindowEventListener_.get());

        // create a full screen default viewport
        // Note: This may throw when adding a viewport with an existing z-order!
        //       But in our case we only have one viewport for now anyway, therefore
        //       no ScopeGuards or anything to handle exceptions.
        this->viewport_ = this->renderWindow_->addViewport(0, 0);

        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(0);

        // add console commands
        ccPrintScreen_ = createConsoleCommand(createFunctor(&GraphicsManager::printScreen, this), "printScreen");
        CommandExecutor::addConsoleCommandShortcut(ccPrintScreen_);
    }

    void GraphicsManager::loadDebugOverlay()
    {
        // Load debug overlay to show info about fps and tick time
        COUT(4) << "Loading Debug Overlay..." << std::endl;
        debugOverlay_.reset(new XMLFile("debug.oxo"));
        Loader::open(debugOverlay_.get());
    }

    /**
    @note
        A note about the Ogre::FrameListener: Even though we don't use them,
        they still get called. However, the delta times are not correct (except
        for timeSinceLastFrame, which is the most important). A little research
        as shown that there is probably only one FrameListener that doesn't even
        need the time. So we shouldn't run into problems.
    */
    void GraphicsManager::postUpdate(const Clock& time)
    {
        Ogre::FrameEvent evt;
        evt.timeSinceLastFrame = time.getDeltaTime();
        evt.timeSinceLastEvent = time.getDeltaTime(); // note: same time, but shouldn't matter anyway

        // don't forget to call _fireFrameStarted to OGRE to make sure
        // everything goes smoothly
        ogreRoot_->_fireFrameStarted(evt);

        // Pump messages in all registered RenderWindows
        // This calls the WindowEventListener objects.
        Ogre::WindowEventUtilities::messagePump();
        // make sure the window stays active even when not focused
        // (probably only necessary on windows)
        this->renderWindow_->setActive(true);

        // Time before rendering
        uint64_t timeBeforeTick = time.getRealMicroseconds();

        // Render frame
        ogreRoot_->_updateAllRenderTargets();

        uint64_t timeAfterTick = time.getRealMicroseconds();
        // Subtract the time used for rendering from the tick time counter
        Game::getInstance().subtractTickTime((int32_t)(timeAfterTick - timeBeforeTick));

        // again, just to be sure OGRE works fine
        ogreRoot_->_fireFrameEnded(evt); // note: uses the same time as _fireFrameStarted
    }

    void GraphicsManager::setCamera(Ogre::Camera* camera)
    {
        this->viewport_->setCamera(camera);
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
    void GraphicsManager::messageLogged(const std::string& message,
        Ogre::LogMessageLevel lml, bool maskDebug, const std::string& logName)
    {
        int orxonoxLevel;
        std::string introduction;
        // Do not show caught OGRE exceptions in front
        if (message.find("EXCEPTION") != std::string::npos)
        {
            orxonoxLevel = OutputLevel::Debug;
            introduction = "Ogre, caught exception: ";
        }
        else
        {
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
            introduction = "Ogre: ";
        }
        OutputHandler::getOutStream(orxonoxLevel)
            << introduction << message << std::endl;
    }

    size_t GraphicsManager::getRenderWindowHandle()
    {
        size_t windowHnd = 0;
        renderWindow_->getCustomAttribute("WINDOW", &windowHnd);
        return windowHnd;
    }

    bool GraphicsManager::isFullScreen() const
    {
        Ogre::ConfigOptionMap& options = ogreRoot_->getRenderSystem()->getConfigOptions();
        if (options.find("Full Screen") != options.end())
        {
            if (options["Full Screen"].currentValue == "Yes")
                return true;
            else
                return false;
        }
        else
        {
            COUT(0) << "Could not find 'Full Screen' render system option. Fix This!!!" << std::endl;
            return false;
        }
    }

    void GraphicsManager::printScreen()
    {
        assert(this->renderWindow_);
        this->renderWindow_->writeContentsToTimestampedFile(PathConfig::getLogPathString() + "screenShot_", ".png");
    }
}
