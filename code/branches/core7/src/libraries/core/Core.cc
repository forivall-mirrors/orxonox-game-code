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
#include "core/singleton/ScopedSingletonIncludes.h"
#include "util/SignalHandler.h"
#include "ApplicationPaths.h"
#include "ConfigurablePaths.h"
#include "commandline/CommandLineIncludes.h"
#include "config/ConfigFileManager.h"
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

    Core::Core(const std::string& cmdLine)
        : applicationPaths_(NULL)
        , configurablePaths_(NULL)
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
        , rootModule_(NULL)
        , config_(NULL)
        , destructionHelper_(this)
    {
        orxout(internal_status) << "initializing Core object..." << endl;

        // Set the hard coded fixed paths
        this->applicationPaths_ = new ApplicationPaths();

        // Create a new dynamic library manager
        this->dynLibManager_ = new DynLibManager();

        // TODO: initialize Root-Context
        // TODO: initialize IdentifierManager here
        // TODO: initialize ScopeManager here
        // TODO: initialize CommandLineParser here
        // TODO: initialize ConsoleCommandManager here
        // TODO: initialize NetworkFunctionManager here
        this->rootModule_ = ModuleInstance::getCurrentModuleInstance();
        this->rootModule_->loadAllStaticallyInitializedInstances(0);

        // Parse command line arguments AFTER the modules have been loaded (static code!)
        CommandLineParser::parse(cmdLine);

        // Set configurable paths like log, config and media
        this->configurablePaths_ = new ConfigurablePaths();
        this->configurablePaths_->setConfigurablePaths(ApplicationPaths::getInstance());

        orxout(internal_info) << "Root path:       " << ApplicationPaths::getRootPathString() << endl;
        orxout(internal_info) << "Executable path: " << ApplicationPaths::getExecutablePathString() << endl;
        orxout(internal_info) << "Modules path:    " << ApplicationPaths::getModulePathString() << endl;

        orxout(internal_info) << "Data path:       " << ConfigurablePaths::getDataPathString() << endl;
        orxout(internal_info) << "Ext. data path:  " << ConfigurablePaths::getExternalDataPathString() << endl;
        orxout(internal_info) << "Config path:     " << ConfigurablePaths::getConfigPathString() << endl;
        orxout(internal_info) << "Log path:        " << ConfigurablePaths::getLogPathString() << endl;

        // create a signal handler
        // This call is placed as soon as possible, but after the directories are set
        this->signalHandler_ = new SignalHandler();
        this->signalHandler_->doCatch(ApplicationPaths::getExecutablePathString(), ConfigurablePaths::getLogPathString() + "orxonox_crash.log");

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
        orxout(internal_info) << "configuring Core" << endl;
        this->config_ = new CoreConfig();

        // Set the correct log path and rewrite the log file with the correct log levels
        OutputManager::getInstance().getLogWriter()->setLogDirectory(ConfigurablePaths::getLogPathString());

#if !defined(ORXONOX_PLATFORM_APPLE) && !defined(ORXONOX_USE_WINMAIN)
        // Create persistent IO console
        if (CommandLineParser::getValue("noIOConsole").get<bool>() == false && this->config_->getStartIOConsole())
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
        this->tclBind_ = new TclBind(ConfigurablePaths::getDataPathString());
        this->tclThreadManager_ = new TclThreadManager(tclBind_->getTclInterpreter());

        // Create singletons that always exist (in other libraries)
        orxout(internal_info) << "creating root scope:" << endl;
        this->rootScope_ = new Scope<ScopeID::ROOT>();

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

        safeObjectDelete(&graphicsScope_);
        safeObjectDelete(&guiManager_);
        safeObjectDelete(&inputManager_);
        safeObjectDelete(&graphicsManager_);
        safeObjectDelete(&rootScope_);
        safeObjectDelete(&tclThreadManager_);
        safeObjectDelete(&tclBind_);
        safeObjectDelete(&ioConsole_);
        safeObjectDelete(&loaderInstance_);
        safeObjectDelete(&config_);
        safeObjectDelete(&languageInstance_);
        safeObjectDelete(&configFileManager_);
        safeObjectDelete(&signalHandler_);
        Context::getRootContext()->unregisterObject(); // unregister context from object lists - otherwise the root context would be destroyed while unloading the root module
        if (this->rootModule_)
        {
            this->rootModule_->unloadAllStaticallyInitializedInstances(0);
            this->rootModule_->deleteAllStaticallyInitializedInstances();
        }
        Context::setRootContext(NULL);
        safeObjectDelete(&rootModule_);
        safeObjectDelete(&dynLibManager_);
        safeObjectDelete(&configurablePaths_);
        safeObjectDelete(&applicationPaths_);

        orxout(internal_status) << "finished destroying Core object" << endl;
    }

    void Core::loadModules()
    {
        orxout(internal_info) << "Loading modules:" << endl;

        const std::vector<std::string>& modulePaths = ApplicationPaths::getInstance().getModulePaths();
        for (std::vector<std::string>::const_iterator it = modulePaths.begin(); it != modulePaths.end(); ++it)
        {
            try
            {
                ModuleInstance* module = new ModuleInstance(*it);
                this->loadModule(module);
                this->modules_.push_back(module);
            }
            catch (...)
            {
                orxout(user_error) << "Couldn't load module \"" << *it << "\": " << Exception::handleMessage() << endl;
            }
        }

        orxout(internal_info) << "finished loading modules" << endl;
    }

    void Core::loadModule(ModuleInstance* module)
    {
        orxout(internal_info) << "Loading module " << module->getName() << "..." << endl;

        ModuleInstance::setCurrentModuleInstance(module);
        DynLib* dynLib = this->dynLibManager_->load(module->getName());
        module->setDynLib(dynLib);
        module->loadAllStaticallyInitializedInstances(0);
        IdentifierManager::getInstance().createClassHierarchy();
        ScopeManager::getInstance().updateListeners();
    }

    void Core::unloadModules()
    {
        for (std::list<ModuleInstance*>::iterator it = this->modules_.begin(); it != this->modules_.end(); ++it)
        {
            ModuleInstance* module = (*it);
            this->unloadModule(module);
            delete module;
        }
        this->modules_.clear();
    }

    void Core::unloadModule(ModuleInstance* module)
    {
        orxout(internal_info) << "Unloading module " << module->getName() << "..." << endl;

        module->unloadAllStaticallyInitializedInstances(0);
        module->deleteAllStaticallyInitializedInstances();
        this->dynLibManager_->unload(module->getDynLib());
        module->setDynLib(NULL);
    }

    void Core::loadGraphics()
    {
        orxout(internal_info) << "loading graphics in Core" << endl;

        // Any exception should trigger this, even in upgradeToGraphics (see its remarks)
        Loki::ScopeGuard unloader = Loki::MakeObjGuard(*this, &Core::unloadGraphics, true);

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
        graphicsScope_ = new Scope<ScopeID::GRAPHICS>();

        unloader.Dismiss();

        orxout(internal_info) << "finished loading graphics in Core" << endl;
    }

    void Core::unloadGraphics(bool loadGraphicsManagerWithoutRenderer)
    {
        orxout(internal_info) << "unloading graphics in Core" << endl;

        if (this->graphicsManager_)
            this->graphicsManager_->unloadDebugOverlay();

        safeObjectDelete(&graphicsScope_);
        safeObjectDelete(&guiManager_);
        safeObjectDelete(&inputManager_);
        safeObjectDelete(&graphicsManager_);

        // Load Ogre::Root again, but without the render system
        try
        {
            if (loadGraphicsManagerWithoutRenderer)
                this->graphicsManager_ = new GraphicsManager(false);
        }
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
}
