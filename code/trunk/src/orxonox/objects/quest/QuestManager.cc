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
    @file QuestManager.cc
    @brief Implementation of the QuestManager class.
*/

#include "OrxonoxStableHeaders.h"
#include "QuestManager.h"

#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/input/InputManager.h"
#include "util/Convert.h"

#include "util/Exception.h"
#include "gui/GUIManager.h"
#include "Quest.h"
#include "QuestHint.h"

namespace orxonox
{
    //! Pointer to the current (and single) instance of this class.
    /*static*/ QuestManager* QuestManager::singletonRef_s = NULL;
    /*static*/ bool QuestManager::GUIOpen = false;

    SetConsoleCommand(QuestManager, toggleQuestGUI, true);

    /**
    @brief
        Constructor. Registers the object.
    @todo
        Is inheriting from BaseObject proper?
    */
    QuestManager::QuestManager()
    {
        RegisterRootObject(QuestManager);

        assert(singletonRef_s == 0);
        singletonRef_s = this;
    }

    /**
    @brief
        Destructor.
    */
    QuestManager::~QuestManager()
    {

    }

    /**
    @brief
        Returns a reference to the current (and single) instance of the QuestManager, and creates one if there isn't one to begin with.
    @return
        Returns a reference to the single instance of the Quest Manager.
    */
    /*static*/ QuestManager & QuestManager::getInstance()
    {
        assert(singletonRef_s);
        return *singletonRef_s;
    }

    /**
    @brief
        Registers a Quest with the QuestManager to make it globally accessable.
        Uses it's id to make sure to be able to be identify and retrieve it later.
    @param quest
        The Quest that is to be registered.
    @return
        Returns true if successful, false if not.
    */
    bool QuestManager::registerQuest(Quest* quest)
    {
        if(quest == NULL) //!< Doh! Just as if there were actual quests behind NULL-pointers.
        {
            COUT(2) << "Registration of Quest in QuestManager failed, because inserted Quest-pointer was NULL." << std::endl;
            return false;
        }

        std::pair<std::map<std::string, Quest*>::iterator,bool> result;
        result = this->questMap_.insert( std::pair<std::string,Quest*>(quest->getId(),quest) ); //!< Inserting the Quest.

        if(result.second) //!< If inserting was a success.
        {
            COUT(3) << "Quest with questId {" << quest->getId() << "} successfully inserted." << std::endl;
            return true;
        }
        else
        {
           COUT(2) << "Quest with the same id was already present." << std::endl;
           return false;
        }
    }

    /**
    @brief
        Registers a QuestHint with the QuestManager to make it globally accessable.
        Uses it's id to make sure to be able to be identify and retrieve it later.
    @param hint
        The QuestHint to be registered.
    @return
        Returns true if successful, false if not.
    */
    bool QuestManager::registerHint(QuestHint* hint)
    {
        if(hint == NULL) //!< Still not liking NULL-pointers.
        {
            COUT(2) << "Registration of QuestHint in QuestManager failed, because inserted QuestHint-pointer was NULL." << std::endl;
            return false;
        }

        std::pair<std::map<std::string, QuestHint*>::iterator,bool> result;
        result = this->hintMap_.insert ( std::pair<std::string,QuestHint*>(hint->getId(),hint) ); //!< Inserting the QuestHSint.

        if(result.second) //!< If inserting was a success.
        {
            COUT(3) << "QuestHint with hintId {" << hint->getId() << "} successfully inserted." << std::endl;
            return true;
        }
        else
        {
           COUT(2) << "QuestHint with the same id was already present." << std::endl;
           return false;
        }
    }

    /**
    @brief
        Finds a Quest with the given id.
    @param questId
        The id of the Quest sought for.
    @return
        Returns a pointer to the Quest with the input id.
        Returns NULL if there is no Quest with the given questId.
    @throws
        Throws an exception if the given questId is invalid.
    */
    Quest* QuestManager::findQuest(const std::string & questId)
    {
        if(!QuestItem::isId(questId)) //!< Check vor validity of the given id.
        {
            ThrowException(Argument, "Invalid questId.");
        }

        Quest* quest;
        std::map<std::string, Quest*>::iterator it = this->questMap_.find(questId);
        if (it != this->questMap_.end()) //!< If the Quest is registered.
        {
            quest = it->second;
        }
        else
        {
           quest = NULL;
           COUT(2) << "The quest with id {" << questId << "} is nowhere to be found." << std::endl;
        }

        return quest;

    }

    /**
    @brief
        Finds a QuestHint with the given id.
    @param hintId
        The id of the QuestHint sought for.
    @return
        Returns a pointer to the QuestHint with the input id.
        Returns NULL if there is no QuestHint with the given hintId.
    @throws
        Throws an exception if the given hintId is invalid.
    */
    QuestHint* QuestManager::findHint(const std::string & hintId)
    {
        if(!QuestItem::isId(hintId)) //!< Check vor validity of the given id.
        {
            ThrowException(Argument, "Invalid hintId.");
        }

        QuestHint* hint;
        std::map<std::string, QuestHint*>::iterator it = this->hintMap_.find(hintId);
        if (it != this->hintMap_.end()) //!< If the QuestHint is registered.
        {
            hint = it->second;
        }
        else
        {
           hint = NULL;
           COUT(2) << "The hint with id {" << hintId << "} is nowhere to be found." << std::endl;
        }

        return hint;

    }

    QuestContainer* QuestManager::getQuestTree(std::string & name)
    {
        GUIOverlay* gui = GUIManager::getInstance().getOverlay(name);

        PlayerInfo* player;
        if(gui == NULL)
        {
            COUT(1) << "Something BAD happened." << std::endl;
            return NULL;
        }
        COUT(1) << player << std::endl;
        ConverterExplicit<BaseObject, PlayerInfo>::convert(player, *(gui->getOwner()));
    
        QuestContainer* root = NULL;
        QuestContainer* current = NULL;
        
        std::list<Quest*>* pRootQuests = new std::list<Quest*>();
        std::list<Quest*> rootQuests = *pRootQuests;
        getRootQuests(player, rootQuests);
        
        for(std::list<Quest*>::iterator it = rootQuests.begin(); it != rootQuests.end(); it++)
        {
            Quest* quest = *it;
            
            QuestContainer* container = new QuestContainer;

            container->description = quest->getDescription();
            addHints(container, quest, player);
            addSubQuests(container, quest, player);

            if(root == NULL)
            {
                root = container;
            }
            else
            {
                current->next = container;
            }
            
            current = container;

        }
        if(current != NULL)
            current->next = NULL;

        delete pRootQuests;

        return root;
    }

    void QuestManager::getRootQuests(const PlayerInfo* player, std::list<Quest*> & list)
    {
        for(std::map<std::string, Quest*>::iterator it=this->questMap_.begin(); it!=this->questMap_.end(); it++)
        {
            Quest* quest = (*it).second;
            if(quest->getParentQuest() == NULL && !quest->isInactive(player))
            {
                list.push_back(quest);
            }
        }
    }

    void QuestManager::addSubQuests(QuestContainer* container, Quest* quest, const PlayerInfo* player)
    {
        QuestContainer* current = NULL;
        QuestContainer* first = NULL;

        std::list<Quest*> quests = quest->getSubQuestList();
        for(std::list<Quest*>::iterator it = quests.begin(); it != quests.end(); it++)
        {
            Quest* subQuest = *it;
            if(!subQuest->isInactive(player))
            {
                QuestContainer* subQuestContainer = new QuestContainer;

                subQuestContainer->description = subQuest->getDescription();
                addHints(subQuestContainer, subQuest, player);
                addSubQuests(subQuestContainer, subQuest, player);

                if(first == NULL)
                {
                    first = subQuestContainer;
                }
                else
                {
                    current->next = subQuestContainer;
                }
                
                current = subQuestContainer;
            }
        }

        if(current != NULL)
            current->next = NULL;
        container->subQuests = first;
        
    }

    void QuestManager::addHints(QuestContainer* container, Quest* quest, const PlayerInfo* player)
    {
        HintContainer* current = NULL;
        HintContainer* first = NULL;

        std::list<QuestHint*> hints = quest->getHintsList();
        for(std::list<QuestHint*>::iterator it = hints.begin(); it != hints.end(); it++)
        {
            if((*it)->isActive(player))
            {
                HintContainer* hint = new HintContainer;
                hint->description = (*it)->getDescription();

                if(first == NULL)
                {
                    first = hint;
                }
                else
                {
                    current->next = hint;
                }
                
                current = hint;
            }
        }

        if(current != NULL)
            current->next = NULL;
        container->hint = first;
    }

    /*static*/ void QuestManager::toggleQuestGUI(void)
    {
        if (!QuestManager::GUIOpen)
        {
            GUIManager::getInstancePtr()->showGUI("QuestGUI");
            GUIManager::getInstancePtr()->executeCode("showCursor()");
            InputManager::getInstance().requestEnterState("guiMouseOnly");
            GUIManager::getInstancePtr()->executeCode("loadQuestsList()");
            GUIOpen = true;
        }
        else
        {
            GUIManager::getInstancePtr()->executeCode("hideGUI(\"QuestGUI\")");
            GUIManager::getInstancePtr()->executeCode("hideCursor()");
            InputManager::getInstance().requestLeaveState("guiMouseOnly");
            GUIOpen = false;
        }
    }


}
