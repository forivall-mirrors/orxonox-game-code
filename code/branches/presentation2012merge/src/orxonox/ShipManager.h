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
 */

/**
    @file ShipManager.h
    @brief Definition of the ShipManager singleton.
    @ingroup Orxonox
*/

#ifndef _ShipManager_H__
#define _ShipManager_H__

#include "OrxonoxPrereqs.h"

#include <cassert>
#include <list>
#include <map>
#include <string>
#include "worldentities/pawns/SpaceShip.h"

#include "util/Singleton.h"
#include "core/OrxonoxClass.h"

// tolua_begin
namespace orxonox
{

    /**
    @author
       Matthias Hutter

    @ingroup Orxonox
    */
    class _OrxonoxExport ShipManager
    // tolua_end
        : public Singleton<ShipManager>, public OrxonoxClass
    { // tolua_export
            friend class Singleton<LevelManager>;
        public:
            ShipManager();
            virtual ~ShipManager();

            void setConfigValues(); //!< Set the config values for this object.
            // tolua_begin
            /**
            @brief Get the instance of the LevelManager.
            @return Returns the instance of the LevelManager.
            */
            static ShipManager& getInstance()
                { return Singleton<ShipManager>::getInstance(); }
            // tolua_end

        private:
            ShipManager(const ShipManager&);


            void compileAvailableLevelList(void); //!< Compile the list of available Levels.
            void updateAvailableLevelList(void); //!< Update the list of available Levels.

            std::list<Level*> levels_; //!< A list of all the Levels whose activity has been requested, in the order in which they will become active.
            std::set<LevelInfoItem*, LevelInfoCompare> availableLevels_; //!< The set of available Levels sorted alphabetically according to the name of the Level.

            // Helpers to allow fast access to the availableLevels list.
            unsigned int nextIndex_; //! The next expected index to be accessed.
            std::set<LevelInfoItem*, LevelInfoCompare>::iterator nextLevel_; //! The next expected Level to be accessed.

            static LevelManager* singletonPtr_s;
    }; // tolua_export
} // tolua_export

#endif /* _LevelManager_H__ */
