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

#include "util/ScopedSingletonManager.h"
#include "core/ClassTreeMask.h"
#include "core/CommandLineParser.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/Loader.h"
#include "core/Resource.h"
#include "core/XMLFile.h"
#include "PlayerManager.h"
#include "Level.h"
#include "LevelInfo.h"

namespace orxonox
{
    SetCommandLineArgument(level, "").shortcut("l").information("Default level file (overrides LevelManager::defaultLevelName_ configValue)");

    ManageScopedSingleton(LevelManager, ScopeID::Root, false);

    LevelManager::LevelManager()
    {
        RegisterRootObject(LevelManager);
        this->setConfigValues();

        // check override
        if (!CommandLineParser::getArgument("level")->hasDefaultValue())
        {
            ModifyConfigValue(defaultLevelName_, tset, CommandLineParser::getValue("level").getString());
        }

        this->compileAvailableLevelList();
    }

    LevelManager::~LevelManager()
    {
    }

    void LevelManager::setConfigValues()
    {
        SetConfigValue(defaultLevelName_, "presentation_dm.oxw")
            .description("Sets the pre selection of the level in the main menu.");
    }

    void LevelManager::requestActivity(Level* level)
    {
        assert( std::find(this->levels_s.begin(), this->levels_s.end(), level)==this->levels_s.end() );
        if( std::find(this->levels_s.begin(), this->levels_s.end(), level)!=this->levels_s.end() )
            return; // level is already in list
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

    unsigned int LevelManager::getNumberOfLevels()
    {
        this->updateAvailableLevelList();

        COUT(0) << "Number of Levels: " << this->infos_.size() << std::endl;

        return this->availableLevels_.size();
    }

    const std::string& LevelManager::getAvailableLevelListItem(unsigned int index) const
    {
        if (index >= this->availableLevels_.size())
            return BLANKSTRING;
        else
        {
            std::map<std::string, LevelInfoItem*>::const_iterator it = this->infos_.find(this->availableLevels_[index]);
            assert(it->second);
            return it->second->getName();
        }
    }

    void LevelManager::compileAvailableLevelList()
    {
        Ogre::StringVectorPtr levels = Resource::findResourceNames("*.oxw");
        // Iterate over all *.oxw level files.
        for (Ogre::StringVector::const_iterator it = levels->begin(); it != levels->end(); ++it)
        {
            //TODO: Replace with tag,
            if (it->find("old/") != 0)
            {
                size_t pos = it->find(".oxw");

                bool infoExists = false;
                // Load the LevelInfo object from the level file.
                XMLFile file = XMLFile(*it);
                ClassTreeMask mask = ClassTreeMask();
                mask.exclude(ClassIdentifier<BaseObject>::getIdentifier());
                mask.include(ClassIdentifier<LevelInfo>::getIdentifier());
                Loader::load(&file, mask);
                for(ObjectList<LevelInfo>::iterator item = ObjectList<LevelInfo>::begin(); item != ObjectList<LevelInfo>::end(); ++item)
                {
                    LevelInfoItem* info = item->copy();
                    COUT(0) << "BUUUUUUUUUH: " << info->getName() << " | " << info->getXMLFilename() << " | " << it->substr(0, pos) << std::endl;
                    if(info->getXMLFilename() == *it)
                    {
                        this->infos_.insert(std::pair<std::string, LevelInfoItem*>(it->substr(0, pos),info));
                        infoExists = true;
                    }
                }
                Loader::unload(&file, mask);
                if(!infoExists)
                {
                    this->infos_.insert(std::pair<std::string, LevelInfoItem*>(it->substr(0, pos), new LevelInfoItem(it->substr(0, pos), *it)));
                }

                this->availableLevels_.push_back(it->substr(0, pos));
            }
        }
    }

    void LevelManager::updateAvailableLevelList(void)
    {
        //TODO: Implement some kind of update?
    }
}
