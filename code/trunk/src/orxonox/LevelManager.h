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
#include "core/OrxonoxClass.h"

// tolua_begin
namespace orxonox
{
    class _OrxonoxExport LevelManager
    // tolua_end
        : public OrxonoxClass
    { // tolua_export
        public:
            LevelManager();
            virtual ~LevelManager();

            void setConfigValues();

            void requestActivity(Level* level);
            void releaseActivity(Level* level);
            Level* getActiveLevel();

            void setDefaultLevel(const std::string& levelName); //tolua_export
            const std::string& getDefaultLevel(); //tolua_export

            static LevelManager* getInstancePtr() { return singletonRef_s; }
            static LevelManager& getInstance() { assert(singletonRef_s); return *singletonRef_s; } // tolua_export

        private:
            LevelManager(const LevelManager&);

            void activateNextLevel();

            std::list<Level*> levels_s;

            // config values
            std::string defaultLevelName_;

            static LevelManager* singletonRef_s;
    }; // tolua_export
} // tolua_export

#endif /* _LevelManager_H__ */
