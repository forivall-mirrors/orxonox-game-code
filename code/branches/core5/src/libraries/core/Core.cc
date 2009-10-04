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
#include "util/SignalHandler.h"
#include "PathConfig.h"
#include "CommandExecutor.h"
#include "CommandLine.h"
#include "ConfigFileManager.h"
#include "ConfigValueIncludes.h"
#include "CoreIncludes.h"
#include "DynLibManager.h"
#include "GameMode.h"
#include "GraphicsManager.h"
#include "GUIManager.h"
#include "Identifier.h"
#include "Language.h"
#include "LuaState.h"
#include "ScopedSingletonManager.h"
#include "Shell.h"
#include "TclBind.h"
#include "TclThreadManager.h"
#include "input/InputManager.h"

namespace orxonox
{
    //! Static pointer to the singleton
    Core* Core::singletonPtr_s  = 0;

    SetCommandLineArgument(settingsFile, "orxonox.ini").information("THE configuration file");
#ifdef ORXONOX_PLATFORM_WINDOWS
    SetCommandLineArgument(limitToCPU, 0).information("Limits the program to one cpu/core (1, 2, 3, etc.). 0 turns it off (default)");
#endif

    /**
    @brief
        Helper class for the Core singleton: we cannot derive
        Core from OrxonoxClass because we need to handle the Identifier
        destruction in the Core destructor.
    */
    class CoreConfiguration : public OrxonoxClass
    {
    public:
        CoreConfiguration()
        {
        }

        void initialise()
        {
            RegisterRootObject(CoreConfiguration);
            this->setConfigValues();
        }

        /**
            @brief Function to collect the SetConfigValue-macro calls.
        */
        void setConfigValues()
        {
#ifdef NDEBUG
            const unsigned int defaultLevelConsole = 1;
            const unsigned int defaultLevelLogfile = 3;
            const unsigned int defaultLevelShell   = 1;
#else
            const unsigned int defaultLevelConsole = 3;
            const unsigned int defaultLevelLogfile = 4;
            const unsigned int defaultLevelShell   = 3;
#endif
            SetConfigValue(softDebugLevelConsole_, defaultLevelConsole)
                .description("The maximal level of debug output shown in the console")
                .callback(this, &CoreConfiguration::debugLevelChanged);
            SetConfigValue(softDebugLevelLogfile_, defaultLevelLogfile)
                .description("The maximal level of debug output shown in the logfile")
                .callback(this, &CoreConfiguration::debugLevelChanged);
            SetConfigValue(softDebugLevelShell_, defaultLevelShell)
                .description("The maximal level of debug output shown in the ingame shell")
                .callback(this, &CoreConfiguration::debugLevelChanged);

            SetConfigValue(language_, Language::getInstance().defaultLanguage_)
                .description("The language of the ingame text")
                .callback(this, &CoreConfiguration::languageChanged);
            SetConfigValue(bInitializeRandomNumberGenerator_, true)
                .description("If true, all random actions are different each time you start the game")
                .callback(this, &CoreConfiguration::initializeRandomNumberGenerator);
        }

        /**
            @brief Callback function if the debug level has changed.
        */
        void debugLevelChanged()
        {
            // softDebugLevel_ is the maximum of the 3 variables
            this->softDebugLevel_ = this->softDebugLevelConsole_;
            if (this->softDebugLevelLogfile_ > this->softDebugLevel_)
                this->softDebugLevel_ = this->softDebugLevelLogfile_;
            if (this->softDebugLevelShell_ > this->softDebugLevel_)
                this->softDebugLevel_ = this->softDebugLevelShell_;

            OutputHandler::setSoftDebugLevel(OutputHandler::LD_All,     this->softDebugLevel_);
            OutputHandler::setSoftDebugLevel(OutputHandler::LD_Console, this->softDebugLevelConsole_);
            OutputHandler::setSoftDebugLevel(OutputHandler::LD_Logfile, this->softDebugLevelLogfile_);
            OutputHandler::setSoftDebugLevel(OutputHandler::LD_Shell,   this->softDebugLevelShell_);
        }

        /**
            @brief Callback function if the language has changed.
        */
        void languageChanged()
        {
            // Read the translation file after the language was configured
            Language::getInstance().readTranslatedLanguageFile();
        }

        /**
            @brief Sets the language in the config-file back to the default.
        */
        void resetLanguage()
        {
            ResetConfigValue(language_);
        }

        void initializeRandomNumberGenerator()
        {
            static bool bInitialized = false;
            if (!bInitialized && this->bInitializeRandomNumberGenerator_)
            {
                srand(static_cast<unsigned int>(time(0)));
                rand();
                bInitialized = true;
            }
        }

        int softDebugLevel_;                            //!< The debug level
        int softDebugLevelConsole_;                     //!< The debug level for the console
        int softDebugLevelLogfile_;                     //!< The debug level for the logfile
        int softDebugLevelShell_;                       //!< The debug level for the ingame shell
        std::string language_;                          //!< The language
        bool bInitializeRandomNumberGenerator_;         //!< If true, srand(time(0)) is called
    };


    Core::Core(const std::string& cmdLine)
        // Cleanup guard for identifier destruction (incl. XMLPort, configValues, consoleCommands)
        : identifierDestroyer_(Identifier::destroyAllIdentifiers)
        // Cleanup guard for external console commands that don't belong to an Identifier
        , consoleCommandDestroyer_(CommandExecutor::destroyExternalCommands)
        , configuration_(new CoreConfiguration()) // Don't yet create config values!
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
        CommandLine::parseCommandLine(cmdLine);

        // Set configurable paths like log, config and media
        this->pathConfig_->setConfigurablePaths();

        // create a signal handler (only active for linux)
        // This call is placed as soon as possible, but after the directories are set
        this->signalHandler_.reset(new SignalHandler());
        this->signalHandler_->doCatch(PathConfig::getExecutablePathString(), PathConfig::getLogPathString() + "orxonox_crash.log");

        // Set the correct log path. Before this call, /tmp (Unix) or %TEMP% was used
        OutputHandler::getOutStream().setLogPath(PathConfig::getLogPathString());

        // Parse additional options file now that we know its path
        CommandLine::parseFile();

#ifdef ORXONOX_PLATFORM_WINDOWS
        // limit the main thread to the first core so that QueryPerformanceCounter doesn't jump
        // do this after ogre has initialised. Somehow Ogre changes the settings again (not through
        // the timer though).
        int limitToCPU = CommandLine::getValue("limitToCPU");
        if (limitToCPU > 0)
            setThreadAffinity(static_cast<unsigned int>(limitToCPU));
#endif

        // Manage ini files and set the default settings file (usually orxonox.ini)
        this->configFileManager_.reset(new ConfigFileManager());
        this->configFileManager_->setFilename(ConfigFileType::Settings,
            CommandLine::getValue("settingsFile").getString());

        // Required as well for the config values
        this->languageInstance_.reset(new Language());

        // creates the class hierarchy for all classes with factories
        Identifier::createClassHierarchy();

        // Do this soon after the ConfigFileManager has been created to open up the
        // possibility to configure everything below here
        this->configuration_->initialise();

        // Load OGRE excluding the renderer and the render window
        this->graphicsManager_.reset(new GraphicsManager(false));

        // initialise Tcl
        this->tclBind_.reset(new TclBind(PathConfig::getDataPathString()));
        this->tclThreadManager_.reset(new TclThreadManager(tclBind_->getTclInterpreter()));

        // create a shell
        this->shell_.reset(new Shell());

        // Create singletons that always exist (in other libraries)
        this->rootScope_.reset(new Scope<ScopeID::Root>());
    }

    /**
    @brief
        All destruction code is handled by scoped_ptrs and ScopeGuards.
    */
    Core::~Core()
    {
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

    /**
        @brief Returns the softDebugLevel for the given device (returns a default-value if the class is right about to be created).
        @param device The device
        @return The softDebugLevel
    */
    /*static*/ int Core::getSoftDebugLevel(OutputHandler::OutputDevice device)
    {
        switch (device)
        {
        case OutputHandler::LD_All:
            return Core::getInstance().configuration_->softDebugLevel_;
        case OutputHandler::LD_Console:
            return Core::getInstance().configuration_->softDebugLevelConsole_;
        case OutputHandler::LD_Logfile:
            return Core::getInstance().configuration_->softDebugLevelLogfile_;
        case OutputHandler::LD_Shell:
            return Core::getInstance().configuration_->softDebugLevelShell_;
        default:
            assert(0);
            return 2;
        }
    }

     /**
        @brief Sets the softDebugLevel for the given device. Please use this only temporary and restore the value afterwards, as it overrides the configured value.
        @param device The device
        @param level The level
    */
    /*static*/ void Core::setSoftDebugLevel(OutputHandler::OutputDevice device, int level)
    {
        if (device == OutputHandler::LD_All)
            Core::getInstance().configuration_->softDebugLevel_ = level;
        else if (device == OutputHandler::LD_Console)
            Core::getInstance().configuration_->softDebugLevelConsole_ = level;
        else if (device == OutputHandler::LD_Logfile)
            Core::getInstance().configuration_->softDebugLevelLogfile_ = level;
        else if (device == OutputHandler::LD_Shell)
            Core::getInstance().configuration_->softDebugLevelShell_ = level;

        OutputHandler::setSoftDebugLevel(device, level);
    }

    /**
        @brief Returns the configured language.
    */
    /*static*/ const std::string& Core::getLanguage()
    {
        return Core::getInstance().configuration_->language_;
    }

    /**
        @brief Sets the language in the config-file back to the default.
    */
    /*static*/ void Core::resetLanguage()
    {
        Core::getInstance().configuration_->resetLanguage();
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
        // singletons from other libraries
        ScopedSingletonManager::update<ScopeID::Root>(time);
        if (this->bGraphicsLoaded_)
        {
            // process input events
            this->inputManager_->update(time);
            // process gui events
            this->guiManager_->update(time);
            // graphics singletons from other libraries
            ScopedSingletonManager::update<ScopeID::Graphics>(time);
        }
        // process thread commands
        this->tclThreadManager_->update(time);
    }

    void Core::postUpdate(const Clock& time)
    {
        if (this->bGraphicsLoaded_)
        {
            // Render (doesn't throw)
            this->graphicsManager_->update(time);
        }
    }
}
