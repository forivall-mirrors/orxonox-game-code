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
    @brief Implementation of the Core class.
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
    //! Path to the parent directory of the ones above if program was installed with relativ pahts
    static boost::filesystem::path rootPath_g;
    static boost::filesystem::path executablePath_g;            //!< Path to the executable
    static boost::filesystem::path mediaPath_g;                 //!< Path to the media file folder
    static boost::filesystem::path configPath_g;                //!< Path to the config file folder
    static boost::filesystem::path logPath_g;                   //!< Path to the log file folder

    Core* Core::singletonRef_s  = 0;

    SetCommandLineArgument(mediaPath, "").information("PATH");
    SetCommandLineArgument(writingPathSuffix, "").information("DIR");
    SetCommandLineArgument(settingsFile, "orxonox.ini");
    SetCommandLineArgument(limitToCPU, 0).information("0: off | #cpu");

    Core::Core()
    {
        RegisterRootObject(Core);

        assert(Core::singletonRef_s == 0);
        Core::singletonRef_s = this;
    }

    void Core::initialise(int argc, char** argv)
    {
        // Parse command line arguments fist
        try
        {
            CommandLine::parseAll(argc, argv);
        }
        catch (ArgumentException& ex)
        {
            COUT(1) << ex.what() << std::endl;
            COUT(0) << "Usage:" << std::endl << "orxonox " << CommandLine::getUsageInformation() << std::endl;
        }

        // limit the main thread to the first core so that QueryPerformanceCounter doesn't jump
        // do this after ogre has initialised. Somehow Ogre changes the settings again (not through
        // the timer though).
        int limitToCPU = CommandLine::getValue("limitToCPU");
        if (limitToCPU > 0)
            setThreadAffinity((unsigned int)limitToCPU);

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
        this->signalHandler_->doCatch(executablePath_g.string(), Core::getLogPathString() + "orxonox_crash.log");

        // Set the correct log path. Before this call, /tmp (Unix) or %TEMP% was used
        OutputHandler::getOutStream().setLogPath(Core::getLogPathString());

        // Manage ini files and set the default settings file (usually orxonox.ini)
        this->configFileManager_ = new ConfigFileManager();
        this->configFileManager_->setFilename(ConfigFileType::Settings,
            CommandLine::getValue("settingsFile").getString());

        this->languageInstance_ = new Language();

        // Do this soon after the ConfigFileManager has been created to open up the
        // possibility to configure everything below here
        this->setConfigValues();

        // Possible media path override by the command line
        if (!CommandLine::getArgument("mediaPath")->hasDefaultValue())
        {
            //std::string mediaPath = CommandLine::getValue("mediaPath");
            Core::tsetMediaPath(CommandLine::getValue("mediaPath"));
        }

        // Create the lua interface
        this->luaBind_ = new LuaBind();

        // initialise Tcl
        this->tclBind_ = new TclBind(Core::getMediaPathString());
        this->tclThreadManager_ = new TclThreadManager(tclBind_->getTclInterpreter());

        // create a shell
        this->shell_ = new Shell();

        // creates the class hierarchy for all classes with factories
        Factory::createClassHierarchy();
        
        this->loaded_ = true;
    }

    /**
        @brief Sets the bool to true to avoid static functions accessing a deleted object.
    */
    Core::~Core()
    {
        this->loaded_ = false;

        delete this->shell_;
        delete this->tclThreadManager_;
        delete this->tclBind_;
        delete this->luaBind_;
        delete this->languageInstance_;
        delete this->configFileManager_;
        delete this->signalHandler_;

        // Destroy command line arguments
        CommandLine::destroyAllArguments();
        // Also delete external console command that don't belong to an Identifier
        CommandExecutor::destroyExternalCommands();

        assert(Core::singletonRef_s);
        Core::singletonRef_s = 0;
    }

    /**
        @brief Function to collect the SetConfigValue-macro calls.
    */
    void Core::setConfigValues()
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
            .description("The maximal level of debug output shown in the console").callback(this, &Core::debugLevelChanged);
        SetConfigValue(softDebugLevelLogfile_, defaultLevelLogfile)
            .description("The maximal level of debug output shown in the logfile").callback(this, &Core::debugLevelChanged);
        SetConfigValue(softDebugLevelShell_, defaultLevelShell)
            .description("The maximal level of debug output shown in the ingame shell").callback(this, &Core::debugLevelChanged);

        SetConfigValue(language_, Language::getLanguage().defaultLanguage_).description("The language of the ingame text").callback(this, &Core::languageChanged);
        SetConfigValue(bInitializeRandomNumberGenerator_, true).description("If true, all random actions are different each time you start the game").callback(this, &Core::initializeRandomNumberGenerator);

        SetConfigValue(mediaPathString_, mediaPath_g.string())
            .description("Relative path to the game data.").callback(this, &Core::mediaPathChanged);
    }

    /**
        @brief Callback function if the debug level has changed.
    */
    void Core::debugLevelChanged()
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
    void Core::languageChanged()
    {
        // Read the translation file after the language was configured
        Language::getLanguage().readTranslatedLanguageFile();
    }

    /**
    @brief
        Callback function if the media path has changed.
    */
    void Core::mediaPathChanged()
    {
        mediaPath_g = boost::filesystem::path(this->mediaPathString_);
    }

    /**
        @brief Returns the softDebugLevel for the given device (returns a default-value if the class ist right about to be created).
        @param device The device
        @return The softDebugLevel
    */
    int Core::getSoftDebugLevel(OutputHandler::OutputDevice device)
    {
        switch (device)
        {
        case OutputHandler::LD_All:
            return Core::getInstance().softDebugLevel_;
        case OutputHandler::LD_Console:
            return Core::getInstance().softDebugLevelConsole_;
        case OutputHandler::LD_Logfile:
            return Core::getInstance().softDebugLevelLogfile_;
        case OutputHandler::LD_Shell:
            return Core::getInstance().softDebugLevelShell_;
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
     void Core::setSoftDebugLevel(OutputHandler::OutputDevice device, int level)
     {
        if (device == OutputHandler::LD_All)
            Core::getInstance().softDebugLevel_ = level;
        else if (device == OutputHandler::LD_Console)
            Core::getInstance().softDebugLevelConsole_ = level;
        else if (device == OutputHandler::LD_Logfile)
            Core::getInstance().softDebugLevelLogfile_ = level;
        else if (device == OutputHandler::LD_Shell)
            Core::getInstance().softDebugLevelShell_ = level;

        OutputHandler::setSoftDebugLevel(device, level);
     }

    /**
        @brief Returns the configured language.
    */
    const std::string& Core::getLanguage()
    {
        return Core::getInstance().language_;
    }

    /**
        @brief Sets the language in the config-file back to the default.
    */
    void Core::resetLanguage()
    {
        Core::getInstance().resetLanguageIntern();
    }

    /**
        @brief Sets the language in the config-file back to the default.
    */
    void Core::resetLanguageIntern()
    {
        ResetConfigValue(language_);
    }

    /**
    @brief
        Temporary sets the media path
    @param path
        The new media path
    */
    void Core::_tsetMediaPath(const std::string& path)
    {
        ModifyConfigValue(mediaPathString_, tset, path);
    }

    /*static*/ const boost::filesystem::path& Core::getMediaPath()
    {
        return mediaPath_g;
    }
    /*static*/ std::string Core::getMediaPathString()
    {
        return mediaPath_g.string() + '/';
    }

    /*static*/ const boost::filesystem::path& Core::getConfigPath()
    {
        return configPath_g;
    }
    /*static*/ std::string Core::getConfigPathString()
    {
        return configPath_g.string() + '/';
    }

    /*static*/ const boost::filesystem::path& Core::getLogPath()
    {
        return logPath_g;
    }
    /*static*/ std::string Core::getLogPathString()
    {
        return logPath_g.string() + '/';
    }

    void Core::initializeRandomNumberGenerator()
    {
        static bool bInitialized = false;
        if (!bInitialized && this->bInitializeRandomNumberGenerator_)
        {
            srand(time(0));
            rand();
            bInitialized = true;
        }
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

        executablePath_g = boost::filesystem::path(buffer);
#ifndef ORXONOX_PLATFORM_APPLE
        executablePath_g = executablePath_g.branch_path(); // remove executable name
#endif
    }

    /**
    @brief
        Checks for "orxonox_dev_build.keep_me" in the executable diretory.
        If found it means that this is not an installed run, hence we
        don't write the logs and config files to ~/.orxonox
    */
    void Core::checkDevBuild()
    {
        if (boost::filesystem::exists(executablePath_g / "orxonox_dev_build.keep_me"))
        {
            COUT(1) << "Running from the build tree." << std::endl;
            Core::isDevBuild_ = true;
            mediaPath_g  = ORXONOX_MEDIA_DEV_PATH;
            configPath_g = ORXONOX_CONFIG_DEV_PATH;
            logPath_g    = ORXONOX_LOG_DEV_PATH;
        }
        else
        {
#ifdef INSTALL_COPYABLE // --> relative paths
            // Also set the root path
            boost::filesystem::path relativeExecutablePath(ORXONOX_RUNTIME_INSTALL_PATH);
            rootPath_g = executablePath_g;
            while (!boost::filesystem::equivalent(rootPath_g / relativeExecutablePath, executablePath_g) && !rootPath_g.empty())
                rootPath_g = rootPath_g.branch_path();
            if (rootPath_g.empty())
                ThrowException(General, "Could not derive a root directory. Might the binary installation directory contain '..' when taken relative to the installation prefix path?");

            // Using paths relative to the install prefix, complete them
            mediaPath_g  = rootPath_g / ORXONOX_MEDIA_INSTALL_PATH;
            configPath_g = rootPath_g / ORXONOX_CONFIG_INSTALL_PATH;
            logPath_g    = rootPath_g / ORXONOX_LOG_INSTALL_PATH;
#else
            // There is no root path, so don't set it at all

            mediaPath_g  = ORXONOX_MEDIA_INSTALL_PATH;

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

            configPath_g = userDataPath / ORXONOX_CONFIG_INSTALL_PATH;
            logPath_g    = userDataPath / ORXONOX_LOG_INSTALL_PATH;
#endif
        }

        // Option to put all the config and log files in a separate folder
        if (!CommandLine::getArgument("writingPathSuffix")->hasDefaultValue())
        {
            std::string directory(CommandLine::getValue("writingPathSuffix").getString());
            configPath_g = configPath_g / directory;
            logPath_g    = logPath_g    / directory;
        }
    }

    /*
    @brief
        Checks for the log and the config directory and creates them
        if necessary. Otherwise me might have problems opening those files.
    */
    void Core::createDirectories()
    {
        std::vector<std::pair<boost::filesystem::path, std::string> > directories;
        directories.push_back(std::pair<boost::filesystem::path, std::string>
            (boost::filesystem::path(configPath_g), "config"));
        directories.push_back(std::pair<boost::filesystem::path, std::string>
            (boost::filesystem::path(logPath_g),    "log"));

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
