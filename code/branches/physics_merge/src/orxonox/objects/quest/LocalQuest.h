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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */
 
/**
    @file
    @brief Definition of the LocalQuest class.
*/

#ifndef _LocalQuest_H__
#define _LocalQuest_H__

#include "OrxonoxPrereqs.h"

#include <map>
#include <string>

#include "core/XMLPort.h"
#include "Quest.h"

namespace orxonox
{
    /**
    @brief
        Handles Quests which have different states for different players.
        LocalQuests have (as opposed to GlobalQuests) a different state for each player, that means if for one player the status of the Quest changes it does not for all the other players which also possess this quest.
        
        Creating a LocalQuest through XML goes as follows:
        
        <LocalQuest id="questId"> //Where questId is a GUID, see http://en.wikipedia.org/wiki/Globally_Unique_Identifier#Basic_structure for more information
            <QuestDescription title="Title" description="Description." /> //The description of the quest.
            <subquests>
                <Quest id ="questId1" /> //A list of n subquest, be aware, each of the <Quest /> tags must have a description and so on and so forth as well.
                ...
                <Quest id="questIdn" />
                </subquests>
                <hints>
                <QuestHint id="hintId1" /> //A list of n QuestHints, see QuestHint for the full XML representation of those.
                ...
                <QuestHint id="hintIdn" />
                </hints>
            <fail-effects>
                <QuestEffect /> //A list of QuestEffects, invoked when the Quest is failed, see QuestEffect for the full XML representation.
                ...
                <QuestEffect />
            </fail-effects>
            <complete-effects>
                <QuestEffect /> //A list of QuestEffects, invoked when the Quest is completed, see QuestEffect for the full XML representation.
                ...
                <QuestEffect />
            </complete-effects>
        </LocalQuest>
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport LocalQuest : public Quest
    {
        public:
            LocalQuest(BaseObject* creator);
            virtual ~LocalQuest();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a LocalQuest object through XML.
            
            virtual bool fail(PlayerInfo* player); //!< Fails the Quest.
            virtual bool complete(PlayerInfo* player); //!< Completes the Quest.

        protected:
            virtual bool isStartable(const PlayerInfo* player) const; //!< Checks whether the Quest can be started.
            virtual bool isFailable(const PlayerInfo* player) const; //!< Checks whether the Quest can be failed.
            virtual bool isCompletable(const PlayerInfo* player) const; //!< Checks whether the Quest can be completed.

            virtual questStatus::Enum getStatus(const PlayerInfo* player) const; //!< Returns the status of the Quest for a specific player.
            virtual bool setStatus(PlayerInfo* player, const questStatus::Enum & status); //!< Sets the status for a specific player.

        private:
            std::map<const PlayerInfo*, questStatus::Enum> playerStatus_; //!< List of the status for each player, with the Player-pointer as key.

    };

}

#endif /* _LocalQuest_H__ */
