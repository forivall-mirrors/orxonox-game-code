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
#include "core/OrxonoxClass.h"

namespace orxonox
{
    /**
     * The MoodManager class enables the game to set different themes, i.e. audio themes, each
     * with a set of different audio files. A theme (called mood) is set by the server and applies to
     * all clients. Existing moods are currently hard-coded in function checkMoodValidity(). Each mood
     * needs to have a folder with its name in "data_extern/audo/ambient/" containing sound files named like
     * the ones in mood "default".
     */
    class _OrxonoxExport MoodListener : virtual public OrxonoxClass
    {
        friend class MoodManager;

        protected:
            MoodListener();
            virtual ~MoodListener() {}

            const std::string& getMood() const { return mood_s; }

        private:
            virtual void moodChanged(const std::string& mood) = 0;

            static void changedMood(const std::string& mood);
            static std::string mood_s;
    };

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

            static MoodManager* singletonPtr_s;
    };
}

#endif /* _MoodManager_H__ */
