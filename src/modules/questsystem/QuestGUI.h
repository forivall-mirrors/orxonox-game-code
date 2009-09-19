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
    @brief Definition of the QuestGIU class.
*/

#ifndef _QuestGUI_H__
#define _QuestGUI_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <list>
#include <map>
#include <string>
#include <CEGUIForwardRefs.h>

#include "core/OrxonoxClass.h"

namespace orxonox {

    /**
    @brief
        Handles the GUI for the Questsystem.
    @author
        Damian 'Mozork' Frick
    */
    class _QuestsystemExport QuestGUI : public OrxonoxClass
    {

        public:
        
            QuestGUI(PlayerInfo* player);
            virtual ~QuestGUI();

            CEGUI::Window* getGUI(void); //!< Get the root CEGUI Window of the GUI.
            void update(void); //!< Update the GUI.

            /**
            @brief Retreive the CEGUI WindowManager.
            @return Returns the CEGUI WindoWManager.
            */
            inline CEGUI::WindowManager* getWindowManager(void)
                { return this->windowManager_; }
            /**
            @brief Retrieve the root window.
            @return Returns the root window.
            */
            inline CEGUI::Window* getRootWindow(void)
                { return this->rootWindow_; }

            CEGUI::Window* getWindow(void); //!< Get a CEGUI Window to use.
            void giveWindow(CEGUI::Window* window); //!< Return a no longer needed CEGUI Window for reuse.

            /**
            @brief Retreive the player.
            @return Returns the player.
            */
            inline PlayerInfo* getPlayer(void)
                { return this->player_; }

            static QuestGUINode* findNode(CEGUI::Window* window); //!< Finde the QuestGUINode belonging to the input CEGUI Window.
    
        private:
            
            int createNode(QuestGUINode* parent, QuestItem* item, int depth, int index); //!< Recursive method to create Nodes for all Quests an Hints the given Quest is a parent to.

            void clear(void); //!< Clear the QuestGUI.
            
            PlayerInfo* player_; //!< The player that owns the GUI.
            CEGUI::WindowManager* windowManager_; //!< The CEGUI WindowManager. //TODO: needed?
            CEGUI::Window* rootWindow_; //!< The root CEGUI Window of the GUI.

            QuestGUINode* root_; //!< An empty QuestGUINode being the parent of all otherwise parent-less nodes.
            std::map<CEGUI::Window*, QuestGUINode*> nodes_; //!< A list of all QuestGUINodes, ordered by their respective CEGUI Windows.
            std::list<CEGUI::Window*> windows_; //!< A list of windows to be used.

    };

}

#endif /* _QuestGUI_H__ */

