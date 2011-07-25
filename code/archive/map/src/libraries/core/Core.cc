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
 *      Fabian 'x3n' Landau
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief
    Implementation of the Core singleton with its global variables (avoids boost include)
*/

#include "Core.h"

#include <cassert>
#include <vector>

#ifdef ORXONOX_PLATFORM_WINDOWS
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#  undef min
#  undef max
#endif

#include "util/Clock.h"
#include "util/Debug.h"
#include "util/Exception.h"
#include "util/Scope.h"
#include "util/SignalHandler.h"
#include "PathConfig.h"
#include "CommandExecutor.h"
#include "CommandLineParser.h"
#include "ConfigFileManager.h"
#include "ConfigValueIncludes.h"
#include "CoreIncludes.h"
#include "DynLibManager.h"
#include "GameMode.h"
#include "GraphicsManager.h"
#include "GUIManager.h"
#include "Identifier.h"
#include "Language.h"
#include "IOConsole.h"
#include "LuaState.h"
#include "ScopedSingletonManager.h"
#include "TclBind.h"
#include "TclThreadManager.h"
#include "input/InputManager.h"

namespace orxonox
{
    //! Static pointer to the singleton
    Core* Core::singletonPtr_s  = 0;

    SetCommandLineArgument(settingsFile, "orxonox.ini").information("THE configuration file");
#ifdef ORXONOX_PLATFORM_WINDOWS
    SetCommandLineArgument(limitToCPU, 1).information("Limits the program to one CPU/core (1, 2, 3, etc.). Default is the first core (faster than off)");
#endif

    Core::Core(const std::string& cmdLine)
        // Cleanup guard for identifier destruction (incl. XMLPort, configValues, consoleCommands)
        : identifierDestroyer_(Identifier::destroyAllIdentifiers)
        // Cleanup guard for external console commands that don't belong to an Identifier
        , consoleCommandDestroyer_(CommandExecutor::destroyExternalCommands)
        , bGraphicsLoaded_(false)
    {
        // Set the hard coded fixed paths
        this->pathConfig_.reset(new PathConfig());

        // Create a new dynamic library manager
        this->dynLibManager_.reset(new DynLibManager());

        // Load modules
        const std::vector<std::string>& modulePaths = this->pathConfig_->getModulePaths();
        for (std::vector<std::string>::const_iterator it = modulePaths.begin(); it != modulePaths.end(); ++it)
        {
            try
            {
                this->dynLibManager_->load(*it);
            }
            catch (...)
            {
                COUT(1) << "Couldn't load module \"" << *it << "\": " << Exception::handleMessage() << std::endl;
            }
        }

        // Parse command line arguments AFTER the modules have been loaded (static code!)
        CommandLineParser::parseCommandLine(cmdLine);

        // Set configurable paths like log, config and media
        this->pathConfig_->setConfigurablePaths();

        // create a signal handler (only active for Linux)
        // This call is placed as soon as possible, but after the directories are set
        this->signalHandler_.reset(new SignalHandler());
        this->signalHandler_->doCatch(PathConfig::getExecutablePathString(), PathConfig::getLogPathString() + "orxonox_crash.log");

        // Set the correct log path. Before this call, /tmp (Unix) or %TEMP% (Windows) was used
        OutputHandler::getInstance().setLogPath(PathConfig::getLogPathString());

        // Parse additional options file now that we know its path
        CommandLineParser::parseFile();

#ifdef ORXONOX_PLATFORM_WINDOWS
        // limit the main thread to the first core so that QueryPerformanceCounter doesn't jump
        // do this after ogre has initialised. Somehow Ogre changes the settings again (not through
        // the timer though).
        int limitToCPU = CommandLineParser::getValue("limitToCPU");
        if (limitToCPU > 0)
            setThreadAffinity(static_cast<unsigned int>(limitToCPU));
#endif

        // Manage ini files and set the default settings file (usually orxonox.ini)
        this->configFileManager_.reset(new ConfigFileManager());
        this->configFileManager_->setFilename(ConfigFileType::Settings,
            CommandLineParser::getValue("settingsFile").getString());

        // Required as well for the config values
        this->languageInstance_.reset(new Language());

        // Do this soon after the ConfigFileManager has been created to open up the
        // possibility to configure everything below here
        ClassIdentifier<Core>::getIdentifier("Core")->initialiseObject(this, "Core", true);
        this->setConfigValues();

        // create persistent io console
        this->ioConsole_.reset(new IOConsole());

        // creates the class hierarchy for all classes with factories
        Identifier::createClassHierarchy();

        // Load OGRE excluding the renderer and the render window
        this->graphicsManager_.reset(new GraphicsManager(false));

        // initialise Tcl
        this->tclBind_.reset(new TclBind(PathConfig::getDataPathString()));
        this->tclThreadManager_.reset(new TclThreadManager(tclBind_->getTclInterpreter()));

        // Create singletons that always exist (in other libraries)
        this->rootScope_.reset(new Scope<ScopeID::Root>());
    }

    /**
    @brief
        All destruction code is handled by scoped_ptrs and ScopeGuards.
    */
    Core::~Core()
    {
        // Remove us from the object lists again to avoid problems when destroying them
        this->unregisterObject();
    }

    //! Function to collect the SetConfigValue-macro calls.
    void Core::setConfigValues()
    {
#ifdef ORXONOX_RELEASE
        const unsigned int defaultLevelLogFile = 3;
#else
        const unsigned int defaultLevelLogFile = 4;
#endif
        setConfigValueGeneric(this, &this->softDebugLevelLogFile_, ConfigFileType::Settings, "OutputHandler", "softDebugLevelLogFile", defaultLevelLogFile)
            .description("The maximum level of debug output shown in the log file");
        OutputHandler::getInstance().setSoftDebugLevel(OutputHandler::logFileOutputListenerName_s, this->softDebugLevelLogFile_);

        SetConfigValue(language_, Language::getInstance().defaultLanguage_)
            .description("The language of the in game text")
            .callback(this, &Core::languageChanged);
        SetConfigValue(bInitRandomNumberGenerator_, true)
            .description("If true, all random actions are different each time you start the game")
            .callback(this, &Core::initRandomNumberGenerator);
    }

    //! Callback function if the language has changed.
    void Core::languageChanged()
    {
        // Read the translation file after the language was configured
        Language::getInstance().readTranslatedLanguageFile();
    }

    void Core::initRandomNumberGenerator()
    {
        static bool bInitialized = false;
        if (!bInitialized && this->bInitRandomNumberGenerator_)
        {
            srand(static_cast<unsigned int>(time(0)));
            rand();
            bInitialized = true;
        }
    }

    void Core::loadGraphics()
    {
        // Any exception should trigger this, even in upgradeToGraphics (see its remarks)
        Loki::ScopeGuard unloader = Loki::MakeObjGuard(*this, &Core::unloadGraphics);

        // Upgrade OGRE to receive a render window
        graphicsManager_->upgradeToGraphics();

        // Calls the InputManager which sets up the input devices.
        inputManager_.reset(new InputManager());

        // Load the CEGUI interface
        guiManager_.reset(new GUIManager(graphicsManager_->getRenderWindow(),
            inputManager_->getMousePosition(), graphicsManager_->isFullScreen()));

        bGraphicsLoaded_ = true;
        GameMode::bShowsGraphics_s = true;

        // Load some sort of a debug overlay (only denoted by its name, "debug.oxo")
        graphicsManager_->loadDebugOverlay();

        // Create singletons associated with graphics (in other libraries)
        graphicsScope_.reset(new Scope<ScopeID::Graphics>());

        unloader.Dismiss();
    }

    void Core::unloadGraphics()
    {
        this->graphicsScope_.reset();
        this->guiManager_.reset();
        this->inputManager_.reset();
        this->graphicsManager_.reset();

        // Load Ogre::Root again, but without the render system
        try
            { this->graphicsManager_.reset(new GraphicsManager(false)); }
        catch (...)
        {
            COUT(0) << "An exception occurred during 'unloadGraphics':" << Exception::handleMessage() << std::endl
                    << "Another exception might be being handled which may lead to undefined behaviour!" << std::endl
                    << "Terminating the program." << std::endl;
            abort();
        }

        bGraphicsLoaded_ = false;
        GameMode::bShowsGraphics_s = false;
    }

    //! Sets the language in the config-file back to the default.
    void Core::resetLanguage()
    {
        ResetConfigValue(language_);
    }

    /**
    @note
        The code of this function has been copied and adjusted from OGRE, an open source graphics engine.
            (Object-oriented Graphics Rendering Engine)
        For the latest info, see http://www.ogre3d.org/

        Copyright (c) 2000-2008 Torus Knot Software Ltd

        OGRE is licensed under the LGPL. For more info, see OGRE license.
    */
    void Core::setThreadAffinity(int limitToCPU)
    {
#ifdef ORXONOX_PLATFORM_WINDOWS

        if (limitToCPU <= 0)
            return;

        unsigned int coreNr = limitToCPU - 1;
        // Get the current process core mask
        DWORD procMask;
        DWORD sysMask;
#  if _MSC_VER >= 1400 && defined (_M_X64)
        GetProcessAffinityMask(GetCurrentProcess(), (PDWORD_PTR)&procMask, (PDWORD_PTR)&sysMask);
#  else
        GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);
#  endif

        // If procMask is 0, consider there is only one core available
        // (using 0 as procMask will cause an infinite loop below)
        if (procMask == 0)
            procMask = 1;

        // if the core specified with coreNr is not available, take the lowest one
        if (!(procMask & (1 << coreNr)))
            coreNr = 0;

        // Find the lowest core that this process uses and coreNr suggests
        DWORD threadMask = 1;
        while ((threadMask & procMask) == 0 || (threadMask < (1u << coreNr)))
            threadMask <<= 1;

        // Set affinity to the first core
        SetThreadAffinityMask(GetCurrentThread(), threadMask);
#endif
    }

    void Core::preUpdate(const Clock& time)
    {
        // Update singletons before general ticking
        ScopedSingletonManager::preUpdate<ScopeID::Root>(time);
        if (this->bGraphicsLoaded_)
        {
            // Process input events
            this->inputManager_->preUpdate(time);
            // Update GUI
            this->guiManager_->preUpdate(time);
            // Update singletons before general ticking
            ScopedSingletonManager::preUpdate<ScopeID::Graphics>(time);
        }
        // Process console events and status line
        this->ioConsole_->preUpdate(time);
        // Process thread commands
        this->tclThreadManager_->preUpdate(time);
    }

    void Core::postUpdate(const Clock& time)
    {
        // Update singletons just before rendering
        ScopedSingletonManager::postUpdate<ScopeID::Root>(time);
        if (this->bGraphicsLoaded_)
        {
            // Update singletons just before rendering
            ScopedSingletonManager::postUpdate<ScopeID::Graphics>(time);
            // Render (doesn't throw)
            this->graphicsManager_->postUpdate(time);
        }
    }
}
