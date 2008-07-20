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
@brief
    Implementation of the Settings class.
*/

#include "OrxonoxStableHeaders.h"
#include "Settings.h"

#include "util/String.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"

namespace orxonox
{

// Using a macro makes the above list much more readable.
// Settings::addGameMode adds the mode in a map, so we can access game modes by string.
#define CreateGameMode(name, showsGraphics, isMaster, hasServer)                                        \
    const GameMode GameMode::GM_##name = { GameMode::name, showsGraphics, isMaster, hasServer, #name }; \
    bool temporaryVariable##name = Settings::addGameMode(&GameMode::GM_##name)

    //                          showsGraphics  isMaster  hasServer
    CreateGameMode(None,        false,         false,    false);
    CreateGameMode(Unspecified, true,          false,    false);
    CreateGameMode(Server,      true,          true,     true );
    CreateGameMode(Client,      true,          false,    false);
    CreateGameMode(Standalone,  true,          true,     false);
    CreateGameMode(Dedicated,   false,         true,     true );

    /**
    @brief
        Constructor: Registers the object and sets the config-values.
    */
    Settings::Settings()
    {
        RegisterRootObject(Settings);
        gameMode_ = GameMode::GM_Unspecified;
        setConfigValues();
    }

    /**
    @brief
        Returns a unique instance of Core.
    @return
        The instance
    */
    Settings& Settings::getInstance()
    {
        static Settings instance;
        return instance;
    }

    /**
    @brief
        Function to collect the SetConfigValue-macro calls.
    */
    void Settings::setConfigValues()
    {
        SetConfigValue(dataPath_, "../../Media/").description("Relative path to the game data.");
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
    @brief
        Temporary sets the data path
    @param path
        The new data path
    */
    void Settings::_tsetDataPath(const std::string& path)
    {
        ModifyConfigValue(dataPath_, tset, path);
    }

    /**
    @brief
        Sets the game mode.
    */
    /*static*/ void Settings::setGameMode(const std::string& mode)
    {
        std::string modeL = getLowercase(mode);
        std::map<std::string, const GameMode*>::const_iterator it = getInstance().gameModes_.find(modeL);
        if (it != getInstance().gameModes_.end())
            getInstance().gameMode_ = *(*it).second;
        else
        {
            COUT(2) << "Warning: mode \"" << mode << "\" doesn't exist. "
                    << "Defaulting to 'Standalone'" << std::endl;
            getInstance().gameMode_ = GameMode::GM_Standalone;
        }
    }

    /*static*/ bool Settings::addGameMode(const GameMode* mode)
    {
        getInstance().gameModes_[getLowercase(mode->name)] = mode;
        return true;
    }


    /**
    @brief
        Gets an argument from the command line by name.
    @return
        Is 0 if name was not found.
    */
    /*static*/ const Settings::CommandLineArgument* Settings::getCommandLineArgument(const std::string &name)
    {
        std::map<std::string, CommandLineArgument>::const_iterator it = getInstance().commandArguments_.find(name);
        if (it != getInstance().commandArguments_.end())
        {
            return &((*it).second);
        }
        else
            return 0;
    }

}
