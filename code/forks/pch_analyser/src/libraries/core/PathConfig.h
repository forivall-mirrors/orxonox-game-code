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

#ifndef _PathConfig_H__
#define _PathConfig_H__

#include "CorePrereqs.h"

#include <string>
#include <vector>
#include <QDir>
#include "util/Singleton.h"

namespace orxonox
{
    /**
    @brief
        The PathConfig class is a singleton used to configure different paths.
    @details
        The class provides information about the data, config, log, executable
        and root path.
        It determines those by the use of platform specific functions.
    @remarks
        Not all paths are always available:
        - root only when installed copyable
    */
    class _CoreExport PathConfig : public Singleton<PathConfig>
    {
        friend class Singleton<PathConfig>;
        friend class Core;

        public:
            /**
            @brief
                Retrieves the executable path and sets all hard coded fixed paths (currently only the module path)
                Also checks for "orxonox_dev_build.keep_me" in the executable diretory.
                If found it means that this is not an installed run, hence we
                don't write the logs and config files to ~/.orxonox
            @throw
                GeneralException
            */
            PathConfig();
            ~PathConfig();

            //! Returns the path to the root folder as QDir
            static const QDir& getRootPath()
                { return getInstance().rootPath_; }
            //! Returns the path to the executable folder as QDir
            static const QDir& getExecutablePath()
                { return getInstance().executablePath_; }
            //! Returns the path to the data files as QDir
            static const QDir& getDataPath()
                { return getInstance().dataPath_; }
            //! Returns the path to the config files as QDir
            static const QDir& getConfigPath()
                { return getInstance().configPath_; }
            //! Returns the path to the log files as QDir
            static const QDir& getLogPath()
                { return getInstance().logPath_; }

            //! Returns the path to the root folder as std::string
            static std::string getRootPathString();
            //! Returns the path to the executable folder as std::string
            static std::string getExecutablePathString();
            //! Returns the path to the data files as std::string
            static std::string getDataPathString();
            //! Returns the path to the config files as std::string
            static std::string getConfigPathString(); //tolua_export
            //! Returns the path to the log files as std::string
            static std::string getLogPathString();

            //! Return trrue for runs in the build directory (not installed)
            static bool isDevelopmentRun() { return getInstance().bDevRun_; }

        private:
            PathConfig(const PathConfig&); //!< Don't use (undefined symbol)

            /**
            @brief
                Sets config, log and media path and creates the folders if necessary.
            @throws
                GeneralException
            */
            void setConfigurablePaths();

            //! Path to the parent directory of the ones above if program was installed with relativ paths
            QDir               rootPath_;
            QDir               executablePath_;        //!< Path to the executable
            QDir               dataPath_;              //!< Path to the data files folder
            QDir               configPath_;            //!< Path to the config files folder
            QDir               logPath_;               //!< Path to the log files folder

            bool               bDevRun_;               //!< True for runs in the build directory (not installed)
            static PathConfig* singletonPtr_s;
    }; //tolua_export
} //tolua_export

#endif /* _PathConfig_H__ */
