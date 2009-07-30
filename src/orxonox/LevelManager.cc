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

#include "LevelManager.h"

#include <map>
#include <boost/filesystem.hpp>

#include "core/CommandLine.h"
#include "core/ConfigValueIncludes.h"
#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "core/Loader.h"
#include "PlayerManager.h"
#include "objects/Level.h"
#include "objects/infos/HumanPlayer.h"

namespace orxonox
{
    SetCommandLineArgument(level, "").shortcut("l").information("Default level file (overrides LevelManager::defaultLevelName_ configValue)");

    LevelManager* LevelManager::singletonPtr_s = 0;

    LevelManager::LevelManager()
    {
        RegisterRootObject(LevelManager);
        this->setConfigValues();

        // check override
        if (!CommandLine::getArgument("level")->hasDefaultValue())
        {
            ModifyConfigValue(defaultLevelName_, tset, CommandLine::getValue("level").getString());
        }
    }

    LevelManager::~LevelManager()
    {
    }

    void LevelManager::setConfigValues()
    {
        SetConfigValue(defaultLevelName_, "presentation_dm.oxw")
            .description("Sets the preselection of the level in the main menu.");
    }

    void LevelManager::requestActivity(Level* level)
    {
        this->levels_s.push_back(level);
        if (this->levels_s.size() == 1)
            this->activateNextLevel();
    }

    void LevelManager::releaseActivity(Level* level)
    {
        if (this->levels_s.size() > 0)
        {
            if (this->levels_s.front() == level)
            {
                level->setActive(false);
                this->levels_s.pop_front();
                this->activateNextLevel();
            }
            else
            {
                for (std::list<Level*>::iterator it = this->levels_s.begin(); it != this->levels_s.end(); ++it)
                    if ((*it) == level)
                        this->levels_s.erase(it);
            }
        }
    }

    Level* LevelManager::getActiveLevel()
    {
        if (this->levels_s.size() > 0)
            return this->levels_s.front();
        else
            return 0;
    }

    void LevelManager::activateNextLevel()
    {
        if (this->levels_s.size() > 0)
        {
            this->levels_s.front()->setActive(true);
            for (std::map<unsigned int, PlayerInfo*>::const_iterator it = PlayerManager::getInstance().getClients().begin(); it != PlayerManager::getInstance().getClients().end(); ++it)
                this->levels_s.front()->playerEntered(it->second);
        }
    }

    void LevelManager::setDefaultLevel(const std::string& levelName)
    {
        ModifyConfigValue(defaultLevelName_, set, levelName);
    }

    const std::string& LevelManager::getDefaultLevel() const
    {
        return defaultLevelName_;
    }

    std::string LevelManager::getAvailableLevelListItem(unsigned int index) const
    {
        if (index >= availableLevels_.size())
            return std::string();
        else
            return availableLevels_[index];
    }

    void LevelManager::compileAvailableLevelList()
    {
        availableLevels_.clear();

        boost::filesystem::directory_iterator file(Core::getMediaPathString() + "levels");
        boost::filesystem::directory_iterator end;

        while (file != end)
        {
            if (!boost::filesystem::is_directory(*file) && file->string()[file->string().length()-1] != '~')
            {
                std::string filename = file->path().leaf();
                if (filename.length() > 4)
                    availableLevels_.push_back(filename.substr(0,filename.length()-4));
            }
            ++file;
        }
    }
}
