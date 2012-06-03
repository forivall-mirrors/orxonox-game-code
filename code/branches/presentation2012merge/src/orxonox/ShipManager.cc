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
 *      Matthias Hutter
 *
 */

/**
    @file ShipManager.cc
    @brief Work-in Progress: Implementation of the ShipManager singleton.
           Should make SpaceShip info available to lua handlers.
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
        Constructor.
    */
    ShipManager::ShipManager()
    {
        RegisterRootObject(LevelManager);
        this->compileAvailableLevelList();
        this->nextIndex_ = 0;
        this->nextLevel_ = this->availableLevels_.begin();
    }

    ShipManager::~ShipManager()
    {
        // Delete all the Ship objects because the LevelManager created them
        std::set<SpaceShip*, LevelInfoCompare>::iterator it = availableLevels_.begin();
        for (; it != availableLevels_.end(); ++it)
            delete *it;
    }


    /**
    @brief
        Get the number of available Ships.
    @return
        Returns the number of available Ships.
    */
    unsigned int LevelManager::getNumberOfLevels(){ return this->availableLevels_.size(); }

    /**
    @brief
        Get the SpaceShip at the given index in the list of available Ships.
        The SpaceShips are sorted in alphabetical order accoridng to the name of the Ship.
        This method is most efficiently called with consecutive indices (or at least ascending indices).
    @param index
        The index of the item that should be returned.
    @return
        Returns a pointer to the SpaceShip at the given index.
    */
    SpaceShip* ShipManager::getAvailableShipListItem(unsigned int index)
    {
        if(index >= this->availableShips_.size())
            return NULL;

        // If this index directly follows the last we can optimize a lot.
        if(index == this->nextIndex_)
        {
            this->nextIndex_++;
            std::set<SpaceShip*, SpaceShipCompare>::iterator it = this->nextShip_;
            this->nextLevel_++;
            return *it;
        }
        else
        {
            // If this index is bigger than the last, we can optimize a little.
            if(index < this->nextIndex_)
            {
                this->nextIndex_ = 0;
                this->nextShip_ = this->availableShips_.begin();
            }

            while(this->nextIndex_ != index)
            {
                this->nextIndex_++;
                this->nextShip_++;
            }
            this->nextIndex_++;
            std::set<SpaceShip*, SpaceShipCompare>::iterator it = this->nextLevel_;
            this->nextLevel_++;
            return *it;
        }
    }

    /**
    @brief
        Compile the list of available Levels.
        Iterates over all *.oxw files, loads the LevelInfo objects in them and from that it creates the LevelInfoItems which are inserted in a list.
    */
    void ShipManager::compileAvailableShipList()
    {
        /*
        // We only want to load as little as possible
        ClassTreeMask mask;
            mask.exclude(Class(BaseObject));
            mask.include(Class(SpaceShip));
        SpaceShip* info = NULL;
        XMLFile file = XMLFile(ship);
        Loader::load(&file, mask, false, true);
        for(ObjectList<SpaceShip>::iterator item = ObjectList<SpaceShip>::begin(); item != ObjectList<SpaceShip>::end(); ++item)
                if(item->getXMLFilename() == *it)
                    info = item->copy();
        Loader::unload(&file);
        */

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
            if (it->find("old/") != 0 )
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
}
