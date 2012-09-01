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

#ifndef _LevelManager_H__
#define _LevelManager_H__

#include "OrxonoxPrereqs.h"

#include <cassert>
#include <list>
#include <string>

#include "util/Singleton.h"
#include "core/OrxonoxClass.h"

// tolua_begin
namespace orxonox
{
    class _OrxonoxExport LevelManager
    // tolua_end
        : public Singleton<LevelManager>, public OrxonoxClass
    { // tolua_export
            friend class Singleton<LevelManager>;
        public:
            LevelManager();
            virtual ~LevelManager();

            void setConfigValues();

            void requestActivity(Level* level);
            void releaseActivity(Level* level);
            Level* getActiveLevel();

            void setDefaultLevel(const std::string& levelName); //tolua_export
            const std::string& getDefaultLevel() const; //tolua_export
            void compileAvailableLevelList(); //tolua_export
            const std::string& getAvailableLevelListItem(unsigned int index) const; //tolua_export

            static LevelManager* getInstancePtr() { return singletonPtr_s; }
            static LevelManager& getInstance()    { return Singleton<LevelManager>::getInstance(); } // tolua_export

        private:
            LevelManager(const LevelManager&);

            void activateNextLevel();

            std::list<Level*> levels_s;
            std::vector<std::string> availableLevels_;

            // config values
            std::string defaultLevelName_;

            static LevelManager* singletonPtr_s;
    }; // tolua_export
} // tolua_export

#endif /* _LevelManager_H__ */
