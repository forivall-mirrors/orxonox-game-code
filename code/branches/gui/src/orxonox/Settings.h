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
    @file Core.h
    @brief Declaration of the Settings class.

    The static Settings class is only used to configure some variables
    through the config-file.
*/

#ifndef _Settings_H__
#define _Settings_H__

#include "OrxonoxPrereqs.h"
#include <string>
#include "core/OrxonoxClass.h"
#include "core/Debug.h"
#include "util/MultiTypeMath.h"
#include "util/Convert.h"

namespace orxonox
{
    /**
    @brief
        Defines a bit field structure that holds the mode as enum plus
        the attributes as single named bits.
        Every different GameMode is stored as static const, but if you wish to
        compare two modes, you will have to use the 'mode' member variable.
    */
    struct GameMode
    {
        enum Mode
        {
            None,
            Unspecified,
            Server,
            Client,
            Standalone,
            Dedicated,
        };

        Mode mode;
        bool showsGraphics;
        bool isMaster;
        bool hasServer;
        std::string name;

        static const GameMode GM_None;
        static const GameMode GM_Unspecified;
        static const GameMode GM_Server;
        static const GameMode GM_Client;
        static const GameMode GM_Standalone;
        static const GameMode GM_Dedicated;
    };


    class _OrxonoxExport Settings : public OrxonoxClass
    {
    public:
        struct CommandLineArgument
        {
            std::string name_;
            MultiTypeMath value_;
            bool bHasDefaultValue_;
        };

        void setConfigValues();

        static const std::string& getDataPath();
        static void tsetDataPath(const std::string& path);

        static const GameMode& getGameMode();
        static const GameMode& getGameMode(const std::string& name);
        static void setGameMode(const GameMode& mode);
        static void setGameMode(const std::string& mode);
        static bool addGameMode(const GameMode* mode);

        static const CommandLineArgument* getCommandLineArgument(const std::string& name);
        template <class T>
        static bool addCommandLineArgument(const std::string &name, const std::string& valueStr, const T& defaultValue);

    private:
        Settings();
        Settings(const Settings& instance);
        ~Settings() { }
        static Settings& getInstance();

        void _tsetDataPath(const std::string& path);

        std::string dataPath_;                                        //!< Path to the game data
        GameMode gameMode_;                                           //!< Current game mode
        std::map<std::string, const GameMode*> gameModes_;            //!< Holds all game modes for easy string access
        //! holds all command line arguments (even if not given!)
        std::map<std::string, CommandLineArgument> commandArguments_;
    };

    /**
    @brief
        Returns the relative path to the game data.
    */
    inline const std::string& Settings::getDataPath()
    {
        return getInstance().dataPath_;
    }

    inline void Settings::tsetDataPath(const std::string& path)
    {
        getInstance()._tsetDataPath(path);
    }

    inline const GameMode& Settings::getGameMode()
    {
        return getInstance().gameMode_;
    }

    inline const GameMode& Settings::getGameMode(const std::string& name)
    {
        if (getInstance().gameModes_.find(name) != getInstance().gameModes_.end())
            return *getInstance().gameModes_[name];
        else
        {
            COUT(2) << "Warning: GameMode '" << name << "' doesn't exist." << std::endl;
            return GameMode::GM_None;
        }
    }

    inline void Settings::setGameMode(const GameMode& mode)
    {
        getInstance().gameMode_ = mode;
    }

    /**
    @brief
        Adds one argument of the command line to the map of command line arguments.
    @param name
        Name of the command line option.
    @param valueStr
        The value of the command line option as string
    @param defaultValue
        Default value for the option (marked when used).
    @return
        Dummy return value to enable code execution before main().
    */
    template <class T>
    bool Settings::addCommandLineArgument(const std::string &name, const std::string& valueStr, const T& defaultValue)
    {
        T value;
        bool useDefault = false;
        if (valueStr == "")
        {
            // note: ArgReader only returns "" for not found arguments, " " otherwise for empty ones.
            value = defaultValue;
            useDefault = true;
        }
        else if (!convertValue(&value, valueStr))
        {
            COUT(1) << "Command Line: Couldn't read option '" << name << "'." << std::endl;
            return false;
        }
        CommandLineArgument arg = { name, MultiTypeMath(value), useDefault };
        getInstance().commandArguments_[name] = arg;
        return true;
    }
}

#endif /* _Settings_H__ */
