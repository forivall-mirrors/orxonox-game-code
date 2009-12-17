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

#include "core/ConsoleCommand.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/ScopedSingletonManager.h"

namespace orxonox
{
    ManageScopedSingleton(MoodManager, ScopeID::Root, false);

    MoodManager::MoodManager()
    {
        RegisterRootObject(MoodManager);
        this->setConfigValues();
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(createFunctor(&MoodManager::setMood, this), "setMood"));
    }

    MoodManager::~MoodManager()
    {
    }

    void MoodManager::setConfigValues()
    {
        SetConfigValue(mood_, "default")
            .description("Sets the mood for the current level.")
            .callback(this, &MoodManager::checkMoodValidity);
    }

    /** Sets the mood
    @note
        TODO: Inform dependent classes of mood change
    */
    void MoodManager::setMood(const std::string& mood)
    {
        ModifyConfigValue(mood_, set, mood);
    }
    
    //! Gets the current mood
    const std::string& MoodManager::getMood()
    {
        return mood_;
    }

    void MoodManager::checkMoodValidity()
    {
        if (mood_ != "default" && mood_ != "dnb") // Insert new moods here; TODO: make this generic
            ResetConfigValue(mood_);
        COUT(4) << "MoodManager: Mood set to " << mood_ << std::endl;
    }
}
