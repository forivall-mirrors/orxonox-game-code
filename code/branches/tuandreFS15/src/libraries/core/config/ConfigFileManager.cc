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
    @brief Implementation of ConfigFileManager.
*/

#include "ConfigFileManager.h"

#include "SettingsConfigFile.h"

namespace orxonox
{
    ///////////////////////
    // ConfigFileManager //
    ///////////////////////

    ConfigFileManager* ConfigFileManager::singletonPtr_s = 0;

    /// Constructor: Initializes the array of config files with NULL.
    ConfigFileManager::ConfigFileManager()
    {
        this->configFiles_.assign(NULL);
    }

    /// Destructor: Deletes the config files.
    ConfigFileManager::~ConfigFileManager()
    {
        for (boost::array<ConfigFile*, 3>::const_iterator it = this->configFiles_.begin(); it != this->configFiles_.end(); ++it)
            if (*it)
                delete (*it);
    }

    /// Defines the file-name for the config file of a given type (settings, calibration, etc.).
    void ConfigFileManager::setFilename(ConfigFileType::Value type, const std::string& filename)
    {
        if (this->getConfigFile(type))
            delete this->configFiles_[type];
        // Create and load config file
        switch (type)
        {
        case ConfigFileType::Settings:
            this->configFiles_[type] = new SettingsConfigFile(filename);
            break;
        case ConfigFileType::JoyStickCalibration:
        case ConfigFileType::CommandHistory:
            this->configFiles_[type] = new ConfigFile(filename);
            break;
        }
        this->configFiles_[type]->load();
    }
}
