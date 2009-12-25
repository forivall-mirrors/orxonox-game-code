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

#include "questsystem/QuestsystemPrereqs.h"
#include <CEGUIForwardRefs.h>

#include <list>
#include <map>
#include <string>

#include "util/Singleton.h"
#include "core/OrxonoxClass.h"

#include "QuestGUI.h"

// tolua_begin
namespace orxonox
{
    /**
    @brief
        Is a Singleton and manages Quests, by registering every Quest/QuestHint (through registerX()) and making them globally accessable (through findX()).
        Quests (and QuestHints) are registered in the QuestManager with their id, and can be accessed in the same way.
    @author
        Damian 'Mozork' Frick
    */
    class _QuestsystemExport QuestManager
// tolua_end
        : public Singleton<QuestManager>, public orxonox::OrxonoxClass
    { // tolua_export

            friend class Singleton<QuestManager>;
            friend class QuestGUI;

        public:
            QuestManager();
            virtual ~QuestManager();

            //! Returns a reference to the single instance of the Quest Manager.
            static QuestManager& getInstance() { return Singleton<QuestManager>::getInstance(); } // tolua_export

            //! Retreive the main window for the GUI.
            CEGUI::Window* getQuestGUI(const std::string & guiName); // tolua_export

            bool registerQuest(Quest* quest); //!< Registers a Quest in the QuestManager.
            bool registerHint(QuestHint* quest); //!< Registers a QuestHint in the QuestManager.

            Quest* findQuest(const std::string & questId); //!< Returns the Quest with the input id.
            QuestHint* findHint(const std::string & hintId); //!< Returns the QuestHint with the input id.

        protected:
            std::map<std::string, Quest*> & getQuests(void); //!< Retreive all Quests.

        private:
            static QuestManager* singletonPtr_s;
            PlayerInfo* retreivePlayer(const std::string & guiName); //!< Retrieve the player for a certain GUI.

            std::map<std::string, Quest*> questMap_; //!< All Quests registered by their id's.
            std::map<std::string, QuestHint*> hintMap_; //!< All QuestHints registered by their id's.

            std::map<PlayerInfo*, QuestGUI*> questGUIs_; //!< All GUI's registered by the players.

    }; // tolua_export

} // tolua_export

#endif /* _QuestManager_H__ */
