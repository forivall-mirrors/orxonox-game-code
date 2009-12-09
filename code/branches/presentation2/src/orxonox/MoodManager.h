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
 *      Kevin 'youngk' Young
 *   Co-authors:
 *      ...
 *
 */

#ifndef _MoodManager_H__
#define _MoodManager_H__

#include "OrxonoxPrereqs.h"

#include <cassert>
#include <list>
#include <string>

#include "util/Singleton.h"
#include "core/OrxonoxClass.h"

// tolua_begin
namespace orxonox
{
    class _OrxonoxExport MoodManager
    // tolua_end
        : public Singleton<MoodManager>, public OrxonoxClass
    { // tolua_export
            friend class Singleton<MoodManager>;
        public:
            MoodManager();
            ~MoodManager();

            void setConfigValues();

            void setMood(const std::string& mood);
            const std::string& getMood();

            static MoodManager& getInstance() { return Singleton<MoodManager>::getInstance(); } // tolua_export

        private:

            // config values
            std::string mood_;
            std::string moodOld_;

            void checkMoodValidity();

            static MoodManager* singletonPtr_s;
    }; // tolua_export
} // tolua_export

#endif /* _MoodManager_H__ */
