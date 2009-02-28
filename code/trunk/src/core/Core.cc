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
 *   Co-authors:
 *      Reto Grieder
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
#  include <windows.h>
#elif defined(ORXONOX_PLATFORM_APPLE)
#  include <sys/param.h>
#  include <mach-o/dyld.h>
#else /* Linux */
#  include <sys/types.h>
#  include <unistd.h>
#endif

#include "SpecialConfig.h"
#include "util/Exception.h"
#include "Language.h"
#include "CoreIncludes.h"
#include "ConfigValueIncludes.h"
#include "LuaBind.h"
#include "CommandLine.h"

namespace orxonox
{
    //! Path to the parent directory of the ones above if program was installed with relativ pahts
    static boost::filesystem::path rootPath_g;
    static boost::filesystem::path executablePath_g;            //!< Path to the executable
    static boost::filesystem::path mediaPath_g;                 //!< Path to the media file folder
    static boost::filesystem::path configPath_g;                //!< Path to the config file folder
    static boost::filesystem::path logPath_g;                   //!< Path to the log file folder

    bool Core::bShowsGraphics_s = false;
    bool Core::bHasServer_s     = false;
    bool Core::bIsClient_s      = false;
    bool Core::bIsStandalone_s  = false;
    bool Core::bIsMaster_s      = false;

    bool Core::isDevBuild_s     = false;
    Core* Core::singletonRef_s  = 0;

    SetCommandLineArgument(mediaPath, "").information("PATH");
    SetCommandLineArgument(directory, "").information("DIR");

    /**
        @brief Constructor: Registers the object and sets the config-values.
        @param A reference to a global variable, used to avoid an infinite recursion in getSoftDebugLevel()
    */
    Core::Core()
    {
        RegisterRootObject(Core);

        assert(Core::singletonRef_s == 0);
        Core::singletonRef_s = this;

        this->bInitializeRandomNumberGenerator_ = false;
        this->setConfigValues();

        // Set the correct log path. Before this call, /tmp (Unix) or %TEMP% was used
        OutputHandler::getOutStream().setLogPath(Core::getLogPathString());

        // Possible media path override by the command line
        if (!CommandLine::getArgument("mediaPath")->hasDefaultValue())
        {
            //std::string mediaPath = CommandLine::getValue("mediaPath");
            Core::tsetMediaPath(CommandLine::getValue("mediaPath"));
        }
    }

    /**
        @brief Sets the bool to true to avoid static functions accessing a deleted object.
    */
    Core::~Core()
    {
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

        SetConfigValue(mediaPathString_, Core::getMediaPathPOSIXString())
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
        return mediaPath_g.directory_string() + CP_SLASH;
    }
    /*static*/ std::string Core::getMediaPathPOSIXString()
    {
        return mediaPath_g.string() + '/';
        
    }

    /*static*/ const boost::filesystem::path& Core::getConfigPath()
    {
        return configPath_g;
    }
    /*static*/ std::string Core::getConfigPathString()
    {
        return configPath_g.directory_string() + CP_SLASH;
    }
    /*static*/ std::string Core::getConfigPathPOSIXString()
    {
        return configPath_g.string() + '/';
    }

    /*static*/ const boost::filesystem::path& Core::getLogPath()
    {
        return logPath_g;
    }
    /*static*/ std::string Core::getLogPathString()
    {
        return logPath_g.directory_string() + CP_SLASH;
    }
    /*static*/ std::string Core::getLogPathPOSIXString()
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
    @brief
        Performs the rather lower level operations just after
        int main() has been called.
    @remarks
        This gets called AFTER pre-main stuff like AddFactory,
        SetConsoleCommand, etc.
    */
    /*static*/ void Core::postMainInitialisation()
    {
        // set location of the executable
        Core::setExecutablePath();

        // Determine whether we have an installed or a binary dir run
        // The latter occurs when simply running from the build directory
        Core::checkDevBuild();

        // Make sure the directories we write in exist or else make them
        Core::createDirectories();
    }

    /**
    @brief
        Compares the executable path with the working directory
    */
    /*static*/ void Core::setExecutablePath()
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
    /*static*/ void Core::checkDevBuild()
    {
        if (boost::filesystem::exists(executablePath_g / "orxonox_dev_build.keep_me"))
        {
            COUT(1) << "Running from the build tree." << std::endl;
            Core::isDevBuild_s = true;
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
            while (!boost::filesystem::equivalent(rootPath_g / relativeExecutablePath, executablePath_g) || rootPath_g.empty())
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
        if (!CommandLine::getArgument("directory")->hasDefaultValue())
        {
            std::string directory(CommandLine::getValue("directory"));
            configPath_g = configPath_g / directory;
            logPath_g    = logPath_g    / directory;
        }
    }

    /*
    @brief
        Checks for the log and the config directory and creates them
        if necessary. Otherwise me might have problems opening those files.
    */
    /*static*/ void Core::createDirectories()
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
                                         Please remove " + it->first.file_string());
            }
            if (boost::filesystem::create_directories(it->first)) // function may not return true at all (bug?)
            {
                COUT(4) << "Created " << it->second << " directory" << std::endl;
            }
        }
    }
}
