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
    @brief Implementation of the Settings class.
*/

#include "OrxonoxStableHeaders.h"
#include "Settings.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"

namespace orxonox
{
  /**
    @brief Constructor: Registers the object and sets the config-values.
  */
  Settings::Settings()
  {
    RegisterRootObject(Settings);
    setConfigValues();
  }

  Settings::~Settings()
  {
  }

  /**
    @brief Returns a unique instance of Core.
    @return The instance
  */
  Settings& Settings::getSingleton()
  {
    static Settings instance;
    return instance;
  }

  /**
    @brief Function to collect the SetConfigValue-macro calls.
  */
  void Settings::setConfigValues()
  {
    SetConfigValue(dataPath_, "../../Media/").description("Relative path to the game data.").callback(this, &Settings::dataPathChanged);
  }

  /**
    @brief Callback function if the datapath has changed.
  */
  void Settings::dataPathChanged()
  {
    if (dataPath_ != "" && dataPath_[dataPath_.size() - 1] != '/')
    {
      ModifyConfigValue(dataPath_, set, dataPath_ + "/");
    }

    if (dataPath_ == "")
    {
      ModifyConfigValue(dataPath_, set, "/");
      COUT(2) << "Warning: Data path set to \"/\", is that really correct?" << std::endl;
    }
  }

  /**
    @brief Temporary sets the data path
    @param path The new data path
  */
  void Settings::_tsetDataPath(const std::string& path)
  {
    ModifyConfigValue(dataPath_, tset, path);
  }

  /*static*/ void Settings::tsetDataPath(const std::string& path)
  {
    getSingleton()._tsetDataPath(path);
  }

  /**
    @brief Returns the relative path to the game data.
    @return The path to the game data
  */
  /*static*/ const std::string& Settings::getDataPath()
  {
    return getSingleton().dataPath_;
  }
}
