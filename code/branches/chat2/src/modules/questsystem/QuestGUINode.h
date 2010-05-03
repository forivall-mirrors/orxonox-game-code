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

#include <list>
#include <string>
#include <CEGUIForwardRefs.h>
#include "core/OrxonoxClass.h"


namespace orxonox {

    class _QuestsystemExport QuestGUINode : public OrxonoxClass
    {

        public:

            QuestGUINode(void);
            QuestGUINode(QuestGUI* gui, QuestGUINode* parent, QuestItem* item, int depth, int index);
            virtual ~QuestGUINode(void);

            void toggleVisibility(void);

            void getName(std::string & buffer); //!< Sets the input buffer to the name of the node.
            /**
            @brief Retreive the window of this node.
            @return The CEGUI Window of this node.
            */
            inline CEGUI::Window* getWindow(void)
                { return this->window_; }

            bool openDetails(const CEGUI::EventArgs& e); //!< Opens the details window for the Quest/QuestHint clicked on.
            bool closeDetails(const CEGUI::EventArgs& e); //!< Close the details window.

        private:
            CEGUI::Window* getDetails(void); //!< Creates the details window.

            void initialize(void); //!< Initialize the object.
            void updatePosition(void); //!< Update the position list item.
            void createWindow(void); //!< Helper method to create the CEGUI Window the node.
            static CEGUI::Rect getStaticTextArea(const CEGUI::Window* window); //Helper method for setHeight(). Gets the StaticTextArea for an input CEGUI Window.
            static int setHeight(CEGUI::Window* window); //Helper method to adjust the height of an input Window (of type StaticText) to the text it holds.

            bool visible_; //!< Boolean determining the visibility of the node.

            QuestGUI* gui_; //!< The QuestGUI this node belongs to.
            QuestGUINode* parent_; //!< The parent node.
            std::list<QuestGUINode*> subNodes_; //!< A list of all subnodes.
            QuestItem* item_; //!< QuestItem belonging to this node.

            int depth_; //!< The depth (resp. indentation) of this node in the list of Quests. (Irrelevant for QuestHints)
            int index_; //!< The index of this node in the list of Quests, resp. in the list of QuestHints, if the node belongs to a QuestHint, rather than a Quest.

            CEGUI::Window* window_; //!< The list window of this node.
            CEGUI::Window* details_; //!< The details window of this node.

            //! Some magic numbers
            static const int TITLE_HEIGHT = 26;
            static const int BORDER_WIDTH = 5;
            static const int SCROLLBAR_WIDTH = 13;
            static const int BUTTON_HEIGHT = 30;
            static const int INDENT_WIDTH = 20;

    };

}

#endif /* _QuestGUINode_H__ */

