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

#include "ApplicationPaths.h"

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

// Differentiate Boost Filesystem v2 and v3
#if (BOOST_FILESYSTEM_VERSION < 3)
#  define BF_LEAF leaf
#  define BF_GENERIC_STRING string
#else
#  define BF_LEAF path().filename().string
#  define BF_GENERIC_STRING generic_string
#endif

namespace orxonox
{
    namespace bf = boost::filesystem;

    //! Static pointer to the singleton
    ApplicationPaths* ApplicationPaths::singletonPtr_s  = 0;

    ApplicationPaths::ApplicationPaths()
        : rootPath_(*(new bf::path()))
        , executablePath_(*(new bf::path()))
        , modulePath_(*(new bf::path()))
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
            ApplicationPaths::bBuildDirectoryRun_ = true;
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

    ApplicationPaths::~ApplicationPaths()
    {
        delete &rootPath_;
        delete &executablePath_;
        delete &modulePath_;
    }

    std::vector<std::string> ApplicationPaths::getModulePaths()
    {
        std::vector<std::string> modulePaths;

        // We search for helper files with the following extension
        const std::string& moduleextension = specialConfig::moduleExtension;
        size_t moduleextensionlength = moduleextension.size();

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

    /*static*/ std::string ApplicationPaths::getRootPathString()
    {
        return getInstance().rootPath_.BF_GENERIC_STRING() + '/';
    }

    /*static*/ std::string ApplicationPaths::getExecutablePathString()
    {
        return getInstance().executablePath_.BF_GENERIC_STRING() + '/';
    }

    /*static*/ std::string ApplicationPaths::getModulePathString()
    {
        return getInstance().modulePath_.BF_GENERIC_STRING() + '/';
    }
}
