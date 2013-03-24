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
    @file LevelManager.h
    @brief Definition of the LevelManager singleton.
    @ingroup Orxonox
*/

#ifndef _LevelManager_H__
#define _LevelManager_H__

#include "OrxonoxPrereqs.h"

#include <cassert>
#include <list>
#include <map>
#include <string>

#include "LevelInfo.h"

#include "util/Singleton.h"
#include "core/class/OrxonoxClass.h"

// tolua_begin
namespace orxonox
{

    /**
    @brief
        The LevelManager keeps track of @ref orxonox::Level "Levels" whose activity has been requested and activates the @ref orxonox::Level "Levels" in the list in a FIFO manner with exactly one Level (the first in the list) being active at all times (unless the list is empty).
        It also serves as an access point to get a list of all available @ref orxonox::Level "Levels" (or rather their respective @ref orxonox::LevelInfoItem "LevelInfoItems").

    @author
        Fabian 'x3n' Landau

    @author
        Damian 'Mozork' Frick

    @ingroup Orxonox
    */
    class _OrxonoxExport LevelManager
    // tolua_end
        : public Singleton<LevelManager>, public OrxonoxClass
    { // tolua_export
            friend class Singleton<LevelManager>;
        public:
            LevelManager();
            virtual ~LevelManager();

            void setConfigValues(); //!< Set the config values for this object.

            void requestActivity(Level* level); //!< Request activity for the input Level.
            void releaseActivity(Level* level); //!< Release activity for the input Level.
            Level* getActiveLevel(); //!< Get the currently active Level.

            // tolua_begin
            void setDefaultLevel(const std::string& levelName); //!< Set the default Level.
            /**
            @brief Get the default level.
            @return Returns the filename of the default level.
            */
            const std::string& getDefaultLevel() const
                { return defaultLevelName_; }
            unsigned int getNumberOfLevels(void);
            LevelInfoItem* getAvailableLevelListItem(unsigned int index); //!< Get the LevelInfoItem at the given index in the list of available Levels.

            /**
            @brief Get the instance of the LevelManager.
            @return Returns the instance of the LevelManager.
            */
            static LevelManager& getInstance()
                { return Singleton<LevelManager>::getInstance(); }
            // tolua_end

        private:
            LevelManager(const LevelManager&);

            void activateNextLevel(); //!< Activate the next level.

            void compileAvailableLevelList(void); //!< Compile the list of available Levels.
            void updateAvailableLevelList(void); //!< Update the list of available Levels.

            std::list<Level*> levels_; //!< A list of all the Levels whose activity has been requested, in the order in which they will become active.
            std::set<LevelInfoItem*, LevelInfoCompare> availableLevels_; //!< The set of available Levels sorted alphabetically according to the name of the Level.

            // Helpers to allow fast access to the availableLevels list.
            unsigned int nextIndex_; //! The next expected index to be accessed.
            std::set<LevelInfoItem*, LevelInfoCompare>::iterator nextLevel_; //! The next expected Level to be accessed.

            // config values
            std::string defaultLevelName_;

            static LevelManager* singletonPtr_s;
    }; // tolua_export
} // tolua_export

#endif /* _LevelManager_H__ */
