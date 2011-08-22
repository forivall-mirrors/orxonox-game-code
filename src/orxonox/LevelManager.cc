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
 *      Damian 'Mozork' Frick
 *
 */

/**
    @file LevelManager.cc
    @brief Implementation of the LevelManager singleton.
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
#include "Level.h"
#include "PlayerManager.h"

namespace orxonox
{
    SetCommandLineArgument(level, "").shortcut("l").information("Default level file (overrides LevelManager::defaultLevelName_ configValue)");

    ManageScopedSingleton(LevelManager, ScopeID::Root, false);

    /**
    @brief
        Constructor. Registers the object, sets config values and initializes variables.
    */
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
        this->nextIndex_ = 0;
        this->nextLevel_ = this->availableLevels_.begin();
    }

    LevelManager::~LevelManager()
    {
        // Delete all the LevelInfoItem objects because the LevelManager created them
        std::set<LevelInfoItem*, LevelInfoCompare>::iterator it = availableLevels_.begin();
        for (; it != availableLevels_.end(); ++it)
            delete *it;
    }

    /**
    @brief
        Set the config values for this object.
    */
    void LevelManager::setConfigValues()
    {
        SetConfigValue(defaultLevelName_, "presentationDM.oxw")
            .description("Sets the pre selection of the level in the main menu.");
    }

    /**
    @brief
        Request activity for the input Level.
        The Level will be added to the list of Levels whose activity is requested. The list is accessed in a FIFO manner.
        If the Level is the only Level in the list it will be immediately activated. If not it will be activated as soon as it reaches the front of the list.
    @param level
        A pointer to the Level whose activity is requested.
    */
    void LevelManager::requestActivity(Level* level)
    {
        assert( std::find(this->levels_.begin(), this->levels_.end(), level)==this->levels_.end() );
        // If the level is already in list.
        if( std::find(this->levels_.begin(), this->levels_.end(), level)!=this->levels_.end() )
            return;
        // If it isn't insert it at the back.
        this->levels_.push_back(level);
        // If it is the only level in the list activate it.
        if (this->levels_.size() == 1)
            this->activateNextLevel();
    }

    /**
    @brief
        Release activity for the input Level.
        Removes the Level from the list. If the Level was the one currently active, it is deactivated and the next Level in line is activated.
    @param level
        A pointer to the Level whose activity is to be released.
    */
    void LevelManager::releaseActivity(Level* level)
    {
        if (this->levels_.size() > 0)
        {
            // If the level is the active level in the front of the list.
            if (this->levels_.front() == level)
            {
                // Deactivate it, remove it from the list and activate the next level in line.
                level->setActive(false);
                this->levels_.pop_front();
                this->activateNextLevel();
            }
            else // Else just remove it from the list.
                this->levels_.erase(std::find(this->levels_.begin(), this->levels_.end(), level));
        }
    }

    /**
    @brief
        Get the currently active Level.
    @return
        Returns a pointer to the currently active level or NULL if there currently are no active Levels.
    */
    Level* LevelManager::getActiveLevel()
    {
        if (this->levels_.size() > 0)
            return this->levels_.front();
        else
            return 0;
    }

    /**
    @brief
        Activate the next Level.
    */
    void LevelManager::activateNextLevel()
    {
        if (this->levels_.size() > 0)
        {
            // Activate the level that is the first in the list of levels whose activity has been requested. 
            this->levels_.front()->setActive(true);
            // Make every player enter the newly activated level.
            for (std::map<unsigned int, PlayerInfo*>::const_iterator it = PlayerManager::getInstance().getClients().begin(); it != PlayerManager::getInstance().getClients().end(); ++it)
                this->levels_.front()->playerEntered(it->second);
        }
    }

    /**
    @brief
        Set the default Level.
    @param levelName
        The filename of the default Level.
    */
    void LevelManager::setDefaultLevel(const std::string& levelName)
    {
        ModifyConfigValue(defaultLevelName_, set, levelName);
    }

    /**
    @brief
        Get the number of available Levels.
        Also updates the list of available Levels.
    @return
        Returns the number of available Levels.
    */
    unsigned int LevelManager::getNumberOfLevels()
    {
        this->updateAvailableLevelList();

        return this->availableLevels_.size();
    }

    /**
    @brief
        Get the LevelInfoItem at the given index in the list of available Levels.
        The LevelInfoItems are sorted in alphabetical order accoridng to the name of the Level.
        This method is most efficiently called with consecutive indices (or at least ascending indices).
    @param index
        The index of the item that should be returned.
    @return
        Returns a pointer to the LevelInfoItem at the given index.
    */
    LevelInfoItem* LevelManager::getAvailableLevelListItem(unsigned int index)
    {
        if(index >= this->availableLevels_.size())
            return NULL;

        // If this index directly follows the last we can optimize a lot.
        if(index == this->nextIndex_)
        {
            this->nextIndex_++;
            std::set<LevelInfoItem*, LevelInfoCompare>::iterator it = this->nextLevel_;
            this->nextLevel_++;
            return *it;
        }
        else
        {
            // If this index is bigger than the last, we can optimize a little.
            if(index < this->nextIndex_)
            {
                this->nextIndex_ = 0;
                this->nextLevel_ = this->availableLevels_.begin();
            }

            while(this->nextIndex_ != index)
            {
                this->nextIndex_++;
                this->nextLevel_++;
            }
            this->nextIndex_++;
            std::set<LevelInfoItem*, LevelInfoCompare>::iterator it = this->nextLevel_;
            this->nextLevel_++;
            return *it;
        }
    }

    /**
    @brief
        Compile the list of available Levels.
        Iterates over all *.oxw files, loads the LevelInfo objects in them and from that it creates the LevelInfoItems which are inserted in a list.
    */
    void LevelManager::compileAvailableLevelList()
    {
        // Get all files matching the level criteria
        Ogre::StringVectorPtr levels = Resource::findResourceNames("*.oxw");

        // We only want to load as little as possible
        ClassTreeMask mask;
        mask.exclude(Class(BaseObject));
        mask.include(Class(LevelInfo));

        // Iterate over all the found *.oxw files
        orxout(internal_info) << "Loading LevelInfos..." << endl;
        std::set<std::string> names;
        for (Ogre::StringVector::const_iterator it = levels->begin(); it != levels->end(); ++it)
        {
            // TODO: Replace with tag?
            if (it->find("old/") != 0)
            {
                LevelInfoItem* info = NULL;

                // Load the LevelInfo object from the level file.
                XMLFile file = XMLFile(*it);
                Loader::load(&file, mask, false, true);

                // Find the LevelInfo object we've just loaded (if there was one)
                for(ObjectList<LevelInfo>::iterator item = ObjectList<LevelInfo>::begin(); item != ObjectList<LevelInfo>::end(); ++item)
                    if(item->getXMLFilename() == *it)
                        info = item->copy();

                // We don't need the loaded stuff anymore
                Loader::unload(&file);

                if(info == NULL) 
                {
                    // Create a default LevelInfoItem object that merely contains the name
                    std::string filenameWOExtension = it->substr(0, it->find(".oxw"));
                    info = new LevelInfoItem(filenameWOExtension, *it);
                }

                // Warn about levels with the same name.
                if(!names.insert(info->getName()).second)
                    orxout(internal_warning) << "Multiple levels (" << info->getXMLFilename() << ") with name '" << info->getName() << "' found!" << endl;

                // Warn about multiple items so that it gets fixed quickly
                if(availableLevels_.find(info) != availableLevels_.end())
                {
                    orxout(internal_warning) << "Multiple levels (" << info->getXMLFilename() << ") with same name '" << info->getName() << "' and filename found! Exluding..." << endl;
                    // Delete LevelInfoItem to avoid a dangling pointer
                    delete info;
                }
                else
                    this->availableLevels_.insert(info);
            }
        }
    }

    /**
    @brief
        Update the list of available Levels.
    */
    void LevelManager::updateAvailableLevelList(void)
    {
        //TODO: Implement some kind of update?
    }
}
