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
#include "Factory.h"
#include "Identifier.h"
#include "Language.h"
#include "LuaBind.h"
#include "Shell.h"
#include "TclBind.h"
#include "TclThreadManager.h"

namespace orxonox
{
    //! Static pointer to the singleton
    Core* Core::singletonRef_s  = 0;

    SetCommandLineArgument(mediaPath, "").information("PATH");
    SetCommandLineOnlyArgument(writingPathSuffix, "").information("DIR");
    SetCommandLineArgument(settingsFile, "orxonox.ini");
    SetCommandLineArgument(limitToCPU, 0).information("0: off | #cpu");

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

            // Possible media path override by the command line
            if (!CommandLine::getArgument("mediaPath")->hasDefaultValue())
                tsetMediaPath(CommandLine::getValue("mediaPath"));
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

            SetConfigValue(language_, Language::getLanguage().defaultLanguage_)
                .description("The language of the ingame text")
                .callback(this, &CoreConfiguration::languageChanged);
            SetConfigValue(bInitializeRandomNumberGenerator_, true)
                .description("If true, all random actions are different each time you start the game")
                .callback(this, &CoreConfiguration::initializeRandomNumberGenerator);

            SetConfigValue(mediaPathString_, mediaPath_.string())
                .description("Relative path to the game data.")
                .callback(this, &CoreConfiguration::mediaPathChanged);
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
            Language::getLanguage().readTranslatedLanguageFile();
        }

        /**
        @brief
            Callback function if the media path has changed.
        */
        void mediaPathChanged()
        {
            mediaPath_ = boost::filesystem::path(this->mediaPathString_);
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
            Temporary sets the media path
        @param path
            The new media path
        */
        void tsetMediaPath(const std::string& path)
        {
            ModifyConfigValue(mediaPathString_, tset, path);
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
        std::string mediaPathString_;                   //!< Path to the data/media file folder as string

        //! Path to the parent directory of the ones above if program was installed with relativ pahts
        boost::filesystem::path rootPath_;
        boost::filesystem::path executablePath_;        //!< Path to the executable
        boost::filesystem::path mediaPath_;             //!< Path to the media file folder
        boost::filesystem::path configPath_;            //!< Path to the config file folder
        boost::filesystem::path logPath_;               //!< Path to the log file folder
    };


    Core::Core(int argc, char** argv)
    {
        assert(Core::singletonRef_s == 0);
        Core::singletonRef_s = this;

        // We need the variables very soon. But don't configure them yet!
        this->configuration_ = new CoreConfiguration();

        // Parse command line arguments first
        CommandLine::parseCommandLine(argc, argv);

        // Determine and set the location of the executable
        setExecutablePath();

        // Determine whether we have an installed or a binary dir run
        // The latter occurs when simply running from the build directory
        checkDevBuild();

        // Make sure the directories we write in exist or else make them
        createDirectories();

        // create a signal handler (only active for linux)
        // This call is placed as soon as possible, but after the directories are set
        this->signalHandler_ = new SignalHandler();
        this->signalHandler_->doCatch(configuration_->executablePath_.string(), Core::getLogPathString() + "orxonox_crash.log");

        // Set the correct log path. Before this call, /tmp (Unix) or %TEMP% was used
        OutputHandler::getOutStream().setLogPath(Core::getLogPathString());

        // Parse additional options file now that we know its path
        CommandLine::parseFile();

        // limit the main thread to the first core so that QueryPerformanceCounter doesn't jump
        // do this after ogre has initialised. Somehow Ogre changes the settings again (not through
        // the timer though).
        int limitToCPU = CommandLine::getValue("limitToCPU");
        if (limitToCPU > 0)
            setThreadAffinity((unsigned int)limitToCPU);

        // Manage ini files and set the default settings file (usually orxonox.ini)
        this->configFileManager_ = new ConfigFileManager();
        this->configFileManager_->setFilename(ConfigFileType::Settings,
            CommandLine::getValue("settingsFile").getString());

        this->languageInstance_ = new Language();

        // Do this soon after the ConfigFileManager has been created to open up the
        // possibility to configure everything below here
        this->configuration_->initialise();

        // Create the lua interface
        this->luaBind_ = new LuaBind();

        // initialise Tcl
        this->tclBind_ = new TclBind(Core::getMediaPathString());
        this->tclThreadManager_ = new TclThreadManager(tclBind_->getTclInterpreter());

        // create a shell
        this->shell_ = new Shell();

        // creates the class hierarchy for all classes with factories
        Factory::createClassHierarchy();
    }

    /**
        @brief Sets the bool to true to avoid static functions accessing a deleted object.
    */
    Core::~Core()
    {
        delete this->shell_;
        delete this->tclThreadManager_;
        delete this->tclBind_;
        delete this->luaBind_;
        delete this->configuration_;
        delete this->languageInstance_;
        delete this->configFileManager_;

        // Destroy command line arguments
        CommandLine::destroyAllArguments();
        // Also delete external console command that don't belong to an Identifier
        CommandExecutor::destroyExternalCommands();

        // Clean up class hierarchy stuff (identifiers, XMLPort, configValues, consoleCommand)
        Identifier::destroyAllIdentifiers();

        assert(Core::singletonRef_s);
        Core::singletonRef_s = 0;
        delete this->signalHandler_;
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

    /*static*/ void Core::tsetMediaPath(const std::string& path)
    {
        getInstance().configuration_->tsetMediaPath(path);
    }

    /*static*/ const boost::filesystem::path& Core::getMediaPath()
    {
        return getInstance().configuration_->mediaPath_;
    }
    /*static*/ std::string Core::getMediaPathString()
    {
        return getInstance().configuration_->mediaPath_.string() + '/';
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
        if (limitToCPU <= 0)
            return;

#ifdef ORXONOX_PLATFORM_WINDOWS
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
        Compares the executable path with the working directory
    */
    void Core::setExecutablePath()
    {
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
    }

    /**
    @brief
        Checks for "orxonox_dev_build.keep_me" in the executable diretory.
        If found it means that this is not an installed run, hence we
        don't write the logs and config files to ~/.orxonox
    @throws
        GeneralException
    */
    void Core::checkDevBuild()
    {
        if (boost::filesystem::exists(configuration_->executablePath_ / "orxonox_dev_build.keep_me"))
        {
            COUT(1) << "Running from the build tree." << std::endl;
            Core::isDevBuild_ = true;
            configuration_->mediaPath_  = ORXONOX_MEDIA_DEV_PATH;
            configuration_->configPath_ = ORXONOX_CONFIG_DEV_PATH;
            configuration_->logPath_    = ORXONOX_LOG_DEV_PATH;
        }
        else
        {
#ifdef INSTALL_COPYABLE // --> relative paths
            // Also set the root path
            boost::filesystem::path relativeExecutablePath(ORXONOX_RUNTIME_INSTALL_PATH);
            configuration_->rootPath_ = configuration_->executablePath_;
            while (!boost::filesystem::equivalent(configuration_->rootPath_ / relativeExecutablePath, configuration_->executablePath_)
                   && !configuration_->rootPath_.empty())
                configuration_->rootPath_ = configuration_->rootPath_.branch_path();
            if (configuration_->rootPath_.empty())
                ThrowException(General, "Could not derive a root directory. Might the binary installation directory contain '..' when taken relative to the installation prefix path?");

            // Using paths relative to the install prefix, complete them
            configuration_->mediaPath_  = configuration_->rootPath_ / ORXONOX_MEDIA_INSTALL_PATH;
            configuration_->configPath_ = configuration_->rootPath_ / ORXONOX_CONFIG_INSTALL_PATH;
            configuration_->logPath_    = configuration_->rootPath_ / ORXONOX_LOG_INSTALL_PATH;
#else
            // There is no root path, so don't set it at all

            configuration_->mediaPath_  = ORXONOX_MEDIA_INSTALL_PATH;

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

            configuration_->configPath_ = userDataPath / ORXONOX_CONFIG_INSTALL_PATH;
            configuration_->logPath_    = userDataPath / ORXONOX_LOG_INSTALL_PATH;
#endif
        }

        // Option to put all the config and log files in a separate folder
        if (!CommandLine::getArgument("writingPathSuffix")->hasDefaultValue())
        {
            std::string directory(CommandLine::getValue("writingPathSuffix").getString());
            configuration_->configPath_ = configuration_->configPath_ / directory;
            configuration_->logPath_    = configuration_->logPath_    / directory;
        }
    }

    /*
    @brief
        Checks for the log and the config directory and creates them
        if necessary. Otherwise me might have problems opening those files.
    @throws
        orxonox::GeneralException if the directory to be created is a file.
    */
    void Core::createDirectories()
    {
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

    void Core::update(const Clock& time)
    {
        this->tclThreadManager_->update(time);
    }
}
