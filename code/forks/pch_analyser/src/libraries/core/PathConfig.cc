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

namespace orxonox
{
    //! Static pointer to the singleton
    PathConfig* PathConfig::singletonPtr_s  = 0;

    PathConfig::PathConfig()
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

        executablePath_ = QDir(buffer);
#ifndef ORXONOX_PLATFORM_APPLE
        executablePath_.cdUp(); // remove executable name
#endif

        if (executablePath_.exists("orxonox_dev_build.keep_me"))
        {
            COUT(1) << "Running from the build tree." << std::endl;
            PathConfig::bDevRun_ = true;
        }
        else
        {

#ifdef INSTALL_COPYABLE // --> relative paths

            // Also set the root path
            QDir relativeExecutablePath(specialConfig::defaultRuntimePath);
            rootPath_ = executablePath_;
            while (rootPath_ / relativeExecutablePath != executablePath_)
            {
                if (!rootPath_.cdUp())
                    ThrowException(General, "Could not derive a root directory. Might the binary installation directory contain '..' when taken relative to the installation prefix path?");
            }

#else

            // There is no root path, so don't set it at all

#endif
        }
    }

    PathConfig::~PathConfig()
    {
    }

    void PathConfig::setConfigurablePaths()
    {
        if (bDevRun_)
        {
            dataPath_         = specialConfig::dataDevDirectory;
            configPath_       = specialConfig::configDevDirectory;
            logPath_          = specialConfig::logDevDirectory;
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
#  ifdef ORXONOX_PLATFORM_UNIX /* Apple? */
            char* userDataPathPtr(getenv("HOME"));
#  else
            char* userDataPathPtr(getenv("APPDATA"));
#  endif
            if (userDataPathPtr == NULL)
                ThrowException(General, "Could not retrieve user data path.");
            QDir userDataPath(userDataPathPtr);
            userDataPath /= ".orxonox";

            configPath_ = userDataPath / specialConfig::defaultConfigPath;
            logPath_    = userDataPath / specialConfig::defaultLogPath;

#endif

        }

        // Create directories to avoid problems when opening files in non existent folders.
        std::vector<std::pair<QDir, std::string> > directories;
        directories.push_back(std::make_pair(QDir(configPath_), "config"));
        directories.push_back(std::make_pair(QDir(logPath_), "log"));

        for (std::vector<std::pair<QDir, std::string> >::iterator it = directories.begin();
            it != directories.end(); ++it)
        {
            if (!it->first.exists())
            {
                if (!it->first.mkpath("."))
                    ThrowException(General, std::string("The ") + it->second + " directory could not be created.");
                COUT(4) << "Created " << it->second << " directory" << std::endl;
            }
        }
    }

    /*static*/ std::string PathConfig::getRootPathString()
    {
        return getInstance().rootPath_.path().toStdString() + '/';
    }

    /*static*/ std::string PathConfig::getExecutablePathString()
    {
        return getInstance().executablePath_.path().toStdString() + '/';
    }

    /*static*/ std::string PathConfig::getDataPathString()
    {
        return getInstance().dataPath_.path().toStdString() + '/';
    }

    /*static*/ std::string PathConfig::getConfigPathString()
    {
        return getInstance().configPath_.path().toStdString() + '/';
    }

    /*static*/ std::string PathConfig::getLogPathString()
    {
        return getInstance().logPath_.path().toStdString() + '/';
    }
}
