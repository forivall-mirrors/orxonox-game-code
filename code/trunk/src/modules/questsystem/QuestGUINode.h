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

#ifndef _QuestGUINode_H__
#define _QuestGUINode_H__

#include "questsystem/QuestsystemPrereqs.h"
#include <CEGUIForwardRefs.h>

#include "core/OrxonoxClass.h"

#include <list>
#include <string>


namespace orxonox {

    class _QuestsystemExport QuestGUINode : public OrxonoxClass
    {

        public:
    
            QuestGUINode(void);
            QuestGUINode(QuestGUI* gui, QuestGUINode* parent, QuestItem* item, int depth, int index);
            virtual ~QuestGUINode(void);
    
            int toggleVisibility(void);

            void getName(std::string & buffer); //!< Sets the input buffer to the name of the node.
            /**
            @brief Retreive the window of this node.
            @return The CEGUI Window of this node. 
            */
            inline CEGUI::Window* getWindow(void)
                { return this->window_; }
            CEGUI::Window* getDetails(void); //!< Creates the details window.

            bool openDetails(const CEGUI::EventArgs& e); //!< Opens the details window for the Quest/QuestHint clicked on.
            bool closeDetails(const CEGUI::EventArgs& e); //!< Close the details window.

        private:
            static CEGUI::Rect getStaticTextArea(const CEGUI::Window* window);
            static int setHeight(CEGUI::Window* window);

            void initialize(void); //!< Initialize the object.
            void updatePosition(void); //!< Update the position list item.
            void createWindow(void); //!< Helper method to create the CEGUI Window the node.

            bool visible_; //!< Boolean determining the visibility of the node.

            QuestGUI* gui_; //!< The QuestGUI this node belongs to.
            QuestGUINode* parent_; //!< The parent node.
            std::list<QuestGUINode*> subNodes_; //!< a list of all subnodes.
            QuestItem* item_; //!<

            int depth_; //!< The depth (resp. indentation) of this node in the list of Quests. (Irrelevant for QuestHints)
            int index_; //!< The index of this node in the list of Quests, resp. in the list of QuestHints, if the node belongs to a QuestHint, rather than a Quest.

            CEGUI::Window* window_; //!< The list window of this node.
            CEGUI::Window* details_; //!< The details window of this node.

    };

}

#endif /* _QuestGUINode_H__ */

