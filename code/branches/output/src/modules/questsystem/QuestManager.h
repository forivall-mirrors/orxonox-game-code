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
    @file QuestManager.h
    @brief Definition of the QuestManager class.
    @ingroup Questsystem
*/

#ifndef _QuestManager_H__
#define _QuestManager_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <list>
#include <map>
#include <string>

#include "util/Singleton.h"
#include "core/OrxonoxClass.h"

namespace orxonox  // tolua_export
{  // tolua_export

    /**
    @brief
        Is a Singleton and manages @ref orxonox::Quest "Quests", by registering every @ref orxonox::Quest "Quest" / @ref orxonox::QuestHint "QuestHint" (through registerX()) and making them globally accessible (through findX()).
        @ref orxonox::Quest "Quests" (and @ref orxonox::QuestHint "QuestHints") are registered in the QuestManager with their id, and can be accessed in the same way.

    @author
        Damian 'Mozork' Frick

    @ingroup Questsystem
    */
    class _QuestsystemExport QuestManager  // tolua_export
        : public Singleton<QuestManager>, public orxonox::OrxonoxClass
    { // tolua_export

            friend class Singleton<QuestManager>;

        public:
            QuestManager();
            virtual ~QuestManager();

            //! Returns a reference to the single instance of the QuestManager.
            static QuestManager& getInstance() { return Singleton<QuestManager>::getInstance(); } // tolua_export

            // tolua_begin
            int getNumRootQuests(orxonox::PlayerInfo* player); //!< Get the number of Quests the input player has, that are root quests.
            Quest* getRootQuest(orxonox::PlayerInfo* player, int index); //!< Get the index-th root quest of the input player.

            int getNumSubQuests(Quest* quest, orxonox::PlayerInfo* player); //!< Get the number of sub-quest of an input Quest for an input player.
            Quest* getSubQuest(Quest* quest, orxonox::PlayerInfo* player, int index); //!< Get the index-th sub-quest of the input Quest for the input player.

            int getNumHints(Quest* quest, orxonox::PlayerInfo* player); //!< Get the number of QuestHints of the input Quest for the input player.
            QuestHint* getHints(Quest* quest, orxonox::PlayerInfo* player, int index); //!< Get the index-th QuestHint of the input Quest for the input player.

            Quest* getParentQuest(Quest* quest); //!< Get the parent-quest of the input Quest.

            QuestDescription* getDescription(Quest* item); //!< Get the QuestDescription of the input Quest.
            QuestDescription* getDescription(QuestHint* item); //!< Get the QuestDescription of the input QuestHint.

            const std::string getId(Quest* item) const; //!< Get the id of the input Quest.
            const std::string getId(QuestHint* item) const; //!< Get the id of the input QuestHint.
            // tolua_end

            bool registerQuest(Quest* quest); //!< Registers a Quest in the QuestManager.
            bool unregisterQuest(Quest* quest); //!< Unregisters a Quest in the QuestManager.
            bool registerHint(QuestHint* hint); //!< Registers a QuestHint in the QuestManager.
            bool unregisterHint(QuestHint* hint); //!< Unregisters a QuestHint in the QuestManager.

            Quest* findQuest(const std::string & questId); //!< Returns the Quest with the input id.
            QuestHint* findHint(const std::string & hintId); //!< Returns the QuestHint with the input id.

        protected:
            std::map<std::string, Quest*> & getQuests(void); //!< Retrieve all Quests.

        private:
            static QuestManager* singletonPtr_s;
            PlayerInfo* retrievePlayer(const std::string & guiName); //!< Retrieve the player for a certain GUI.

            std::map<std::string, Quest*> questMap_; //!< All Quests registered by their id's.
            std::map<std::string, QuestHint*> hintMap_; //!< All QuestHints registered by their id's.

    }; // tolua_export

} // tolua_export

#endif /* _QuestManager_H__ */
