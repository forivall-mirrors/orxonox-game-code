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
#include "GSRoot.h"

#include <OgreLogManager.h>
#include <OgreRoot.h>

#include "util/Exception.h"
#include "util/Debug.h"
#include "core/Factory.h"
#include "core/ConfigFileManager.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/CommandLine.h"
#include "core/Shell.h"
#include "core/TclBind.h"
#include "core/TclThreadManager.h"
#include "GraphicsEngine.h"
#include "Settings.h"

#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32 
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include "windows.h"

   //Get around Windows hackery
#  ifdef max
#    undef max
#  endif
#  ifdef min
#    undef min
#  endif
#endif

namespace orxonox
{
    SetCommandLineArgument(dataPath, "").setInformation("PATH");
    SetCommandLineArgument(limitToCPU, 1).setInformation("0: off | #cpu");

    GSRoot::GSRoot()
        : RootGameState("root")
        , settings_(0)
        , ogreRoot_(0)
        , ogreLogger_(0)
        , graphicsEngine_(0)
        , tclBind_(0)
        , tclThreadManager_(0)
        , shell_(0)
    {
        RegisterRootObject(GSRoot);
    }

    GSRoot::~GSRoot()
    {
    }

    void GSRoot::setConfigValues()
    {
        SetConfigValue(ogreConfigFile_,  "ogre.cfg").description("Location of the Ogre config file");
        SetConfigValue(ogrePluginsFile_, "plugins.cfg").description("Location of the Ogre plugins file");
        SetConfigValue(ogreLogFile_,     "ogre.log").description("Logfile for messages from Ogre. \
                                                                 Use \"\" to suppress log file creation.");
        SetConfigValue(ogreLogLevelTrivial_ , 5).description("Corresponding orxonox debug level for ogre Trivial");
        SetConfigValue(ogreLogLevelNormal_  , 4).description("Corresponding orxonox debug level for ogre Normal");
        SetConfigValue(ogreLogLevelCritical_, 2).description("Corresponding orxonox debug level for ogre Critical");
    }

    void GSRoot::enter()
    {
#if ORXONOX_DEBUG_MODE == 1
        ConfigFileManager::getInstance()->setFile(CFT_Settings, "orxonox_d.ini");
#else
        ConfigFileManager::getInstance()->setFile(CFT_Settings, "orxonox.ini");
#endif

        // do this after the previous call..
        setConfigValues();

        // creates the class hierarchy for all classes with factories
        Factory::createClassHierarchy();

        // instantiate Settings class
        this->settings_ = new Settings();

        std::string dataPath;
        CommandLine::getValue("dataPath", &dataPath);
        if (dataPath != "")
        {
            if (*dataPath.end() != '/' && *dataPath.end() != '\\')
                Settings::tsetDataPath(dataPath + "/");
            else
                Settings::tsetDataPath(dataPath);
        }

        // initialise TCL
        this->tclBind_ = new TclBind(Settings::getDataPath());
        this->tclThreadManager_ = new TclThreadManager(tclBind_->getTclInterpreter());

        // create a shell
        this->shell_ = new Shell();

        setupOgre();

        // initialise graphics engine. Doesn't load the render window yet!
        graphicsEngine_ = new GraphicsEngine();

        // limit the main thread to the first core so that QueryPerformanceCounter doesn't jump
        // do this after ogre has initialised. Somehow Ogre changes the settings again (not through
        // the timer though).
        int limitToCPU;
        CommandLine::getValue("limitToCPU", &limitToCPU);
        if (limitToCPU > 0)
            setThreadAffinity((unsigned int)(limitToCPU - 1));

        // add console commands
        FunctorMember<GSRoot>* functor1 = createFunctor(&GSRoot::exitGame);
        functor1->setObject(this);
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor1, "exit"));

        // add console commands
        FunctorMember01<GameStateBase, const std::string&>* functor2 = createFunctor(&GameStateBase::requestState);
        functor2->setObject(this);
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor2, "selectGameState"));
    }

    void GSRoot::leave()
    {
        // TODO: remove and destroy console commands

        delete graphicsEngine_;

        delete this->ogreRoot_;

#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
        // delete the ogre log and the logManager (since we have created it).
        this->ogreLogger_->getDefaultLog()->removeListener(this);
        this->ogreLogger_->destroyLog(Ogre::LogManager::getSingleton().getDefaultLog());
        delete this->ogreLogger_;
#endif

        delete this->shell_;
        delete this->tclThreadManager_;
        delete this->tclBind_;

        delete settings_;

    }

    void GSRoot::ticked(const Clock& time)
    {
        TclThreadManager::getInstance().tick(time.getDeltaTime());

        this->tickChild(time);
    }

    /**
    @note
        The code of this function has been copied and adjusted from OGRE, an open source graphics engine.
            (Object-oriented Graphics Rendering Engine)
        For the latest info, see http://www.ogre3d.org/

        Copyright (c) 2000-2008 Torus Knot Software Ltd
        
        OGRE is licensed under the LGPL. For more info, see ogre license info.
    */
    void GSRoot::setThreadAffinity(unsigned int limitToCPU)
    {
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
        // Get the current process core mask
	    DWORD procMask;
	    DWORD sysMask;
#if _MSC_VER >= 1400 && defined (_M_X64)
	    GetProcessAffinityMask(GetCurrentProcess(), (PDWORD_PTR)&procMask, (PDWORD_PTR)&sysMask);
#else
	    GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);
#endif

	    // If procMask is 0, consider there is only one core available
	    // (using 0 as procMask will cause an infinite loop below)
	    if (procMask == 0)
		    procMask = 1;

        // if the core specified with limitToCPU is not available, take the lowest one
        if (!(procMask & (1 << limitToCPU)))
            limitToCPU = 0;

	    // Find the lowest core that this process uses and limitToCPU suggests
        DWORD threadMask = 1;
	    while ((threadMask & procMask) == 0 || (threadMask < (1u << limitToCPU)))
		    threadMask <<= 1;

	    // Set affinity to the first core
	    SetThreadAffinityMask(GetCurrentThread(), threadMask);
#endif
    }

    /**
    @brief
        Creates the Ogre Root object and sets up the ogre log.
    */
    void GSRoot::setupOgre()
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
    void GSRoot::messageLogged(const std::string& message,
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
}
