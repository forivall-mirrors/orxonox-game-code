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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007, Felix Schulthess
 *
 */

/**
@file
@brief
    Implementation of an partial interface to Ogre.
*/

#include "OrxonoxStableHeaders.h"
#include "GraphicsEngine.h"

#include <fstream>

#include <OgreConfigFile.h>
#include <OgreException.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreTextureManager.h>
#include <OgreViewport.h>

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/Debug.h"
#include "core/CommandExecutor.h"
#include "core/ConsoleCommand.h"
#include "core/Exception.h"

#include "overlays/console/InGameConsole.h"
#include "overlays/OverlayGroup.h"
#include "tools/ParticleInterface.h"
#include "Settings.h"
#include "tools/WindowEventListener.h"

#include "objects/CameraHandler.h"

namespace orxonox
{
    GraphicsEngine* GraphicsEngine::singletonRef_s = 0;

    /**
    @brief
        Returns the singleton instance.
    @return
        The only instance of GraphicsEngine.
    */
    /*static*/ GraphicsEngine& GraphicsEngine::getSingleton()
    {
        assert(singletonRef_s);
        return *singletonRef_s;
    }

    /**
    @brief
        Non-initialising constructor.
    */
    GraphicsEngine::GraphicsEngine()
        : root_(0)
        , renderWindow_(0)
        , levelSceneManager_(0)
        , levelViewport_(0)
    {
        RegisterObject(GraphicsEngine);

        singletonRef_s = this;

        this->detailLevelParticle_ = 0;

        this->setConfigValues();
        CCOUT(4) << "Constructed" << std::endl;
    }

    void GraphicsEngine::setConfigValues()
    {
        SetConfigValue(resourceFile_,    "resources.cfg").description("Location of the resources file in the data path.");
        SetConfigValue(ogreConfigFile_,  "ogre.cfg").description("Location of the Ogre config file");
        SetConfigValue(ogrePluginsFile_, "plugins.cfg").description("Location of the Ogre plugins file");
        SetConfigValue(ogreLogFile_,     "ogre.log").description("Logfile for messages from Ogre. \
                                                                 Use \"\" to suppress log file creation.");
        SetConfigValue(ogreLogLevelTrivial_ , 5).description("Corresponding orxonox debug level for ogre Trivial");
        SetConfigValue(ogreLogLevelNormal_  , 4).description("Corresponding orxonox debug level for ogre Normal");
        SetConfigValue(ogreLogLevelCritical_, 2).description("Corresponding orxonox debug level for ogre Critical");

        unsigned int old = this->detailLevelParticle_;
        SetConfigValue(detailLevelParticle_, 2).description("O: off, 1: low, 2: normal, 3: high");

        if (this->detailLevelParticle_ != old)
            for (Iterator<ParticleInterface> it = ObjectList<ParticleInterface>::begin(); it; ++it)
                it->detailLevelChanged(this->detailLevelParticle_);
    }

    /**
    @brief
        Destroys all the Ogre related objects
    */
    GraphicsEngine::~GraphicsEngine()
    {
        CCOUT(4) << "Destroying objects..." << std::endl;
        Ogre::WindowEventUtilities::removeWindowEventListener(this->renderWindow_, this);
        if (this->root_)
            delete this->root_;
        this->root_ = 0;
        this->levelSceneManager_ = 0;
        this->renderWindow_ = 0;

#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
        // delete the ogre log and the logManager (since we have created it).
        if (Ogre::LogManager::getSingletonPtr() != 0)
        {
            Ogre::LogManager::getSingleton().getDefaultLog()->removeListener(this);
            Ogre::LogManager::getSingleton().destroyLog(Ogre::LogManager::getSingleton().getDefaultLog());
            delete Ogre::LogManager::getSingletonPtr();
        }
        CCOUT(4) << "Destroying objects done" << std::endl;
#endif

        singletonRef_s = 0;
    }

    /**
    @brief
        Creates the Ogre Root object and sets up the ogre log.
    */
    void GraphicsEngine::setup()
    {
        CCOUT(3) << "Setting up..." << std::endl;

        // TODO: LogManager doesn't work on linux platform. The why is yet unknown.
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
        // create a new logManager
        Ogre::LogManager* logger = new Ogre::LogManager();
        CCOUT(4) << "Ogre LogManager created" << std::endl;

        // create our own log that we can listen to
        Ogre::Log *myLog;
        if (this->ogreLogFile_ == "")
            myLog = logger->createLog("ogre.log", true, false, true);
        else
            myLog = logger->createLog(this->ogreLogFile_, true, false, false);
        CCOUT(4) << "Ogre Log created" << std::endl;

        myLog->setLogDetail(Ogre::LL_BOREME);
        myLog->addListener(this);
#endif

        // Root will detect that we've already created a Log
        CCOUT(4) << "Creating Ogre Root..." << std::endl;

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

        root_ = new Ogre::Root(ogrePluginsFile_, ogreConfigFile_, ogreLogFile_);

        if (!root_->getInstalledPlugins().size())
        {
            ThrowException(PluginsNotFound, "No Ogre plugins declared. Cannot load Ogre.");
        }

#if 0 // Ogre 1.4.3 doesn't support setDebugOutputEnabled(.)
//#if ORXONOX_PLATFORM != ORXONOX_PLATFORM_WIN32
        // tame the ogre ouput so we don't get all the mess in the console
        Ogre::Log* defaultLog = Ogre::LogManager::getSingleton().getDefaultLog();
        defaultLog->setDebugOutputEnabled(false);
        defaultLog->setLogDetail(Ogre::LL_BOREME);
        defaultLog->addListener(this);
#endif

        CCOUT(4) << "Creating Ogre Root done" << std::endl;

        // specify where Ogre has to look for resources. This call doesn't parse anything yet!
        declareRessourceLocations();

        CCOUT(3) << "Set up done." << std::endl;
    }

    void GraphicsEngine::declareRessourceLocations()
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
        catch (Ogre::Exception& ex)
        {
            COUT(1) << ex.getFullDescription() << std::endl;
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

    bool GraphicsEngine::loadRenderer()
    {
        CCOUT(4) << "Configuring Renderer" << std::endl;

        // check for file existence because Ogre displays exceptions if not
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

        if (!root_->restoreConfig())
            if (!root_->showConfigDialog())
                return false;

        CCOUT(4) << "Creating render window" << std::endl;
        try
        {
            this->renderWindow_ = root_->initialise(true, "OrxonoxV2");
        }
        catch (...)
        {
            COUT(2) << "Error: There was an exception when initialising Ogre Root." << std::endl;
            return false;
        }

        if (!root_->isInitialised())
        {
            CCOUT(2) << "Error: Initialising Ogre root object failed." << std::endl;
            return false;
        }
        Ogre::WindowEventUtilities::addWindowEventListener(this->renderWindow_, this);
        //Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

        // create a full screen viewport in which to render the scene and the loading screen
        // The GUI uses its own one to be able to render on top of everything.
        // That explains why the Z order here is only 1 and not 0 (top most)
        this->levelViewport_ = this->renderWindow_->addViewport(0, 1);

        return true;
    }

    bool GraphicsEngine::initialiseResources()
    {
        CCOUT(4) << "Initialising resources" << std::endl;
        //TODO: Do NOT load all the groups, why are we doing that? And do we really do that? initialise != load...
        try
        {
            Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
            /*Ogre::StringVector str = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
            for (unsigned int i = 0; i < str.size(); i++)
            {
            Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(str[i]);
            }*/
        }
        catch (Ogre::Exception& e)
        {
            CCOUT(2) << "Error: There was an Error when initialising the resources." << std::endl;
            CCOUT(2) << "ErrorMessage: " << e.getFullDescription() << std::endl;
            return false;
        }
        return true;
    }

    /**
    @brief
        Creates the SceneManager
    */
    bool GraphicsEngine::createNewScene()
    {
        CCOUT(4) << "Creating new SceneManager..." << std::endl;
        if (levelSceneManager_)
        {
            CCOUT(2) << "SceneManager already exists! Skipping." << std::endl;
            return false;
        }
        this->levelSceneManager_ = this->root_->createSceneManager(Ogre::ST_GENERIC, "LevelSceneManager");
        CCOUT(3) << "Created SceneManager: " << levelSceneManager_ << std::endl;
        return true;
    }

    /**
    @brief
        Returns the window handle of the render window.
        At least the InputHandler uses this to create the OIS::InputManager
    @return
        The window handle of the render window
    */
    size_t GraphicsEngine::getWindowHandle()
    {
        if (this->renderWindow_)
        {
            size_t windowHnd = 0;
            this->renderWindow_->getCustomAttribute("WINDOW", &windowHnd);
            return windowHnd;
        }
        else
            return 0;
    }

    /**
    @brief
        Get the width of the render window
    @return
        The width of the render window
    */
    int GraphicsEngine::getWindowWidth() const
    {
        if (this->renderWindow_)
            return this->renderWindow_->getWidth();
        else
            return 0;
    }

    /**
    @brief
        Get the height of the render window
    @return
        The height of the render window
    */
    int GraphicsEngine::getWindowHeight() const
    {
        if (this->renderWindow_)
            return this->renderWindow_->getHeight();
        else
            return 0;
    }

    /**
    @brief
        Returns the window aspect ratio height/width.
    @return
        The window aspect ratio
    */
    float GraphicsEngine::getWindowAspectRatio() const
    {
        if (this->renderWindow_)
            return (float)this->renderWindow_->getHeight() / (float)this->renderWindow_->getWidth();
        else
            return 1.0f;
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
    void GraphicsEngine::messageLogged(const std::string& message,
        Ogre::LogMessageLevel lml, bool maskDebug, const std::string &logName)
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
    void GraphicsEngine::windowMoved(Ogre::RenderWindow *rw)
    {
        for (Iterator<orxonox::WindowEventListener> it = ObjectList<orxonox::WindowEventListener>::start(); it; ++it)
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
    void GraphicsEngine::windowResized(Ogre::RenderWindow *rw)
    {
        for (Iterator<orxonox::WindowEventListener> it = ObjectList<orxonox::WindowEventListener>::start(); it; ++it)
            it->windowResized(this->renderWindow_->getWidth(), this->renderWindow_->getHeight());
    }

    /**
    @brief
        Window focus has changed.
    @param rw
        The render window it occured in
    */
    void GraphicsEngine::windowFocusChanged(Ogre::RenderWindow *rw)
    {
        for (Iterator<orxonox::WindowEventListener> it = ObjectList<orxonox::WindowEventListener>::start(); it; ++it)
            it->windowFocusChanged();
    }

    /**
    @brief
        Window was closed.
    @param rw
        The render window it occured in
    */
    void GraphicsEngine::windowClosed(Ogre::RenderWindow *rw)
    {
        // using CommandExecutor in order to avoid depending on Orxonox.h.
        CommandExecutor::execute("exit", false);
    }

}
