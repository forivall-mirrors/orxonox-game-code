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

#include "ConfigurablePaths.h"

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
#include "commandline/CommandLineIncludes.h"

// Differentiate Boost Filesystem v2 and v3
#if (BOOST_FILESYSTEM_VERSION < 3)
#  define BF_GENERIC_STRING string
#else
#  define BF_GENERIC_STRING generic_string
#endif

namespace orxonox
{
    namespace bf = boost::filesystem;

    //! Static pointer to the singleton
    ConfigurablePaths* ConfigurablePaths::singletonPtr_s  = 0;

    SetCommandLineArgument(externalDataPath, "").information("Path to the external data files");
    SetCommandLineArgument(writingPathSuffix, "").information("Additional subfolder for config and log files");

    ConfigurablePaths::ConfigurablePaths()
        : dataPath_(*(new bf::path()))
        , externalDataPath_(*(new bf::path()))
        , configPath_(*(new bf::path()))
        , logPath_(*(new bf::path()))
    {
    }

    ConfigurablePaths::~ConfigurablePaths()
    {
        delete &dataPath_;
        delete &externalDataPath_;
        delete &configPath_;
        delete &logPath_;
    }

    void ConfigurablePaths::setConfigurablePaths(const ApplicationPaths& applicationPaths)
    {
        if (applicationPaths.buildDirectoryRun())
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
            dataPath_   = applicationPaths.getRootPath() / specialConfig::defaultDataPath;
            configPath_ = applicationPaths.getRootPath() / specialConfig::defaultConfigPath;
            logPath_    = applicationPaths.getRootPath() / specialConfig::defaultLogPath;

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

    /*static*/ std::string ConfigurablePaths::getDataPathString()
    {
        return getInstance().dataPath_.BF_GENERIC_STRING() + '/';
    }

    /*static*/ std::string ConfigurablePaths::getExternalDataPathString()
    {
        return getInstance().externalDataPath_.BF_GENERIC_STRING() + '/';
    }

    /*static*/ std::string ConfigurablePaths::getConfigPathString()
    {
        return getInstance().configPath_.BF_GENERIC_STRING() + '/';
    }

    /*static*/ std::string ConfigurablePaths::getLogPathString()
    {
        return getInstance().logPath_.BF_GENERIC_STRING() + '/';
    }
}
