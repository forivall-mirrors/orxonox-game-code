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
    @brief Definition of the QuestManager class.
*/

#ifndef _QuestManager_H__
#define _QuestManager_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include <map>
#include <string>

#include "util/Singleton.h"
#include "core/OrxonoxClass.h"

// tolua_begin
namespace orxonox
{

    struct QuestContainer;
    struct HintContainer;

    struct QuestContainer
    {
        const QuestDescription* description;
        std::string status;
        HintContainer* hint;
        QuestContainer* subQuests;
        QuestContainer* next;
    };

    struct HintContainer
    {
        const QuestDescription* description;
        HintContainer* next;
    };

    /**
    @brief
        Is a Singleton and manages Quests, by registering every Quest/QuestHint (through registerX()) and making them globally accessable (through findX()).
        Quests (and QuestHints) are registered in the QuestManager with their id, and can be accessed in the same way.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport QuestManager : public Singleton<QuestManager>, public orxonox::OrxonoxClass
    {
// tolua_end
            friend class Singleton<QuestManager>;
        public:
            QuestManager();
            virtual ~QuestManager();

            //! Returns a reference to the single instance of the Quest Manager.
            static QuestManager& getInstance() { return Singleton<QuestManager>::getInstance(); } // tolua_export

            bool registerQuest(Quest* quest); //!< Registers a Quest in the QuestManager.
            bool registerHint(QuestHint* quest); //!< Registers a QuestHint in the QuestManager.

            Quest* findQuest(const std::string & questId); //!< Returns the Quest with the input id.
            QuestHint* findHint(const std::string & hintId); //!< Returns the QuestHint with the input id.

            QuestContainer* getQuestTree(std::string & name); // tolua_export

        private:
            static QuestManager* singletonPtr_s;

            std::map<std::string, Quest*> questMap_; //!< All Quests registered by their id's.
            std::map<std::string, QuestHint*> hintMap_; //!< All QuestHints registered by their id's.

            void getRootQuests(const PlayerInfo* player, std::list<Quest*> & list);
            HintContainer* addHints(Quest* quest, const PlayerInfo* player);
            QuestContainer* addSubQuest(Quest* quest, const PlayerInfo* player);

    }; // tolua_export

} // tolua_export

#endif /* _QuestManager_H__ */
