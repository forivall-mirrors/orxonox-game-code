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

#include "PathConfig.h"

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <vector>
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
#include "util/Output.h"
#include "util/Exception.h"
#include "CommandLineParser.h"

// Differentiate Boost Filesystem v2 and v3
#if (BOOST_FILESYSTEM_VERSION < 3)
#  define BF_LEAF leaf
#  define BF_GENERIC_STRING string
#  define BF_NATIVE_STRING file_string
#else
#  define BF_LEAF path().filename().string
#  define BF_GENERIC_STRING generic_string
#  define BF_NATIVE_STRING string
#endif

namespace orxonox
{
    namespace bf = boost::filesystem;

    //! Static pointer to the singleton
    PathConfig* PathConfig::singletonPtr_s  = 0;

    SetCommandLineArgument(externalDataPath, "").information("Path to the external data files");
    SetCommandLineArgument(writingPathSuffix, "").information("Additional subfolder for config and log files");

    PathConfig::PathConfig()
        : rootPath_(*(new bf::path()))
        , executablePath_(*(new bf::path()))
        , modulePath_(*(new bf::path()))
        , dataPath_(*(new bf::path()))
        , externalDataPath_(*(new bf::path()))
        , configPath_(*(new bf::path()))
        , logPath_(*(new bf::path()))
        , bBuildDirectoryRun_(false)
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
        uint32_t path_len = 1023;
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

        // Remove executable filename
        executablePath_ = bf::path(buffer).branch_path();

        /////////////////////
        // SET MODULE PATH //
        /////////////////////

        if (bf::exists(executablePath_ / "orxonox_dev_build.keep_me"))
        {
            orxout(internal_info) << "Running from the build tree." << endl;
            PathConfig::bBuildDirectoryRun_ = true;
            modulePath_ = specialConfig::moduleDevDirectory;
        }
        else
        {

#ifdef INSTALL_COPYABLE // --> relative paths

            // Also set the root path
            bf::path relativeExecutablePath(specialConfig::defaultRuntimePath);
            rootPath_ = executablePath_;
            while (!bf::equivalent(rootPath_ / relativeExecutablePath, executablePath_) && !rootPath_.empty())
                rootPath_ = rootPath_.branch_path();
            if (rootPath_.empty())
                ThrowException(General, "Could not derive a root directory. Might the binary installation directory contain '..' when taken relative to the installation prefix path?");

            // Module path is fixed as well
            modulePath_ = rootPath_ / specialConfig::defaultModulePath;

#else

            // There is no root path, so don't set it at all
            // Module path is fixed as well
            modulePath_ = specialConfig::moduleInstallDirectory;

#endif
        }
    }

    PathConfig::~PathConfig()
    {
        delete &rootPath_;
        delete &executablePath_;
        delete &modulePath_;
        delete &dataPath_;
        delete &externalDataPath_;
        delete &configPath_;
        delete &logPath_;
    }

    void PathConfig::setConfigurablePaths()
    {
        if (bBuildDirectoryRun_)
        {
            dataPath_         = specialConfig::dataDevDirectory;
            configPath_       = specialConfig::configDevDirectory;
            logPath_          = specialConfig::logDevDirectory;

            // Check for data path override by the command line
            if (!CommandLineParser::getArgument("externalDataPath")->hasDefaultValue())
                externalDataPath_ = CommandLineParser::getValue("externalDataPath").get<std::string>();
            else
                externalDataPath_ = specialConfig::externalDataDevDirectory;
        }
        else
        {

#ifdef INSTALL_COPYABLE // --> relative paths

            // Using paths relative to the install prefix, complete them
            dataPath_   = rootPath_ / specialConfig::defaultDataPath;
            configPath_ = rootPath_ / specialConfig::defaultConfigPath;
            logPath_    = rootPath_ / specialConfig::defaultLogPath;

#else

            dataPath_  = specialConfig::dataInstallDirectory;

            // Get user directory
#ifdef ORXONOX_PLATFORM_UNIX
            char* userDataPathPtr(getenv("HOME"));
#else
            char* userDataPathPtr(getenv("APPDATA"));
#endif
            if (userDataPathPtr == NULL)
                ThrowException(General, "Could not retrieve user data path.");
            bf::path userDataPath(userDataPathPtr);
            userDataPath /= ".orxonox";

            configPath_ = userDataPath / specialConfig::defaultConfigPath;
            logPath_    = userDataPath / specialConfig::defaultLogPath;

#endif

        }

        // Option to put all the config and log files in a separate folder
        if (!CommandLineParser::getArgument("writingPathSuffix")->hasDefaultValue())
        {
            const std::string& directory(CommandLineParser::getValue("writingPathSuffix").get<std::string>());
            configPath_ = configPath_ / directory;
            logPath_    = logPath_    / directory;
        }

        // Create directories to avoid problems when opening files in non existent folders.
        std::vector<std::pair<bf::path, std::string> > directories;
        directories.push_back(std::make_pair(bf::path(configPath_), std::string("config")));
        directories.push_back(std::make_pair(bf::path(logPath_), std::string("log")));

        for (std::vector<std::pair<bf::path, std::string> >::iterator it = directories.begin();
            it != directories.end(); ++it)
        {
            if (bf::exists(it->first) && !bf::is_directory(it->first))
            {
                ThrowException(General, std::string("The ") + it->second + " directory has been preoccupied by a file! \
                                         Please remove " + it->first.BF_GENERIC_STRING());
            }
            if (bf::create_directories(it->first)) // function may not return true at all (bug?)
            {
                orxout(internal_info) << "Created " << it->second << " directory" << endl;
            }
        }
    }

    std::vector<std::string> PathConfig::getModulePaths()
    {
        std::vector<std::string> modulePaths;

        // We search for helper files with the following extension
        const std::string& moduleextension = specialConfig::moduleExtension;
        size_t moduleextensionlength = moduleextension.size();

#ifdef ORXONOX_PLATFORM_WINDOWS
        // Add that path to the PATH variable in case a module depends on another one
        const char* currentPATH = getenv("PATH");
        std::string newPATH = modulePath_.BF_NATIVE_STRING();
        if (currentPATH != NULL)
            newPATH = std::string(currentPATH) + ';' + newPATH;
        putenv(const_cast<char*>(("PATH=" + newPATH).c_str()));
#endif

        // Make sure the path exists, otherwise don't load modules
        if (!boost::filesystem::exists(modulePath_))
            return modulePaths;

        boost::filesystem::directory_iterator file(modulePath_);
        boost::filesystem::directory_iterator end;

        // Iterate through all files
        while (file != end)
        {
            std::string filename = file->BF_LEAF();

            // Check if the file ends with the extension in question
            if (filename.size() > moduleextensionlength)
            {
                if (filename.substr(filename.size() - moduleextensionlength) == moduleextension)
                {
                    // We've found a helper file
                    const std::string& library = filename.substr(0, filename.size() - moduleextensionlength);
                    modulePaths.push_back(getModulePathString() + library);
                }
            }
            ++file;
        }

        return modulePaths;
    }

    /*static*/ std::string PathConfig::getRootPathString()
    {
        return getInstance().rootPath_.BF_GENERIC_STRING() + '/';
    }

    /*static*/ std::string PathConfig::getExecutablePathString()
    {
        return getInstance().executablePath_.BF_GENERIC_STRING() + '/';
    }

    /*static*/ std::string PathConfig::getDataPathString()
    {
        return getInstance().dataPath_.BF_GENERIC_STRING() + '/';
    }

    /*static*/ std::string PathConfig::getExternalDataPathString()
    {
        return getInstance().externalDataPath_.BF_GENERIC_STRING() + '/';
    }

    /*static*/ std::string PathConfig::getConfigPathString()
    {
        return getInstance().configPath_.BF_GENERIC_STRING() + '/';
    }

    /*static*/ std::string PathConfig::getLogPathString()
    {
        return getInstance().logPath_.BF_GENERIC_STRING() + '/';
    }

    /*static*/ std::string PathConfig::getModulePathString()
    {
        return getInstance().modulePath_.BF_GENERIC_STRING() + '/';
    }
}
