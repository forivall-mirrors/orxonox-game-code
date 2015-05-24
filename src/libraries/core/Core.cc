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
#include <cstdlib>
#include <ctime>
#include <fstream>
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
#include "util/Output.h"
#include "util/Exception.h"
#include "util/output/LogWriter.h"
#include "util/output/OutputManager.h"
#include "core/singleton/Scope.h"
#include "core/singleton/ScopedSingletonWrapper.h"
#include "util/SignalHandler.h"
#include "PathConfig.h"
#include "commandline/CommandLineIncludes.h"
#include "config/ConfigFileManager.h"
#include "config/ConfigValueIncludes.h"
#include "CoreIncludes.h"
#include "DynLibManager.h"
#include "GameMode.h"
#include "GraphicsManager.h"
#include "GUIManager.h"
#include "class/Identifier.h"
#include "Language.h"
#include "Loader.h"
#include "LuaState.h"
#include "command/ConsoleCommandManager.h"
#include "command/IOConsole.h"
#include "command/TclBind.h"
#include "command/TclThreadManager.h"
#include "input/InputManager.h"
#include "object/ObjectList.h"
#include "module/ModuleInstance.h"
#include "UpdateListener.h"

namespace orxonox
{
    //! Static pointer to the singleton
    Core* Core::singletonPtr_s  = 0;

    SetCommandLineArgument(settingsFile, "orxonox.ini").information("THE configuration file");
#if !defined(ORXONOX_PLATFORM_APPLE) && !defined(ORXONOX_USE_WINMAIN)
    SetCommandLineSwitch(noIOConsole).information("Use this if you don't want to use the IOConsole (for instance for Lua debugging)");
#endif

#ifdef ORXONOX_PLATFORM_WINDOWS
    SetCommandLineArgument(limitToCPU, 0).information("Limits the program to one CPU/core (1, 2, 3, etc.). Default is off = 0.");
#endif

    // register Core as an abstract class to avoid problems if the class hierarchy is created within Core-constructor
    RegisterAbstractClass(Core).inheritsFrom<Configurable>();

    Core::Core(const std::string& cmdLine)
        : pathConfig_(NULL)
        , dynLibManager_(NULL)
        , signalHandler_(NULL)
        , configFileManager_(NULL)
        , languageInstance_(NULL)
        , loaderInstance_(NULL)
        , ioConsole_(NULL)
        , tclBind_(NULL)
        , tclThreadManager_(NULL)
        , rootScope_(NULL)
        , graphicsManager_(NULL)
        , inputManager_(NULL)
        , guiManager_(NULL)
        , graphicsScope_(NULL)
        , bGraphicsLoaded_(false)
        , bStartIOConsole_(true)
        , lastLevelTimestamp_(0)
        , ogreConfigTimestamp_(0)
        , bDevMode_(false)
        , destructionHelper_(this)
    {
        orxout(internal_status) << "initializing Core object..." << endl;

        // Set the hard coded fixed paths
        this->pathConfig_ = new PathConfig();

        // Create a new dynamic library manager
        this->dynLibManager_ = new DynLibManager();

        // Load modules
        orxout(internal_info) << "Loading modules:" << endl;
        const std::vector<std::string>& modulePaths = this->pathConfig_->getModulePaths();
        for (std::vector<std::string>::const_iterator it = modulePaths.begin(); it != modulePaths.end(); ++it)
        {
            try
            {
                this->dynLibManager_->load(*it);
            }
            catch (...)
            {
                orxout(user_error) << "Couldn't load module \"" << *it << "\": " << Exception::handleMessage() << endl;
            }
        }

        // TODO: initialize CommandLineParser here
        // TODO: initialize ConsoleCommandManager here
        ModuleInstance::getCurrentModuleInstance()->loadAllStaticallyInitializedInstances();

        // Parse command line arguments AFTER the modules have been loaded (static code!)
        CommandLineParser::parse(cmdLine);

        // Set configurable paths like log, config and media
        this->pathConfig_->setConfigurablePaths();

        orxout(internal_info) << "Root path:       " << PathConfig::getRootPathString() << endl;
        orxout(internal_info) << "Executable path: " << PathConfig::getExecutablePathString() << endl;
        orxout(internal_info) << "Data path:       " << PathConfig::getDataPathString() << endl;
        orxout(internal_info) << "Ext. data path:  " << PathConfig::getExternalDataPathString() << endl;
        orxout(internal_info) << "Config path:     " << PathConfig::getConfigPathString() << endl;
        orxout(internal_info) << "Log path:        " << PathConfig::getLogPathString() << endl;
        orxout(internal_info) << "Modules path:    " << PathConfig::getModulePathString() << endl;

        // create a signal handler (only active for Linux)
        // This call is placed as soon as possible, but after the directories are set
        this->signalHandler_ = new SignalHandler();
        this->signalHandler_->doCatch(PathConfig::getExecutablePathString(), PathConfig::getLogPathString() + "orxonox_crash.log");

#ifdef ORXONOX_PLATFORM_WINDOWS
        // limit the main thread to the first core so that QueryPerformanceCounter doesn't jump
        // do this after ogre has initialised. Somehow Ogre changes the settings again (not through
        // the timer though).
        int limitToCPU = CommandLineParser::getValue("limitToCPU");
        if (limitToCPU > 0)
            setThreadAffinity(static_cast<unsigned int>(limitToCPU));
#endif

        // Manage ini files and set the default settings file (usually orxonox.ini)
        orxout(internal_info) << "Loading config:" << endl;
        this->configFileManager_ = new ConfigFileManager();
        this->configFileManager_->setFilename(ConfigFileType::Settings,
            CommandLineParser::getValue("settingsFile").get<std::string>());

        // Required as well for the config values
        orxout(internal_info) << "Loading language:" << endl;
        this->languageInstance_ = new Language();

        // Do this soon after the ConfigFileManager has been created to open up the
        // possibility to configure everything below here
        RegisterObject(Core);
        orxout(internal_info) << "configuring Core" << endl;
        this->setConfigValues();

        // Set the correct log path and rewrite the log file with the correct log levels
        OutputManager::getInstance().getLogWriter()->setLogDirectory(PathConfig::getLogPathString());

#if !defined(ORXONOX_PLATFORM_APPLE) && !defined(ORXONOX_USE_WINMAIN)
        // Create persistent IO console
        if (CommandLineParser::getValue("noIOConsole").get<bool>())
        {
            ModifyConfigValue(bStartIOConsole_, tset, false);
        }
        if (this->bStartIOConsole_)
        {
            orxout(internal_info) << "creating IO console" << endl;
            this->ioConsole_ = new IOConsole();
        }
#endif

        // creates the class hierarchy for all classes with factories
        orxout(internal_info) << "creating class hierarchy" << endl;
        IdentifierManager::getInstance().createClassHierarchy();

        // Loader
        this->loaderInstance_ = new Loader();

        // Load OGRE excluding the renderer and the render window
        orxout(internal_info) << "creating GraphicsManager:" << endl;
        this->graphicsManager_ = new GraphicsManager(false);

        // initialise Tcl
        this->tclBind_ = new TclBind(PathConfig::getDataPathString());
        this->tclThreadManager_ = new TclThreadManager(tclBind_->getTclInterpreter());

        // Create singletons that always exist (in other libraries)
        orxout(internal_info) << "creating root scope:" << endl;
        this->rootScope_ = new Scope<ScopeID::Root>();

        // Generate documentation instead of normal run?
        std::string docFilename;
        CommandLineParser::getValue("generateDoc", &docFilename);
        if (!docFilename.empty())
        {
            std::ofstream docFile(docFilename.c_str());
            if (docFile.is_open())
            {
                CommandLineParser::generateDoc(docFile);
                docFile.close();
            }
            else
                orxout(internal_error) << "Could not open file for documentation writing" << endl;
        }

        orxout(internal_status) << "finished initializing Core object" << endl;
    }

    void Core::destroy()
    {
        orxout(internal_status) << "destroying Core object..." << endl;

        // Remove us from the object lists again to avoid problems when destroying them
        this->unregisterObject();

        safeObjectDelete(&graphicsScope_);
        safeObjectDelete(&guiManager_);
        safeObjectDelete(&inputManager_);
        safeObjectDelete(&graphicsManager_);
        safeObjectDelete(&rootScope_);
        safeObjectDelete(&tclThreadManager_);
        safeObjectDelete(&tclBind_);
        safeObjectDelete(&ioConsole_);
        safeObjectDelete(&loaderInstance_);
        safeObjectDelete(&languageInstance_);
        safeObjectDelete(&configFileManager_);
        ConsoleCommandManager::destroyAll();
        Context::setRootContext(NULL);
        IdentifierManager::getInstance().destroyAllIdentifiers();
        safeObjectDelete(&signalHandler_);
        safeObjectDelete(&dynLibManager_);
        safeObjectDelete(&pathConfig_);

        orxout(internal_status) << "finished destroying Core object" << endl;
    }

    //! Function to collect the SetConfigValue-macro calls.
    void Core::setConfigValues()
    {
        SetConfigValueExternal(OutputManager::getInstance().getLogWriter()->configurableMaxLevel_,
                               OutputManager::getInstance().getLogWriter()->getConfigurableSectionName(),
                               OutputManager::getInstance().getLogWriter()->getConfigurableMaxLevelName(),
                               OutputManager::getInstance().getLogWriter()->configurableMaxLevel_)
            .description("The maximum level of output shown in the log file")
            .callback(static_cast<BaseWriter*>(OutputManager::getInstance().getLogWriter()), &BaseWriter::changedConfigurableLevel);
        SetConfigValueExternal(OutputManager::getInstance().getLogWriter()->configurableAdditionalContextsMaxLevel_,
                               OutputManager::getInstance().getLogWriter()->getConfigurableSectionName(),
                               OutputManager::getInstance().getLogWriter()->getConfigurableAdditionalContextsMaxLevelName(),
                               OutputManager::getInstance().getLogWriter()->configurableAdditionalContextsMaxLevel_)
            .description("The maximum level of output shown in the log file for additional contexts")
            .callback(static_cast<BaseWriter*>(OutputManager::getInstance().getLogWriter()), &BaseWriter::changedConfigurableAdditionalContextsLevel);
        SetConfigValueExternal(OutputManager::getInstance().getLogWriter()->configurableAdditionalContexts_,
                               OutputManager::getInstance().getLogWriter()->getConfigurableSectionName(),
                               OutputManager::getInstance().getLogWriter()->getConfigurableAdditionalContextsName(),
                               OutputManager::getInstance().getLogWriter()->configurableAdditionalContexts_)
            .description("Additional output contexts shown in the log file")
            .callback(static_cast<BaseWriter*>(OutputManager::getInstance().getLogWriter()), &BaseWriter::changedConfigurableAdditionalContexts);

        SetConfigValue(bDevMode_, PathConfig::buildDirectoryRun())
            .description("Developer mode. If not set, hides some things from the user to not confuse him.")
            .callback(this, &Core::devModeChanged);
        SetConfigValue(language_, Language::getInstance().defaultLanguage_)
            .description("The language of the in game text")
            .callback(this, &Core::languageChanged);
        SetConfigValue(bInitRandomNumberGenerator_, true)
            .description("If true, all random actions are different each time you start the game")
            .callback(this, &Core::initRandomNumberGenerator);
        SetConfigValue(bStartIOConsole_, true)
            .description("Set to false if you don't want to use the IOConsole (for Lua debugging for instance)");
        SetConfigValue(lastLevelTimestamp_, 0)
            .description("Timestamp when the last level was started.");
        SetConfigValue(ogreConfigTimestamp_, 0)
            .description("Timestamp when the ogre config file was changed.");
    }

    /** Callback function for changes in the dev mode that affect debug levels.
        The function behaves according to these rules:
        - 'normal' mode is defined based on where the program was launched: if
          the launch path was the build directory, development mode \c on is
          normal, otherwise normal means development mode \c off.
        - Debug levels should not be hard configured (\c config instead of
          \c tconfig) in non 'normal' mode to avoid strange behaviour.
        - Changing the development mode from 'normal' to the other state will
          immediately change the debug levels to predefined values which can be
          reconfigured with \c tconfig.
    @note
        The debug levels for the IOConsole and the InGameConsole can be found
        in the Shell class. The same rules apply.
    */
    void Core::devModeChanged()
    {
        // Inform listeners
        ObjectList<DevModeListener>::iterator it = ObjectList<DevModeListener>::begin();
        for (; it != ObjectList<DevModeListener>::end(); ++it)
            it->devModeChanged(bDevMode_);
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
        orxout(internal_info) << "loading graphics in Core" << endl;

        // Any exception should trigger this, even in upgradeToGraphics (see its remarks)
        Loki::ScopeGuard unloader = Loki::MakeObjGuard(*this, &Core::unloadGraphics);

        // Upgrade OGRE to receive a render window
        try
        {
            graphicsManager_->upgradeToGraphics();
        }
        catch (const InitialisationFailedException&)
        {
            // Exit the application if the Ogre config dialog was canceled
            orxout(user_error) << Exception::handleMessage() << endl;
            exit(EXIT_FAILURE);
        }
        catch (...)
        {
            // Recovery from this is very difficult. It requires to completely
            // destroy Ogre related objects and load again (without graphics).
            // However since Ogre 1.7 there seems to be a problem when Ogre
            // throws an exception and the graphics engine then gets destroyed
            // and reloaded between throw and catch (access violation in MSVC).
            // That's why we abort completely and only display the exception.
            orxout(user_error) << "An exception occurred during upgrade to graphics. "
                               << "That is unrecoverable. The message was:" << endl
                               << Exception::handleMessage() << endl;
            abort();
        }

        // Calls the InputManager which sets up the input devices.
        inputManager_ = new InputManager();

        // Load the CEGUI interface
        guiManager_ = new GUIManager(inputManager_->getMousePosition());

        bGraphicsLoaded_ = true;
        GameMode::bShowsGraphics_s = true;

        // Load some sort of a debug overlay (only denoted by its name, "debug.oxo")
        graphicsManager_->loadDebugOverlay();

        // Create singletons associated with graphics (in other libraries)
        orxout(internal_info) << "creating graphics scope:" << endl;
        graphicsScope_ = new Scope<ScopeID::Graphics>();

        unloader.Dismiss();

        orxout(internal_info) << "finished loading graphics in Core" << endl;
    }

    void Core::unloadGraphics()
    {
        orxout(internal_info) << "unloading graphics in Core" << endl;

        safeObjectDelete(&graphicsScope_);
        safeObjectDelete(&guiManager_);
        safeObjectDelete(&inputManager_);
        safeObjectDelete(&graphicsManager_);

        // Load Ogre::Root again, but without the render system
        try
            { this->graphicsManager_ = new GraphicsManager(false); }
        catch (...)
        {
            orxout(user_error) << "An exception occurred during 'unloadGraphics':" << Exception::handleMessage() << endl
                               << "Another exception might be being handled which may lead to undefined behaviour!" << endl
                               << "Terminating the program." << endl;
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
        // Update UpdateListeners before general ticking
        for (ObjectList<UpdateListener>::iterator it = ObjectList<UpdateListener>::begin(); it != ObjectList<UpdateListener>::end(); ++it)
            it->preUpdate(time);
        if (this->bGraphicsLoaded_)
        {
            // Process input events
            this->inputManager_->preUpdate(time);
            // Update GUI
            this->guiManager_->preUpdate(time);
        }
        // Process console events and status line
        if (this->ioConsole_ != NULL)
            this->ioConsole_->preUpdate(time);
        // Process thread commands
        this->tclThreadManager_->preUpdate(time);
    }

    void Core::postUpdate(const Clock& time)
    {
        // Update UpdateListeners just before rendering
        for (ObjectList<UpdateListener>::iterator it = ObjectList<UpdateListener>::begin(); it != ObjectList<UpdateListener>::end(); ++it)
            it->postUpdate(time);
        if (this->bGraphicsLoaded_)
        {
            // Render (doesn't throw)
            this->graphicsManager_->postUpdate(time);
        }
    }

    void Core::updateLastLevelTimestamp()
    {
        ModifyConfigValue(lastLevelTimestamp_, set, static_cast<long long>(time(NULL)));
    }

    void Core::updateOgreConfigTimestamp()
    {
        ModifyConfigValue(ogreConfigTimestamp_, set, static_cast<long long>(time(NULL)));
    }


    RegisterAbstractClass(DevModeListener).inheritsFrom<Listable>();

    DevModeListener::DevModeListener()
    {
        RegisterObject(DevModeListener);
    }
}
