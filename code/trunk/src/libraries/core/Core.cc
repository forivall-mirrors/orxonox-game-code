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
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <boost/version.hpp>
#include <boost/filesystem.hpp>

#ifdef ORXONOX_PLATFORM_WINDOWS
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#  undef min
#  undef max
#elif defined(ORXONOX_PLATFORM_APPLE)
#  include <sys/param.h>
#  include <mach-o/dyld.h>
#else /* Linux */
#  include <sys/types.h>
#  include <unistd.h>
#endif

#include "SpecialConfig.h"
#include "util/Debug.h"
#include "util/Exception.h"
#include "util/SignalHandler.h"
#include "Clock.h"
#include "CommandExecutor.h"
#include "CommandLine.h"
#include "ConfigFileManager.h"
#include "ConfigValueIncludes.h"
#include "CoreIncludes.h"
#include "DynLibManager.h"
#include "Factory.h"
#include "GameMode.h"
#include "GraphicsManager.h"
#include "GUIManager.h"
#include "Identifier.h"
#include "Language.h"
#include "LuaState.h"
#include "Shell.h"
#include "TclBind.h"
#include "TclThreadManager.h"
#include "input/InputManager.h"

// Boost 1.36 has some issues with deprecated functions that have been omitted
#if (BOOST_VERSION == 103600)
#  define BOOST_LEAF_FUNCTION filename
#else
#  define BOOST_LEAF_FUNCTION leaf
#endif

namespace orxonox
{
    //! Static pointer to the singleton
    Core* Core::singletonPtr_s  = 0;

    SetCommandLineArgument(externalDataPath, "").information("Path to the external data files");
    SetCommandLineOnlyArgument(writingPathSuffix, "").information("Additional subfolder for config and log files");
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

            // External data directory only exists for dev runs
            if (Core::isDevelopmentRun())
            {
                // Possible data path override by the command line
                if (!CommandLine::getArgument("externalDataPath")->hasDefaultValue())
                    tsetExternalDataPath(CommandLine::getValue("externalDataPath"));
            }
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

        /**
        @brief
            Temporary sets the data path
        @param path
            The new data path
        */
        void tsetExternalDataPath(const std::string& path)
        {
            dataPath_ = boost::filesystem::path(path);
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

        //! Path to the parent directory of the ones above if program was installed with relativ pahts
        boost::filesystem::path rootPath_;
        boost::filesystem::path executablePath_;        //!< Path to the executable
        boost::filesystem::path modulePath_;            //!< Path to the modules
        boost::filesystem::path dataPath_;              //!< Path to the data file folder
        boost::filesystem::path externalDataPath_;      //!< Path to the external data file folder
        boost::filesystem::path configPath_;            //!< Path to the config file folder
        boost::filesystem::path logPath_;               //!< Path to the log file folder
    };


    Core::Core(const std::string& cmdLine)
        // Cleanup guard for identifier destruction (incl. XMLPort, configValues, consoleCommands)
        : identifierDestroyer_(Identifier::destroyAllIdentifiers)
        // Cleanup guard for external console commands that don't belong to an Identifier
        , consoleCommandDestroyer_(CommandExecutor::destroyExternalCommands)
        , configuration_(new CoreConfiguration()) // Don't yet create config values!
        , bDevRun_(false)
        , bGraphicsLoaded_(false)
    {
        // Set the hard coded fixed paths
        this->setFixedPaths();

        // Create a new dynamic library manager
        this->dynLibManager_.reset(new DynLibManager());

        // Load modules
        try
        {
            // We search for helper files with the following extension
            std::string moduleextension = specialConfig::moduleExtension;
            size_t moduleextensionlength = moduleextension.size();

            // Search in the directory of our executable
            boost::filesystem::path searchpath = this->configuration_->modulePath_;

            // Add that path to the PATH variable in case a module depends on another one
            std::string pathVariable = getenv("PATH");
            putenv(const_cast<char*>(("PATH=" + pathVariable + ";" + configuration_->modulePath_.string()).c_str()));

            boost::filesystem::directory_iterator file(searchpath);
            boost::filesystem::directory_iterator end;

            // Iterate through all files
            while (file != end)
            {
                std::string filename = file->BOOST_LEAF_FUNCTION();

                // Check if the file ends with the exension in question
                if (filename.size() > moduleextensionlength)
                {
                    if (filename.substr(filename.size() - moduleextensionlength) == moduleextension)
                    {
                        // We've found a helper file - now load the library with the same name
                        std::string library = filename.substr(0, filename.size() - moduleextensionlength);
                        boost::filesystem::path librarypath = searchpath / library;

                        try
                        {
                            DynLibManager::getInstance().load(librarypath.string());
                        }
                        catch (...)
                        {
                            COUT(1) << "Couldn't load module \"" << librarypath.string() << "\": " << Exception::handleMessage() << std::endl;
                        }
                    }
                }

                ++file;
            }
        }
        catch (...)
        {
            COUT(1) << "An error occurred while loading modules: " << Exception::handleMessage() << std::endl;
        }

        // Parse command line arguments AFTER the modules have been loaded (static code!)
        CommandLine::parseCommandLine(cmdLine);

        // Set configurable paths like log, config and media
        this->setConfigurablePaths();

        // create a signal handler (only active for linux)
        // This call is placed as soon as possible, but after the directories are set
        this->signalHandler_.reset(new SignalHandler());
        this->signalHandler_->doCatch(configuration_->executablePath_.string(), Core::getLogPathString() + "orxonox_crash.log");

        // Set the correct log path. Before this call, /tmp (Unix) or %TEMP% was used
        OutputHandler::getOutStream().setLogPath(Core::getLogPathString());

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
        Factory::createClassHierarchy();

        // Do this soon after the ConfigFileManager has been created to open up the
        // possibility to configure everything below here
        this->configuration_->initialise();

        // Load OGRE excluding the renderer and the render window
        this->graphicsManager_.reset(new GraphicsManager(false));

        // initialise Tcl
        this->tclBind_.reset(new TclBind(Core::getDataPathString()));
        this->tclThreadManager_.reset(new TclThreadManager(tclBind_->getTclInterpreter()));

        // create a shell
        this->shell_.reset(new Shell());
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

        // load the CEGUI interface
        guiManager_.reset(new GUIManager(graphicsManager_->getRenderWindow(),
            inputManager_->getMousePosition(), graphicsManager_->isFullScreen()));

        unloader.Dismiss();

        bGraphicsLoaded_ = true;
    }

    void Core::unloadGraphics()
    {
        this->guiManager_.reset();;
        this->inputManager_.reset();;
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

    /*static*/ void Core::tsetExternalDataPath(const std::string& path)
    {
        getInstance().configuration_->tsetExternalDataPath(path);
    }

    /*static*/ const boost::filesystem::path& Core::getDataPath()
    {
        return getInstance().configuration_->dataPath_;
    }
    /*static*/ std::string Core::getDataPathString()
    {
        return getInstance().configuration_->dataPath_.string() + '/';
    }

    /*static*/ const boost::filesystem::path& Core::getExternalDataPath()
    {
        return getInstance().configuration_->externalDataPath_;
    }
    /*static*/ std::string Core::getExternalDataPathString()
    {
        return getInstance().configuration_->externalDataPath_.string() + '/';
    }

    /*static*/ const boost::filesystem::path& Core::getConfigPath()
    {
        return getInstance().configuration_->configPath_;
    }
    /*static*/ std::string Core::getConfigPathString()
    {
        return getInstance().configuration_->configPath_.string() + '/';
    }

    /*static*/ const boost::filesystem::path& Core::getLogPath()
    {
        return getInstance().configuration_->logPath_;
    }
    /*static*/ std::string Core::getLogPathString()
    {
        return getInstance().configuration_->logPath_.string() + '/';
    }

    /*static*/ const boost::filesystem::path& Core::getRootPath()
    {
        return getInstance().configuration_->rootPath_;
    }
    /*static*/ std::string Core::getRootPathString()
    {
        return getInstance().configuration_->rootPath_.string() + '/';
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

    /**
    @brief
        Retrievs the executable path and sets all hard coded fixed path (currently only the module path)
        Also checks for "orxonox_dev_build.keep_me" in the executable diretory.
        If found it means that this is not an installed run, hence we
        don't write the logs and config files to ~/.orxonox
    @throw
        GeneralException
    */
    void Core::setFixedPaths()
    {
        //////////////////////////
        // FIND EXECUTABLE PATH //
        //////////////////////////

#ifdef ORXONOX_PLATFORM_WINDOWS
        // get executable module
        TCHAR buffer[1024];
        if (GetModuleFileName(NULL, buffer, 1024) == 0)
            ThrowException(General, "Could not retrieve executable path.");

#elif defined(ORXONOX_PLATFORM_APPLE)
        char buffer[1024];
        unsigned long path_len = 1023;
        if (_NSGetExecutablePath(buffer, &path_len))
            ThrowException(General, "Could not retrieve executable path.");

#else /* Linux */
        /* written by Nicolai Haehnle <prefect_@gmx.net> */

        /* Get our PID and build the name of the link in /proc */
        char linkname[64]; /* /proc/<pid>/exe */
        if (snprintf(linkname, sizeof(linkname), "/proc/%i/exe", getpid()) < 0)
        {
            /* This should only happen on large word systems. I'm not sure
               what the proper response is here.
               Since it really is an assert-like condition, aborting the
               program seems to be in order. */
            assert(false);
        }

        /* Now read the symbolic link */
        char buffer[1024];
        int ret;
        ret = readlink(linkname, buffer, 1024);
        /* In case of an error, leave the handling up to the caller */
        if (ret == -1)
            ThrowException(General, "Could not retrieve executable path.");

        /* Ensure proper NUL termination */
        buffer[ret] = 0;
#endif

        configuration_->executablePath_ = boost::filesystem::path(buffer);
#ifndef ORXONOX_PLATFORM_APPLE
        configuration_->executablePath_ = configuration_->executablePath_.branch_path(); // remove executable name
#endif

        /////////////////////
        // SET MODULE PATH //
        /////////////////////

        if (boost::filesystem::exists(configuration_->executablePath_ / "orxonox_dev_build.keep_me"))
        {
            COUT(1) << "Running from the build tree." << std::endl;
            Core::bDevRun_ = true;
            configuration_->modulePath_ = specialConfig::moduleDevDirectory;
        }
        else
        {

#ifdef INSTALL_COPYABLE // --> relative paths

            // Also set the root path
            boost::filesystem::path relativeExecutablePath(specialConfig::defaultRuntimePath);
            configuration_->rootPath_ = configuration_->executablePath_;
            while (!boost::filesystem::equivalent(configuration_->rootPath_ / relativeExecutablePath, configuration_->executablePath_)
                   && !configuration_->rootPath_.empty())
                configuration_->rootPath_ = configuration_->rootPath_.branch_path();
            if (configuration_->rootPath_.empty())
                ThrowException(General, "Could not derive a root directory. Might the binary installation directory contain '..' when taken relative to the installation prefix path?");

            // Module path is fixed as well
            configuration_->modulePath_ = configuration_->rootPath_ / specialConfig::defaultModulePath;

#else

            // There is no root path, so don't set it at all
            // Module path is fixed as well
            configuration_->modulePath_ = specialConfig::moduleInstallDirectory;

#endif
        }
    }

    /**
    @brief
        Sets config, log and media path and creates folders if necessary.
    @throws
        GeneralException
    */
    void Core::setConfigurablePaths()
    {
        if (Core::isDevelopmentRun())
        {
            configuration_->dataPath_  = specialConfig::dataDevDirectory;
            configuration_->externalDataPath_ = specialConfig::externalDataDevDirectory;
            configuration_->configPath_ = specialConfig::configDevDirectory;
            configuration_->logPath_    = specialConfig::logDevDirectory;
        }
        else
        {

#ifdef INSTALL_COPYABLE // --> relative paths

            // Using paths relative to the install prefix, complete them
            configuration_->dataPath_   = configuration_->rootPath_ / specialConfig::defaultDataPath;
            configuration_->configPath_ = configuration_->rootPath_ / specialConfig::defaultConfigPath;
            configuration_->logPath_    = configuration_->rootPath_ / specialConfig::defaultLogPath;

#else

            configuration_->dataPath_  = specialConfig::dataInstallDirectory;

            // Get user directory
#  ifdef ORXONOX_PLATFORM_UNIX /* Apple? */
            char* userDataPathPtr(getenv("HOME"));
#  else
            char* userDataPathPtr(getenv("APPDATA"));
#  endif
            if (userDataPathPtr == NULL)
                ThrowException(General, "Could not retrieve user data path.");
            boost::filesystem::path userDataPath(userDataPathPtr);
            userDataPath /= ".orxonox";

            configuration_->configPath_ = userDataPath / specialConfig::defaultConfigPath;
            configuration_->logPath_    = userDataPath / specialConfig::defaultLogPath;

#endif

        }

        // Option to put all the config and log files in a separate folder
        if (!CommandLine::getArgument("writingPathSuffix")->hasDefaultValue())
        {
            std::string directory(CommandLine::getValue("writingPathSuffix").getString());
            configuration_->configPath_ = configuration_->configPath_ / directory;
            configuration_->logPath_    = configuration_->logPath_    / directory;
        }

        // Create directories to avoid problems when opening files in non existent folders.
        std::vector<std::pair<boost::filesystem::path, std::string> > directories;
        directories.push_back(std::make_pair(boost::filesystem::path(configuration_->configPath_), "config"));
        directories.push_back(std::make_pair(boost::filesystem::path(configuration_->logPath_), "log"));

        for (std::vector<std::pair<boost::filesystem::path, std::string> >::iterator it = directories.begin();
            it != directories.end(); ++it)
        {
            if (boost::filesystem::exists(it->first) && !boost::filesystem::is_directory(it->first))
            {
                ThrowException(General, std::string("The ") + it->second + " directory has been preoccupied by a file! \
                                         Please remove " + it->first.string());
            }
            if (boost::filesystem::create_directories(it->first)) // function may not return true at all (bug?)
            {
                COUT(4) << "Created " << it->second << " directory" << std::endl;
            }
        }
    }

    void Core::preUpdate(const Clock& time)
    {
        if (this->bGraphicsLoaded_)
        {
            // process input events
            this->inputManager_->update(time);
            // process gui events
            this->guiManager_->update(time);
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
