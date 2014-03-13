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
 *      ...
 *
 */

/**
    @file
    @ingroup Config ConfigFile
    @brief Declaration of ConfigFileManager, used to load and save config files.
*/

#ifndef _ConfigFileManager_H__
#define _ConfigFileManager_H__

#include "core/CorePrereqs.h"

#include <boost/array.hpp>

#include "util/Singleton.h"

namespace orxonox
{
    ///////////////////////
    // ConfigFileManager //
    ///////////////////////
    /**
        @brief Manages the different config files (settings, calibration, etc). Implemented as Singleton.
    */
    class _CoreExport ConfigFileManager : public Singleton<ConfigFileManager>
    {
        friend class Singleton<ConfigFileManager>;
        public:
            ConfigFileManager();
            ~ConfigFileManager();

            void setFilename(ConfigFileType::Value type, const std::string& filename);

            /// Returns the config file of a given type (settings, calibration, etc.)
            inline ConfigFile* getConfigFile(ConfigFileType::Value type)
            {
                // Check array bounds
                return configFiles_.at(type);
            }

        private:
            ConfigFileManager(const ConfigFileManager&);    ///< Copy-constructor: not implemented

            boost::array<ConfigFile*, 3> configFiles_;      ///< Stores the config files for each type in an array (must have the same size like ConfigFileType::Value)
            static ConfigFileManager* singletonPtr_s;       ///< Stores the singleton-pointer
    };
}

#endif /* _ConfigFileManager_H__ */
