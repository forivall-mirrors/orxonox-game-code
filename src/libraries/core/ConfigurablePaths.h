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

#ifndef _ConfigurablePaths_H__
#define _ConfigurablePaths_H__

#include "CorePrereqs.h"

#include <string>
#include "util/Singleton.h"

//tolua_begin
namespace orxonox
{
//tolua_end
    /**
    @brief
        The ConfigurablePaths class is a singleton used to configure different paths.
    @details
        The class provides information about the data, config, and log path.
    @remarks
        Not all paths are always available:
        - externalData only for development builds in the build tree
    */
    class _CoreExport ConfigurablePaths //tolua_export
        : public Singleton<ConfigurablePaths>
    { //tolua_export
        friend class Singleton<ConfigurablePaths>;

        public:
            ConfigurablePaths();
            ~ConfigurablePaths();

            //! Returns the path to the data files as boost::filesystem::path
            static const boost::filesystem::path& getDataPath()
                { return getInstance().dataPath_; }
            //! Returns the path to the external data files as boost::filesystem::path
            static const boost::filesystem::path& getExternalDataPath()
                { return getInstance().externalDataPath_; }
            //! Returns the path to the config files as boost::filesystem::path
            static const boost::filesystem::path& getConfigPath()
                { return getInstance().configPath_; }
            //! Returns the path to the log files as boost::filesystem::path
            static const boost::filesystem::path& getLogPath()
                { return getInstance().logPath_; }

            //! Returns the path to the data files as std::string
            static std::string getDataPathString();
            //! Returns the path to the external data files as std::string
            static std::string getExternalDataPathString();
            //! Returns the path to the config files as std::string
            static std::string getConfigPathString(); //tolua_export
            //! Returns the path to the log files as std::string
            static std::string getLogPathString();

            /**
            @brief
                Sets config, log and media path and creates the folders if necessary.
            @throws
                GeneralException
            */
            void setConfigurablePaths(const ApplicationPaths& applicationPaths);

        private:
            ConfigurablePaths(const ConfigurablePaths&); //!< Don't use (undefined symbol)

            boost::filesystem::path& dataPath_;              //!< Path to the data files folder
            boost::filesystem::path& externalDataPath_;      //!< Path to the external data files folder
            boost::filesystem::path& configPath_;            //!< Path to the config files folder
            boost::filesystem::path& logPath_;               //!< Path to the log files folder

            static ConfigurablePaths* singletonPtr_s;
    }; //tolua_export
} //tolua_export

#endif /* _ConfigurablePaths_H__ */
