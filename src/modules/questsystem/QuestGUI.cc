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

#include "QuestGUI.h"

#include <sstream>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>

#include "core/CoreIncludes.h"
#include "Quest.h"
#include "QuestHint.h"
#include "QuestItem.h"
#include "QuestGUINode.h"
#include "QuestManager.h"

namespace orxonox {

    /**
    @brief
        Constructor. Registers and initializes the object.
    @param
        The player the GUI is for.
    */
    QuestGUI::QuestGUI(PlayerInfo* player)
    {
        RegisterRootObject(QuestGUI);

        this->player_ = player;
        this->windowManager_ = CEGUI::WindowManager::getSingletonPtr(); //!< Get CEGUI WindowManager.
        this->rootWindow_ = NULL;
        this->root_ = new QuestGUINode(); //!< Create empty root node.

        COUT(3) << "New QuestGUI created." << std::endl;
    }

    /**
    @brief
        Destructor.
    */
    QuestGUI::~QuestGUI()
    {
        COUT(3) << "Destroying QuestGUI..." << std::endl;

        this->clear(); //!< Clearing the GUI and in the process destroying all QuestGUINodes.

        //! Destroying the windows in the this->windows_ list.
        for(std::list<CEGUI::Window*>::iterator it = this->windows_.begin(); it != this->windows_.end(); it++)
        {
            if(*it != NULL)
                (*it)->destroy();
        }
        this->windows_.clear();

        if(this->root_ != NULL)
            this->root_->destroy();
    }

    /**
    @brief
        Get the root CEGUI Window of the GUI.
    @return
        Returns the root CEGUI Window of the GUI.
    */
    CEGUI::Window* QuestGUI::getGUI(void)
    {
        this->update(); //!< Update the GUI.

        return this->rootWindow_;
    }

    /**
    @brief
        Update the GUI.
    */
    void QuestGUI::update(void)
    {
        COUT(3) << "Updating QuestGUI..." << std::endl;

        this->clear(); //!< Clear the GUI.

        int depth = 0;
        int index = 0;

        //! Create root window.
        this->rootWindow_ = this->windowManager_->createWindow("TaharezLook/ScrollablePane", "QuestGUI/Quests");
        this->rootWindow_->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0),CEGUI::UDim(1, 0)));

        //! Iterate through all Quests.
        std::map<std::string, Quest*> quests = QuestManager::getInstance().getQuests();
        for(std::map<std::string, Quest*>::iterator it = quests.begin(); it != quests.end(); it++)
        {
            Quest* quest = it->second;
            if(quest->getParentQuest() == NULL && !quest->isInactive(this->player_)) //!< If the Quest isn't inactive and a root Quest (meaning it has no parent.), create a Node.
            {
                index = createNode(this->root_, quest, depth, index);
            }
        }
        COUT(3) << "Updating QuestGUI done." << std::endl;
    }

    /**
    @brief
        Clear the QuestGUI.
    */
    void QuestGUI::clear(void)
    {
        COUT(3) << "Clearing QuestGUI..." << std::endl;

        //! Clear all nodes.
        for(std::map<CEGUI::Window*, QuestGUINode*>::iterator it = this->nodes_.begin(); it != this->nodes_.end(); it++)
        {
            QuestGUINode* node = it->second;
            if(node == NULL)
            {
                COUT(1) << "Node is NULL!";
                continue;
            }
            std::string* str = new std::string();
            node->getName(*str);
            COUT(3) << "Clearing Node '" << *str << "' ..." << std::endl;
            delete str;
            node->destroy();
        }
        this->nodes_.clear();

        //! Clear root window.
        if(this->rootWindow_ != NULL)
            this->rootWindow_->destroy();

        COUT(3) << "Clearing QuestGUI done." << std::endl;
    }

    /**
    @brief
        Get a CEGUI Window to use.
        Windows that are no longer used are collected with giveWindow, and are given out again with getWindow, so save some time recreating new windows everytime.
        The retreived window is of type "TaharezLook/TabButton".
    @return
        Returns a CEGUI Window of type "TaharezLook/TabButton".
    */
    CEGUI::Window* QuestGUI::getWindow(void)
    {
        if(!this->windows_.empty()) //!< If there are windows in the list.
        {
            CEGUI::Window* window = this->windows_.back();
            this->windows_.pop_back();
            return window;
        }

        //!< Else create a new one.
        std::ostringstream stream;
        stream << "QuestGUI/Quests/EmptyWindows/" << this->windows_.size()+1;
        return this->windowManager_->createWindow("TaharezLook/TabButton", stream.str());
    }

    /**
    @brief
        Return a no longer needed CEGUI Window for reuse.
    @param window
        The CEGUI window ot be returned.
    */
    void QuestGUI::giveWindow(CEGUI::Window* window)
    {
        if(window == NULL)
            return;
        this->windows_.push_back(window);
        this->rootWindow_->removeChildWindow(window); //!< Remove the window as child of the rootWindow.
        std::ostringstream stream;
        stream << "QuestGUI/Quests/EmptyWindows/" << this->windows_.size();
        window->rename(stream.str());
    }

    /**
    @brief
        Finde the QuestGUINode belonging to the input CEGUI Window.
    @param window
        A pointer to a CEGUI Window.
    @return
        A pointer to the QuestGUI Node belonging to the input CEGUI Window.
    */
    /*static*/ QuestGUINode* QuestGUI::findNode(CEGUI::Window* window)
    {
        for(std::map<PlayerInfo*, QuestGUI*>::iterator it = QuestManager::getInstance().questGUIs_.begin(); it != QuestManager::getInstance().questGUIs_.end(); it++)
        {
            QuestGUI* gui = it->second;
            std::map<CEGUI::Window*, QuestGUINode*>::iterator node = gui->nodes_.find(window);
            if(node != gui->nodes_.end()) return node->second;
        }
        return NULL;
    }

    /**
    @brief
        Recursive method to create Nodes for all Quests an Hints the given Quest is a parent to.
    @param parent
        Pointer to the parent QuestGUINode.
    @param item
        The QuestItem the QuestGUINode is created for.
    @param depth
        Parameter to define how much the list item has to be indented.
    @param index
        "Counter" for Quests and Hints.
    @return
        Returns the index.
    */
    int QuestGUI::createNode(QuestGUINode* parent, QuestItem* item, int depth, int index)
    {
        QuestGUINode* node = new QuestGUINode(this, parent, item, depth, index); //!< Create a new QuestGUINode.

        this->nodes_.insert(std::pair<CEGUI::Window*, QuestGUINode*>(node->getWindow(),node)); //!< Insert the node and its window in the nodes_ map.

        index++;

        //! Check if the QuestItem is a Quest, if not (it's a QuestHint) it just returns.
        Quest* quest = dynamic_cast<Quest*>(item);
        if(quest == NULL)
          return index;

        //! Iterate through all subQuests.
        std::list<Quest*> quests = quest->getSubQuestList();
        for(std::list<Quest*>::iterator it = quests.begin(); it != quests.end(); it++)
        {
            Quest* quest = *it;
            if(!quest->isInactive(this->player_)) //!< Add node if the subQuest is not inactive.
            {
                index = createNode(node, quest, depth+1, index);
            }
        }

        //! Iterate through all hints.
        std::list<QuestHint*> hints = quest->getHintsList();
        int tempIndex = index; //!< Preserve the index, since for the hints we start anew with index 0.
        index = 0;
        for(std::list<QuestHint*>::iterator it = hints.begin(); it != hints.end(); it++)
        {
            QuestHint* hint = *it;
            if(hint->isActive(this->player_)) //!< Add node if the hint is active.
            {
                index = createNode(node, hint, depth+1, index);
            }
        }
        index = tempIndex; //!< Reset the index to the original level.

        return index;
    }

}

