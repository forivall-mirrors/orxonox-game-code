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

/**
@file
@brief
    Implementation of an partial interface to Ogre.
*/

#include "GraphicsManager.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

#include <OgreCompositorManager.h>
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

#include "SpecialConfig.h"
#include "util/Exception.h"
#include "util/StringUtils.h"
#include "util/SubString.h"
#include "core/Clock.h"
#include "core/ConsoleCommand.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/Core.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "tools/ParticleInterface.h"
#include "interfaces/WindowEventListener.h"

// HACK!
#include "overlays/map/Map.h"

namespace orxonox
{
    using boost::shared_ptr;

    class _OrxonoxExport OgreWindowEventListener : public Ogre::WindowEventListener
    {
        void windowResized     (Ogre::RenderWindow* rw);
        void windowFocusChange (Ogre::RenderWindow* rw);
        void windowClosed      (Ogre::RenderWindow* rw);
        //void windowMoved       (Ogre::RenderWindow* rw);
    };

    GraphicsManager* GraphicsManager::singletonRef_s = 0;

    /**
    @brief
        Non-initialising constructor.
    */
    GraphicsManager::GraphicsManager()
        : ogreRoot_(0)
        , ogreLogger_(0)
        , renderWindow_(0)
        , viewport_(0)
        , ogreWindowEventListener_(new OgreWindowEventListener())
    {
        RegisterObject(GraphicsManager);

        assert(singletonRef_s == 0);
        singletonRef_s = this;

        this->setConfigValues();

        // Ogre setup procedure
        setupOgre();

        try
        {
            // load all the required plugins for Ogre
            loadOgrePlugins();
            // read resource declaration file
            this->declareResources();
            // Reads ogre config and creates the render window
            this->loadRenderer();

            // TODO: Spread this
            this->initialiseResources();

            // add console commands
            FunctorMember<GraphicsManager>* functor1 = createFunctor(&GraphicsManager::printScreen);
            functor1->setObject(this);
            ccPrintScreen_ = createConsoleCommand(functor1, "printScreen");
            CommandExecutor::addConsoleCommandShortcut(ccPrintScreen_);
        }
        catch (...)
        {
            // clean up
            delete this->ogreRoot_;
            delete this->ogreLogger_;
            delete this->ogreWindowEventListener_;
            throw;
        }
    }

    /**
    @brief
        Destroys all the Ogre related objects
    */
    GraphicsManager::~GraphicsManager()
    {
/*
        delete this->ccPrintScreen_;
*/

        // HACK! This fixes an exit crash
        Map::hackDestroyMap();
        // unload all compositors (this is only necessary because we don't yet destroy all resources!)
        Ogre::CompositorManager::getSingleton().removeAll();

        // Delete OGRE main control organ
        delete this->ogreRoot_;

        // delete the logManager (since we have created it in the first place).
        delete this->ogreLogger_;

        delete this->ogreWindowEventListener_;

        assert(singletonRef_s);
        singletonRef_s = 0;
    }

    void GraphicsManager::setConfigValues()
    {
        SetConfigValue(resourceFile_,    "resources.cfg")
            .description("Location of the resources file in the data path.");
        SetConfigValue(ogreConfigFile_,  "ogre.cfg")
            .description("Location of the Ogre config file");
        SetConfigValue(ogrePluginsFolder_, ORXONOX_OGRE_PLUGINS_FOLDER)
            .description("Folder where the Ogre plugins are located.");
        SetConfigValue(ogrePlugins_, ORXONOX_OGRE_PLUGINS)
            .description("Comma separated list of all plugins to load.");
        SetConfigValue(ogreLogFile_,     "ogre.log")
            .description("Logfile for messages from Ogre. Use \"\" to suppress log file creation.");
        SetConfigValue(ogreLogLevelTrivial_ , 5)
            .description("Corresponding orxonox debug level for ogre Trivial");
        SetConfigValue(ogreLogLevelNormal_  , 4)
            .description("Corresponding orxonox debug level for ogre Normal");
        SetConfigValue(ogreLogLevelCritical_, 2)
            .description("Corresponding orxonox debug level for ogre Critical");
        SetConfigValue(detailLevelParticle_, 2)
            .description("O: off, 1: low, 2: normal, 3: high").callback(this, &GraphicsManager::detailLevelParticleChanged);
    }

    void GraphicsManager::detailLevelParticleChanged()
    {
        for (ObjectList<ParticleInterface>::iterator it = ObjectList<ParticleInterface>::begin(); it; ++it)
            it->detailLevelChanged(this->detailLevelParticle_);
    }

    void GraphicsManager::update(const Clock& time)
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

        // render
        ogreRoot_->_updateAllRenderTargets();

        // again, just to be sure OGRE works fine
        ogreRoot_->_fireFrameEnded(evt); // note: uses the same time as _fireFrameStarted
    }

    void GraphicsManager::setCamera(Ogre::Camera* camera)
    {
        this->viewport_->setCamera(camera);
    }

    /**
    @brief
        Creates the Ogre Root object and sets up the ogre log.
    */
    void GraphicsManager::setupOgre()
    {
        COUT(3) << "Setting up Ogre..." << std::endl;

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

        boost::filesystem::path ogreConfigFilepath(Core::getConfigPath() / this->ogreConfigFile_);
        boost::filesystem::path ogreLogFilepath(Core::getLogPath() / this->ogreLogFile_);

        // create a new logManager
        // Ogre::Root will detect that we've already created a Log
        std::auto_ptr<Ogre::LogManager> logger(new Ogre::LogManager());
        COUT(4) << "Ogre LogManager created" << std::endl;

        // create our own log that we can listen to
        Ogre::Log *myLog;
        myLog = logger->createLog(ogreLogFilepath.string(), true, false, false);
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
        ogreRoot_ = new Ogre::Root("", ogreConfigFilepath.string(), ogreLogFilepath.string());
        // In case that new Root failed the logger gets destroyed because of the std::auto_ptr
        ogreLogger_ = logger.release();

        COUT(3) << "Ogre set up done." << std::endl;
    }

    void GraphicsManager::loadOgrePlugins()
    {
        // just to make sure the next statement doesn't segfault
        if (ogrePluginsFolder_ == "")
            ogrePluginsFolder_ = ".";

        boost::filesystem::path folder(ogrePluginsFolder_);
        // Do some SubString magic to get the comma separated list of plugins
        SubString plugins(ogrePlugins_, ",", " ", false, '\\', false, '"', false, '(', ')', false, '\0');
        // Use backslash paths on Windows! file_string() already does that though.
        for (unsigned int i = 0; i < plugins.size(); ++i)
            ogreRoot_->loadPlugin((folder / plugins[i]).file_string());
    }

    void GraphicsManager::declareResources()
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
            cf.load((Core::getMediaPath() / resourceFile_).string());
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
                        (Core::getMediaPath() / archName).string(), typeName, secName);
                }
            }
            catch (Ogre::Exception& ex)
            {
                COUT(1) << ex.getFullDescription() << std::endl;
            }
        }
    }

    void GraphicsManager::loadRenderer()
    {
        CCOUT(4) << "Configuring Renderer" << std::endl;

        if (!ogreRoot_->restoreConfig())
            if (!ogreRoot_->showConfigDialog())
                ThrowException(InitialisationFailed, "OGRE graphics configuration dialogue failed.");

        CCOUT(4) << "Creating render window" << std::endl;

        this->renderWindow_ = ogreRoot_->initialise(true, "Orxonox");

        Ogre::WindowEventUtilities::addWindowEventListener(this->renderWindow_, ogreWindowEventListener_);

        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(0);

        // create a full screen default viewport
        this->viewport_ = this->renderWindow_->addViewport(0, 0);
    }

    void GraphicsManager::initialiseResources()
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
    void GraphicsManager::messageLogged(const std::string& message,
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

    void GraphicsManager::printScreen()
    {
        assert(this->renderWindow_);
       
        this->renderWindow_->writeContentsToTimestampedFile(Core::getLogPathString() + "screenShot_", ".jpg");
    }


    /****** OgreWindowEventListener ******/

    void OgreWindowEventListener::windowResized(Ogre::RenderWindow* rw)
    {
        for (ObjectList<orxonox::WindowEventListener>::iterator it
            = ObjectList<orxonox::WindowEventListener>::begin(); it; ++it)
            it->windowResized(rw->getWidth(), rw->getHeight());
    }
    void OgreWindowEventListener::windowFocusChange(Ogre::RenderWindow* rw)
    {
        for (ObjectList<orxonox::WindowEventListener>::iterator it
            = ObjectList<orxonox::WindowEventListener>::begin(); it; ++it)
            it->windowFocusChanged();
    }
    void OgreWindowEventListener::windowClosed(Ogre::RenderWindow* rw)
    {
        Game::getInstance().stop();
    }
}
