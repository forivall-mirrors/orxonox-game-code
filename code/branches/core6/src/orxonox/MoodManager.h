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

#include <string>
#include "util/Singleton.h"
#include "core/class/OrxonoxClass.h"

namespace orxonox
{
    /*
    @brief
        The MoodListener class is aware of a change in themes and directs that info to dependent classes.
    */
    class _OrxonoxExport MoodListener : virtual public OrxonoxClass
    {
        friend class MoodManager;

        protected:
            MoodListener();
            virtual ~MoodListener() {}

        private:
            virtual void moodChanged(const std::string& mood) = 0;

            static void changedMood(const std::string& mood);
    };

    /*
    @brief
        The MoodManager class serves to allow for different musical themes in the game.
    */
    class _OrxonoxExport MoodManager : public Singleton<MoodManager>, public OrxonoxClass
    {
            friend class Singleton<MoodManager>;
        public:
            MoodManager();

            void setConfigValues();

            void setMood(const std::string& mood);
            inline const std::string& getMood() const { return this->mood_; }

            static MoodManager& getInstance() { return Singleton<MoodManager>::getInstance(); }

        private:
            ~MoodManager() {}
            void checkMoodValidity();

            // config values
            std::string mood_;
            std::string oldMood_;
            static const std::string defaultMood_;

            static MoodManager* singletonPtr_s;
    };
}

#endif /* _MoodManager_H__ */
