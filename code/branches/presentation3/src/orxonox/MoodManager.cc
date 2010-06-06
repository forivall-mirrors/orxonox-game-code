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

#include "MoodManager.h"

#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/ScopedSingletonManager.h"
#include "core/Resource.h"

namespace orxonox
{
    ManageScopedSingleton(MoodManager, ScopeID::Root, false);

    // Note: I'm (Kevin Young) not entirely sure whether that's good code style:
    const std::string MoodManager::defaultMood_ = "default";

    MoodManager::MoodManager()
    {
        RegisterRootObject(MoodManager);
        this->setConfigValues();
        
        // Checking for the existence of the folder for the default mood
        /* Note: Currently Resource::exists(path) will always return false when the path field points to a folder.
           MoodManager::checkMoodValidity() performs the same check. Please help.
        */
        const std::string& path = "ambient/" + MoodManager::defaultMood_ + '/';
        if (!Resource::exists(path))
        {
            // TODO: Non-fatal error handling (non-critical resource missing)
            COUT(2) << "Mood Warning: Folder for default mood (" << MoodManager::defaultMood_ << ") does not exist!" << std::endl;
        }
    }

    void MoodManager::setConfigValues()
    {
        SetConfigValue(mood_, MoodManager::defaultMood_)
            .description("Sets the mood for the current level.")
            .callback(this, &MoodManager::checkMoodValidity);
    }

    /** Set a new mood
     */
    void MoodManager::setMood(const std::string& mood)
    {
        ModifyConfigValue(mood_, set, mood);
    }

    void MoodManager::checkMoodValidity()
    {
        //  Generic mood validation
        const std::string& path = "ambient/" + mood_ + '/';
        if (!Resource::exists(path))
        {
            COUT(3) << "Mood " << mood_ << " does not exist. Will not change." << std::endl;
            ResetConfigValue(mood_);
        }
        else
        {
            COUT(3) << "Mood changed to " << mood_ << std::endl;
            MoodListener::changedMood(mood_);
        }
    }


    std::string MoodListener::mood_s;

    MoodListener::MoodListener()
    {
        RegisterRootObject(MoodListener);
    }

    /*static*/ void MoodListener::changedMood(const std::string& mood)
    {
        mood_s = mood;
        for (ObjectList<MoodListener>::iterator it = ObjectList<MoodListener>::begin(); it; ++it)
            it->moodChanged(mood_s);
    }
}
