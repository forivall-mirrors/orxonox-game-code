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

/**
    @file
    @ingroup Management Resources
*/

#ifndef _ApplicationPaths_H__
#define _ApplicationPaths_H__

#include "CorePrereqs.h"

#include <string>
#include <vector>
#include "util/Singleton.h"

//tolua_begin
namespace orxonox
{
//tolua_end
    /**
    @brief
        The ApplicationPaths class is a singleton which provides static paths of the application.
    @details
        The class provides information about the executable, root and module path.
        It determines those by the use of platform specific functions.
    @remarks
        Not all paths are always available:
        - root only when installed copyable
    */
    class _CoreExport ApplicationPaths //tolua_export
        : public Singleton<ApplicationPaths>
    { //tolua_export
        friend class Singleton<ApplicationPaths>;

        public:
            /**
            @brief
                Retrieves the executable path and sets all hard coded fixed paths (currently only the module path)
                Also checks for "orxonox_dev_build.keep_me" in the executable directory.
                If found it means that this is not an installed run, hence we
                don't write the logs and config files to ~/.orxonox
            @throw
                GeneralException
            */
            ApplicationPaths();
            ~ApplicationPaths();

            //! Returns the path to the root folder as boost::filesystem::path
            static const boost::filesystem::path& getRootPath()
                { return getInstance().rootPath_; }
            //! Returns the path to the executable folder as boost::filesystem::path
            static const boost::filesystem::path& getExecutablePath()
                { return getInstance().executablePath_; }
            //! Returns the path to the modules as boost::filesystem::path
            static const boost::filesystem::path& getModulePath()
                { return getInstance().modulePath_; }

            //! Returns the path to the root folder as std::string
            static std::string getRootPathString();
            //! Returns the path to the executable folder as std::string
            static std::string getExecutablePathString();
            //! Returns the path to the modules as std::string
            static std::string getModulePathString();

            //! Return true for runs in the build directory (not installed)
            static bool buildDirectoryRun() { return getInstance().bBuildDirectoryRun_; }

            //! Returns a list with all modules declared by a *.module file in the module folder.
            std::vector<std::string> getModulePaths();

        private:
            ApplicationPaths(const ApplicationPaths&); //!< Don't use (undefined symbol)

            //! Path to the parent directory of the ones above if program was installed with relative paths
            boost::filesystem::path& rootPath_;
            boost::filesystem::path& executablePath_;        //!< Path to the executable
            boost::filesystem::path& modulePath_;            //!< Path to the modules

            bool                     bBuildDirectoryRun_;    //!< True for runs in the build directory (not installed)
            static ApplicationPaths* singletonPtr_s;
    }; //tolua_export
} //tolua_export

#endif /* _ApplicationPaths_H__ */
